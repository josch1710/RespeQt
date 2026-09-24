/*
 * pclink.cpp
 *
 * This file contains code from the SIO2BSD project by KMK (drac030)
 *
 * Copyright 2016 TheMontezuma
 * Copyright 2016 josch1710
 *
 */

#include <QtDebug>
#include <cerrno>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <utime.h>

// The order matters
// clang-format off
#include "respeqtsettings.h"
#include "sio/pclink.h"
// clang-format on

namespace SIO {

static constexpr char DEVICE_LABEL[] = ".PCLINK.VOLUME.LABEL";
static constexpr quint8 PCL_MAX_FNO = 0x14;
static constexpr char RESERVED_NAME_PREFIX_CHAR = '!';

#if defined(Q_OS_WIN)
static constexpr char HOST_SEPARATOR_CHAR = '\\';
#else
static constexpr char HOST_SEPARATOR_CHAR = '/';
#endif

static constexpr char SDX_GO_UP_DIR_CHAR = '<';

/* Atari SIO status block */
using STATUS = struct
{
  uchar stat;
  uchar err;
  uchar tmot;
  uchar none;
};

using PARBUF = struct /* PCLink parameter buffer */
{
  uchar fno;            /* function number */
  uchar handle;         /* file handle */
  uchar f1, f2, f3, f4; /* general-purpose bytes */
  uchar f5, f6;         /* more general-purpose bytes */
  uchar fmode;          /* fmode */
  uchar fatr1;          /* fatr1 */
  uchar fatr2;          /* fatr2 */
  uchar name[12];       /* name */
  uchar names[12];      /* names */
  uchar path[65];       /* path */
};

using DEVICE = struct
{
  STATUS status;      /* the 4-byte status block */
  int on;             /* PCLink mount flag */
  char dirname[1024]; /* PCLink root directory path */
  uchar cwd[65];      /* PCLink current working dir, relative to the above */
  PARBUF parbuf;      /* PCLink parameter buffer */
};

using IODESC = struct
{
  union {
    FILE *file;
    DIR *dir;
  } fps;

  DIRENTRY *dir_cache; /* used only for directories */

  uchar devno;
  uchar cunit;
  uchar fpmode;
  uchar fatr1;
  uchar fatr2;
  uchar t1, t2, t3;
  uchar d1, d2, d3;
  struct stat fpstat;
  char fpname[12];
  size_t fppos; // TODO Perhaps off_t?
  size_t fpread;
  int eof;
  char pathname[1024];
};

using PCLDBF = struct
{
  uchar handle;
  uchar dirbuf[23];
};

static constexpr const char *const invalid_file_names[] =
{
    "CON",
    "PRN",
    "AUX",
    "NUL",
    "COM1",
    "COM2",
    "COM3",
    "COM4",
    "COM5",
    "COM6",
    "COM7",
    "COM8",
    "COM9",
    "LPT1",
    "LPT2",
    "LPT3",
    "LPT4",
    "LPT5",
    "LPT6",
    "LPT7",
    "LPT8",
    "LPT9",
    nullptr
};

static bool D = false;// extended debug
static IODESC iodesc[16];
static DEVICE device[16]; /* 1 PCLINK device with support for 15 units */
static PCLDBF pcl_dbf;
static uchar old_ccom = 0;

static constexpr const char *fun[] =
{
    "FREAD", "FWRITE", "FSEEK", "FTELL", "FLEN", "(none)", "FNEXT", "FCLOSE",
    "INIT", "FOPEN", "FFIRST", "RENAME", "REMOVE", "CHMOD", "MKDIR", "RMDIR",
    "CHDIR", "GETCWD", "SETBOOT", "DFREE", "CHVOL"
};

static char HOST_SEPARATOR_STR[] = {HOST_SEPARATOR_CHAR, 0};
static char RESERVED_NAME_PREFIX_STR[] = {RESERVED_NAME_PREFIX_CHAR, 0};

/*************************************************************************/

PCLINK::PCLINK(const SioWorkerPtr& worker)
    : SDXProtocol(worker) {
  do_pclink_init(1);
}

/*************************************************************************/

void PCLINK::handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) {
  const auto cunit = aux2 & 0x0F; /* PCLink ignores DUNIT */

  if (D) qDebug() << "!n" << tr("PCLINK Command=[$%1] aux1=$%2 aux2=$%3 cunit=$%4").arg(command, 0, 16).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(cunit, 0, 16);

  /* cunit == 0 is init during warm reset */
  if (cunit == 0 || device[cunit].on) {
    switch (command) {
      case 'P': {
        qDebug() << "!n" << tr("[%1] P").arg(deviceName());
        do_pclink(PCLINK_CDEVIC, command, aux1, aux2);
        break;
      }

      case 'R': {
        qDebug() << "!n" << tr("[%1] R").arg(deviceName());
        do_pclink(PCLINK_CDEVIC, command, aux1, aux2);
        break;
      }

      case 'S': /* status */
      {
        sio->port()->writeCommandAck();

        QByteArray status(4, 0);
        status[0] = static_cast<char>(device[cunit].status.stat);
        status[1] = static_cast<char>(device[cunit].status.err);
        status[2] = static_cast<char>(device[cunit].status.tmot);
        status[3] = static_cast<char>(device[cunit].status.none);

        sio->port()->writeComplete();
        sio->port()->writeDataFrame(status);

        qDebug() << "!n" << tr("[%1] Get status for [%2]").arg(deviceName()).arg(cunit);
        break;
      }

      case '?': /* send hi-speed index */
      {
        sio->port()->writeCommandAck();
        sio->port()->writeComplete();
        QByteArray speed(1, 0);
        speed[0] = static_cast<char>(sio->port()->speedByte());
        sio->port()->writeDataFrame(speed);
        qDebug() << "!n" << tr("[%1] Speed poll").arg(deviceName());
        break;
      }

      default: {
        sio->port()->writeCommandNak();
        qWarning() << "!w" << tr("[%1] command: $%2, aux: $%3 NAKed.").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        break;
      }
    }
  }
}

/*************************************************************************/

// ReSharper disable once CppMemberFunctionMayBeStatic
bool PCLINK::hasLink(const int no) { // NOLINT(*-convert-member-functions-to-static)
  if (no < 1 || no > 15) return false;

  return device[no].on == 1;
}

/*************************************************************************/

void PCLINK::setLink(const int no, const char *fileName) {
  if (no < 1 || no > 15) return;

  fps_close(no);
  memset(&device[no].parbuf, 0, sizeof(PARBUF));

  strncpy(device[no].dirname, fileName, 1023);
  device[no].dirname[1023] = 0;
  device[no].cwd[0] = 0;
  device[no].on = 1;

  if (D) qDebug() << "!n" << tr("PCLINK[%1] Mount %2").arg(no).arg(fileName);
}

/*************************************************************************/

void PCLINK::swapLinks(const int from, const int to) {
  if (from < 1 || from > 15 || to < 1 || to > 15) return;

  char tmp_dir_name[1024];
  if (hasLink(from)) {
    strncpy(tmp_dir_name, device[from].dirname, 1023);
    tmp_dir_name[1023] = 0;
    resetLink(from);
    if (hasLink(to)) {
      setLink(from, device[to].dirname);
      resetLink(to);
    }
    setLink(to, tmp_dir_name);
  } else if (hasLink(to)) {
    setLink(from, device[to].dirname);
    resetLink(to);
  }
}

/*************************************************************************/

void PCLINK::resetLink(const int no) {
  if (no < 1 || no > 15) return;

  fps_close(no);
  memset(&device[no].parbuf, 0, sizeof(PARBUF));

  device[no].on = 0;
  device[no].dirname[0] = 0;
  device[no].cwd[0] = 0;

  if (D) qDebug() << "!n" << tr("PCLINK[%1] Unmount").arg(no);
}

/*************************************************************************
 * Command Execution & Dispatcher
 *************************************************************************/

void PCLINK::completeCommand(const quint8 cunit) {
  Q_UNUSED(cunit);
  sio->port()->writeComplete();
}

bool PCLINK::processParameterBuffer(const uchar ccom, const quint8 cunit, const quint8 parsize) {
  if (ccom != 'P') {
    return true;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  PARBUF pbuf;
  memset(&pbuf, 0, sizeof(PARBUF));

  QByteArray data = sio->port()->readDataFrame(parsize, false);

  device[cunit].status.stat &= ~0x02;

  sio->port()->writeDataAck(); /* ack the received block */

  if (data.length() != parsize) {
    device[cunit].status.stat |= 0x02;
    if (D) qDebug() << "!n" << tr("'P' WRONG DATA FRAME, expected size %1 got %2").arg(parsize).arg(data.length());
    device[cunit].status.err = static_cast<uchar>(PclinkError::BlockCrc);
    completeCommand(cunit);
    return false;
  }
  memcpy(&pbuf, data.constData(), parsize);

  device[cunit].status.stat &= ~0x04;

  if (memcmp(&pbuf, &device[cunit].parbuf, sizeof(PARBUF)) == 0) {
    /* this is a retry of P-block. Most commands don't like that */
    if (pbuf.fno != 0x00 && pbuf.fno != 0x01 && pbuf.fno != 0x03 && pbuf.fno != 0x04 && pbuf.fno != 0x06 &&
        pbuf.fno != 0x11 && pbuf.fno != 0x13) {
      if (D) qDebug() << "!n" << tr("PARBLK retry, ignored");
      completeCommand(cunit);
      return false;
    }
  }

  memcpy(&device[cunit].parbuf, &pbuf, sizeof(PARBUF));
  return true;
}

void PCLINK::do_pclink(uchar devno, uchar ccom, uchar caux1, uchar caux2) {
  Q_UNUSED(devno);
  const quint8 cunit = caux2 & 0x0f;
  const quint8 parsize = caux1 ? caux1 : static_cast<quint8>(256);

  if (caux2 & 0xf0) /* protocol version number must be 0 */
  {
    sio->port()->writeCommandNak();
    return;
  }

  if (parsize > sizeof(PARBUF)) /* and not more than fits in parbuf */
  {
    sio->port()->writeCommandNak();
    return;
  }

  if (!processParameterBuffer(ccom, cunit, parsize)) {
    old_ccom = ccom;
    return;
  }

  const uchar fno = device[cunit].parbuf.fno;
  const ulong faux = device[cunit].parbuf.f1 + device[cunit].parbuf.f2 * 256 +
                     device[cunit].parbuf.f3 * 65536;

  if (fno < PCL_MAX_FNO + 1) {
    if (D) qDebug() << "!n" << tr("%1 (fno $%02)").arg(fun[fno]).arg(fno, 0, 16);
  }

  const uchar handle = device[cunit].parbuf.handle;

  switch (static_cast<PclinkFunction>(fno)) {
    case PclinkFunction::FRead:
      handleFRead(cunit, ccom, faux, handle);
      break;
    case PclinkFunction::FWrite:
      handleFWrite(cunit, ccom, faux, handle);
      break;
    case PclinkFunction::FSeek:
      handleFSeek(cunit, ccom, faux, handle);
      break;
    case PclinkFunction::FTell:
      handleFTell(cunit, ccom, handle);
      break;
    case PclinkFunction::FLen:
      handleFLen(cunit, ccom, handle);
      break;
    case PclinkFunction::FNext:
      handleFNext(cunit, ccom, handle);
      break;
    case PclinkFunction::FClose:
      handleFClose(cunit, ccom, handle);
      break;
    case PclinkFunction::Init:
      handleInit(cunit, ccom);
      break;
    case PclinkFunction::FOpen:
    case PclinkFunction::FFirst:
      handleFOpen(cunit, ccom, faux, handle, fno);
      break;
    case PclinkFunction::Rename:
      handleRename(cunit, ccom);
      break;
    case PclinkFunction::Remove:
      handleRemove(cunit, ccom);
      break;
    case PclinkFunction::Chmod:
      handleChmod(cunit, ccom);
      break;
    case PclinkFunction::Mkdir:
      handleMkdir(cunit, ccom);
      break;
    case PclinkFunction::Rmdir:
      handleRmdir(cunit, ccom);
      break;
    case PclinkFunction::Chdir:
      handleChdir(cunit, ccom);
      break;
    case PclinkFunction::GetCwd:
      handleGetCwd(cunit, ccom);
      break;
    case PclinkFunction::DFree:
      handleDFree(cunit, ccom);
      break;
    case PclinkFunction::ChVol:
      handleChvol(cunit, ccom);
      break;
    default:
      if (D) qDebug() << "!n" << tr("fno $%1 not implemented").arg(fno, 0, 16);
      device[cunit].status.err = static_cast<uchar>(PclinkError::UnsupportedMode);
      completeCommand(cunit);
      break;
  }

  old_ccom = ccom;
}

/*************************************************************************
 * Discrete Command Handlers
 *************************************************************************/

void PCLINK::handleFRead(const quint8 cunit, const uchar ccom, const ulong faux, const uchar handle) {
  const ulong blk_size = faux & 0x0000FFFFL;

  if (ccom == 'P') {
    if (handle > 15 || iodesc[handle].fps.file == nullptr) {
      if (D) qDebug() << "!n" << tr("bad handle 1 %1").arg(handle);
      device[cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
      completeCommand(cunit);
      return;
    }

    if (blk_size == 0) {
      if (D) qDebug() << "!n" << tr("bad size $0000 (0)");
      device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
      // ReSharper disable once CppRedundantCastExpression
      set_status_size(static_cast<quint8>(cunit), 0);
      completeCommand(cunit);
      return;
    }

    device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
    iodesc[handle].eof = 0;

    const ulong buffer = static_cast<size_t>(iodesc[handle].fpstat.st_size) - iodesc[handle].fppos;

    ulong effective_size = blk_size;
    if (buffer < blk_size) {
      effective_size = buffer;
      device[cunit].parbuf.f1 = buffer & 0x00ff;
      device[cunit].parbuf.f2 = (buffer & 0xff00) >> 8;
      iodesc[handle].eof = 1;
      if (effective_size == 0)
        device[cunit].status.err = static_cast<uchar>(PclinkError::Eof);
    }
    if (D) qDebug() << "!n" << tr("size $%1 (%2), buffer $%3 (%4)").arg(effective_size, 0, 16).arg(effective_size).arg(buffer, 0, 16).arg(buffer);
    set_status_size(cunit, static_cast<unsigned short>(effective_size));
    completeCommand(cunit);
    return;
  }

  if (ccom == 'R' && old_ccom == 'R') {
    sio->port()->writeCommandNak();
    if (D) qDebug() << "!n" << tr("serial communication error, abort");
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  if (D) qDebug() << "!n" << tr("handle %1").arg(handle);

  auto mem = static_cast<unsigned char*>(malloc(blk_size));

  if (device[cunit].status.err == static_cast<uchar>(PclinkError::Success)) {
    iodesc[handle].fpread = blk_size;

    if (iodesc[handle].fpmode & 0x10) {
      int eof_sig;
      const ulong rdata = dir_read(mem, blk_size, handle, &eof_sig);

      if (rdata != blk_size) {
        if (D) qDebug() << "!n" << tr("FREAD: cannot read %1 bytes from dir").arg(blk_size);
        if (eof_sig) {
          iodesc[handle].fpread = rdata;
          device[cunit].status.err = static_cast<uchar>(PclinkError::Eof);
        } else {
          iodesc[handle].fpread = 0;
          device[cunit].status.err = static_cast<uchar>(PclinkError::General);
        }
      }
    } else {
      if (fseek(iodesc[handle].fps.file, static_cast<long>(iodesc[handle].fppos), SEEK_SET)) {
        if (D) qDebug() << "!n" << tr("FREAD: cannot seek to $%1 (%2)").arg(iodesc[handle].fppos, 0, 16).arg(iodesc[handle].fppos);
        device[cunit].status.err = static_cast<uchar>(PclinkError::SeekError);
      } else {
        if (const size_t fdata = fread(mem, sizeof(char), blk_size, iodesc[handle].fps.file); fdata != blk_size) {
          if (D) qDebug() << "!n" << tr("FREAD: cannot read %1 bytes from file").arg(blk_size);
          if (feof(iodesc[handle].fps.file)) {
            iodesc[handle].fpread = fdata;
            device[cunit].status.err = static_cast<uchar>(PclinkError::Eof);
          } else {
            iodesc[handle].fpread = 0;
            device[cunit].status.err = static_cast<uchar>(PclinkError::General);
          }
        }
      }
    }
  }

  iodesc[handle].fppos += iodesc[handle].fpread;

  if (device[cunit].status.err == static_cast<uchar>(PclinkError::Success)) {
    if (iodesc[handle].eof)
      device[cunit].status.err = static_cast<uchar>(PclinkError::Eof);
    else if (iodesc[handle].fppos == static_cast<size_t>(iodesc[handle].fpstat.st_size))
      device[cunit].status.err = 3;
  }

  set_status_size(cunit, static_cast<unsigned short>(iodesc[handle].fpread));

  if (D) qDebug() << "!n" << tr("FREAD: send $%1 (%2), status $%3").arg(blk_size, 0, 16).arg(blk_size).arg(device[cunit].status.err);

  QByteArray data(reinterpret_cast<const char*>(mem), static_cast<int>(blk_size));
  sio->port()->writeComplete();
  sio->port()->writeDataFrame(data);

  free(mem);
}

void PCLINK::handleFWrite(const quint8 cunit, const uchar ccom, const ulong faux, const uchar handle) {
  const ulong blk_size = faux & 0x0000FFFFL;

  if (ccom == 'P') {
    if (handle > 15 || iodesc[handle].fps.file == nullptr) {
      if (D) qDebug() << "!n" << tr("bad handle 2 %1").arg(handle);
      device[cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
      completeCommand(cunit);
      return;
    }

    if (blk_size == 0) {
      if (D) qDebug() << "!n" << tr("bad size $0000 (0)");
      device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
      set_status_size(cunit, 0);
      completeCommand(cunit);
      return;
    }

    device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

    if (D) qDebug() << "!n" << tr("size $%1 (%2)").arg(blk_size, 0, 16).arg(blk_size);
    set_status_size(cunit, static_cast<unsigned short>(blk_size));
    completeCommand(cunit);
    return;
  }

  if (ccom == 'R' && old_ccom == 'R') {
    sio->port()->writeCommandNak();
    if (D) qDebug() << "!n" << tr("serial communication error, abort");
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  if (D) qDebug() << "!n" << tr("handle %1").arg(handle);

  if ((iodesc[handle].fpmode & 0x10) == 0) {
    if (fseek(iodesc[handle].fps.file, static_cast<long>(iodesc[handle].fppos), SEEK_SET)) {
      if (D) qDebug() << "!n" << tr("FWRITE: cannot seek to $%1 (%2)").arg(iodesc[handle].fppos, 0, 16).arg(iodesc[handle].fppos);
      device[cunit].status.err = static_cast<uchar>(PclinkError::SeekError);
    }
  }

  auto mem = static_cast<unsigned char*>(malloc(blk_size));

  QByteArray data = sio->port()->readDataFrame(static_cast<uint>(blk_size), false);

  sio->port()->writeDataAck(); /* ack the block of data */

  if (static_cast<unsigned long>(data.length()) != blk_size) {
    if (D) qDebug() << "!n" << tr("FWRITE: block CRC mismatch");
    device[cunit].status.err = static_cast<uchar>(PclinkError::BlockCrc);
    free(mem);
    completeCommand(cunit);
    return;
  }
  memcpy(mem, data.constData(), blk_size);

  if (device[cunit].status.err == static_cast<uchar>(PclinkError::Success)) {
    iodesc[handle].fpread = blk_size;

    if (iodesc[handle].fpmode & 0x10) {
      /* ignore raw dir writes */
    } else {
      if (const size_t rdata = fwrite(mem, sizeof(char), blk_size, iodesc[handle].fps.file); static_cast<unsigned long>(rdata) != blk_size) {
        if (D) qDebug() << "!n" << tr("FWRITE: cannot write %1 bytes to file").arg(blk_size);
        iodesc[handle].fpread = rdata;
        device[cunit].status.err = static_cast<uchar>(PclinkError::General);
      }
    }
  }

  iodesc[handle].fppos += iodesc[handle].fpread;

  set_status_size(cunit, static_cast<unsigned short>(iodesc[handle].fpread));

  if (D) qDebug() << "!n" << tr("FWRITE: received $%1 (%2), status $%3").arg(blk_size, 0, 16).arg(blk_size).arg(device[cunit].status.err, 0, 16);

  free(mem);
  completeCommand(cunit);
}

void PCLINK::handleFSeek(const quint8 cunit, const uchar ccom, const ulong faux, const uchar handle) {
  const ulong newpos = faux;

  if (handle > 15 || iodesc[handle].fps.file == nullptr) {
    if (D) qDebug() << "!n" << tr("bad handle 3 %1").arg(handle);
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
    completeCommand(cunit);
    return;
  }

  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    if (D) qDebug() << "!n" << tr("bad exec");
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    completeCommand(cunit);
    return;
  }

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

  if (D) qDebug() << "!n" << tr("handle %1, newpos $%2 (%3)").arg(handle).arg(newpos, 0, 16).arg(newpos);

  if (iodesc[handle].fpmode & 0x08)
    iodesc[handle].fppos = newpos;
  else {
    if (newpos <= static_cast<unsigned long>(iodesc[handle].fpstat.st_size))
      iodesc[handle].fppos = newpos;
    else
      device[cunit].status.err = static_cast<uchar>(PclinkError::SeekError);
  }

  completeCommand(cunit);
}

void PCLINK::handleFTell(const quint8 cunit, const uchar ccom, const uchar handle) {
  if (ccom == 'P') {
    if (handle > 15 || iodesc[handle].fps.file == nullptr) {
      if (D) qDebug() << "!n" << tr("bad handle 4 %1").arg(handle);
      device[cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
      completeCommand(cunit);
      return;
    }

    device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
    if (D) qDebug() << "!n" << tr("device $%1").arg(cunit, 0, 16);
    completeCommand(cunit);
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  const ulong outval = iodesc[handle].fppos;
  if (D) qDebug() << "!n" << tr("handle %1, send $%2 (%3)").arg(handle).arg(outval, 0, 16).arg(outval);

  uchar out[3];
  out[0] = static_cast<unsigned char>(outval & 0x000000ffL);
  out[1] = static_cast<unsigned char>((outval & 0x0000ff00L) >> 8);
  out[2] = static_cast<unsigned char>((outval & 0x00ff0000L) >> 16);

  QByteArray data(reinterpret_cast<const char*>(out), sizeof(out));
  sio->port()->writeComplete();
  sio->port()->writeDataFrame(data);
}

void PCLINK::handleFLen(const quint8 cunit, const uchar ccom, const uchar handle) {
  if (ccom == 'P') {
    if (handle > 15 || iodesc[handle].fps.file == nullptr) {
      if (D) qDebug() << "!n" << tr("bad handle 4 %1").arg(handle);
      device[cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
      completeCommand(cunit);
      return;
    }

    device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
    if (D) qDebug() << "!n" << tr("device $%1").arg(cunit, 0, 16);
    completeCommand(cunit);
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  const auto outval = static_cast<unsigned long>(iodesc[handle].fpstat.st_size);
  if (D) qDebug() << "!n" << tr("handle %1, send $%2 (%3)").arg(handle).arg(outval, 0, 16).arg(outval);

  uchar out[3];
  out[0] = static_cast<unsigned char>(outval & 0x000000ffL);
  out[1] = static_cast<unsigned char>((outval & 0x0000ff00L) >> 8);
  out[2] = static_cast<unsigned char>((outval & 0x00ff0000L) >> 16);

  QByteArray data(reinterpret_cast<const char*>(out), sizeof(out));
  sio->port()->writeComplete();
  sio->port()->writeDataFrame(data);
}

bool PCLINK::scanNextDirectoryEntry(const quint8 cunit, const uchar handle) {
  int eof_flg, match = 0;

  if (D) qDebug() << "!n" << tr("handle %1").arg(handle);

  do {
    struct stat ts{};
    memset(&ts, 0, sizeof(ts));
    memset(pcl_dbf.dirbuf, 0, sizeof(pcl_dbf.dirbuf));
    iodesc[handle].fppos += dir_read(pcl_dbf.dirbuf, sizeof(pcl_dbf.dirbuf), handle, &eof_flg);

    if (D) qDebug() << "!n" << tr("eof_flg %1").arg(eof_flg);

    if (!eof_flg) {
      /* fake stat to satisfy match_dos_names() */
      if ((pcl_dbf.dirbuf[0] & 0x01) == 0)
        ts.st_mode |= S_IWUSR;
      if (pcl_dbf.dirbuf[0] & 0x20)
        ts.st_mode |= S_IFDIR;
      else
        ts.st_mode |= S_IFREG;

      match = !match_dos_names(reinterpret_cast<char*>(pcl_dbf.dirbuf) + 6, iodesc[handle].fpname, iodesc[handle].fatr1, &ts);
    }
  } while (!eof_flg && !match);

  if (eof_flg) {
    if (D) qDebug() << "!n" << tr("FNEXT: EOF");
    device[cunit].status.err = static_cast<uchar>(PclinkError::Eof);
  } else if (iodesc[handle].fppos == static_cast<size_t>(iodesc[handle].fpstat.st_size)) {
    device[cunit].status.err = 3;
  }
  return match != 0;
}

void PCLINK::handleFNext(const quint8 cunit, const uchar ccom, const uchar handle) {
  if (ccom == 'P') {
    device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
    if (D) qDebug() << "!n" << tr("device $%1").arg(cunit, 0, 16);
    completeCommand(cunit);
    return;
  }

  if (ccom == 'R' && old_ccom == 'R') {
    sio->port()->writeCommandNak();
    if (D) qDebug() << "!n" << tr("serial communication error, abort");
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  memset(pcl_dbf.dirbuf, 0, sizeof(pcl_dbf.dirbuf));

  if (handle > 15 || iodesc[handle].fps.file == nullptr) {
    if (D) qDebug() << "!n" << tr("bad handle 5 %1").arg(handle);
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
  } else {
    scanNextDirectoryEntry(cunit, handle);
  }

  /* avoid the 4th execution stage */
  pcl_dbf.handle = device[cunit].status.err;

  if (D) qDebug() << "!n" << tr("FNEXT: status %1, send $%2 $%3%4 $%5%6%7 %8%9%10%11%12%13%14%15%16%17%18 %19-%20-%21 %22:%23:%24").arg(pcl_dbf.handle).arg(pcl_dbf.dirbuf[0], 0, 16).arg(pcl_dbf.dirbuf[2], 0, 16).arg(pcl_dbf.dirbuf[1], 0, 16).arg(pcl_dbf.dirbuf[5]).arg(pcl_dbf.dirbuf[4]).arg(pcl_dbf.dirbuf[3]).arg(pcl_dbf.dirbuf[6]).arg(pcl_dbf.dirbuf[7]).arg(pcl_dbf.dirbuf[8]).arg(pcl_dbf.dirbuf[9]).arg(pcl_dbf.dirbuf[10]).arg(pcl_dbf.dirbuf[11]).arg(pcl_dbf.dirbuf[12]).arg(pcl_dbf.dirbuf[13]).arg(pcl_dbf.dirbuf[14]).arg(pcl_dbf.dirbuf[15]).arg(pcl_dbf.dirbuf[16]).arg(pcl_dbf.dirbuf[17]).arg(pcl_dbf.dirbuf[18]).arg(pcl_dbf.dirbuf[19]).arg(pcl_dbf.dirbuf[20]).arg(pcl_dbf.dirbuf[21]).arg(pcl_dbf.dirbuf[22]);

  QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
  sio->port()->writeComplete();
  sio->port()->writeDataFrame(data);
}

void PCLINK::handleFClose(const quint8 cunit, const uchar ccom, const uchar handle) {
  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  if (handle > 15 || iodesc[handle].fps.file == nullptr) {
    if (D) qDebug() << "!n" << tr("bad handle 6 %1").arg(handle);
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
    completeCommand(cunit);
    return;
  }

  if (D) qDebug() << "!n" << tr("handle %1").arg(handle);

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

  const uchar fpmode = iodesc[handle].fpmode;
  const time_t mtime = iodesc[handle].fpstat.st_mtime;

  char pathname[1024];
  strcpy(pathname, iodesc[handle].pathname);

  fps_close(handle); /* this clears out iodesc[handle] */

  if (mtime && fpmode & 0x08) {
    utimbuf ub{};
    ub.actime = mtime;
    ub.modtime = mtime;
    utime(pathname, &ub);
  }
  completeCommand(cunit);
}

void PCLINK::handleInit(const quint8 cunit, const uchar ccom) {
  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  do_pclink_init(0);

  device[cunit].parbuf.handle = 0xff;
  device[cunit].status.none = PCLINK_CDEVIC;
  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
  completeCommand(cunit);
}

bool PCLINK::validateOpenParameters(const quint8 cunit, const uchar fmode, const char *newpath) {
  if ((fmode & 0x0c) == 0 || (fmode & 0x18) == 0x18) {
    if (D) qDebug() << "!n" << tr("unsupported fmode ($%1)").arg(fmode);
    device[cunit].status.err = static_cast<uchar>(PclinkError::UnsupportedMode);
    return false;
  }

  if (!validate_user_path(device[cunit].dirname, newpath)) {
    if (D) qDebug() << "!n" << tr("invalid path 1 '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    return false;
  }
  return true;
}

int PCLINK::findFreeDescriptor() {
  for (int i = 0; i < 16; i++) {
    if (iodesc[i].fps.file == nullptr)
      return i;
  }
  return -1;
}

bool PCLINK::openDirectoryStream(const quint8 cunit, const int handle, const char *newpath, const char *fname_expanded) {
  Q_UNUSED(cunit);
  Q_UNUSED(fname_expanded);
  DIR *dh = opendir(newpath);
  if (!dh)
    return false;
  iodesc[handle].fps.dir = dh;
  return true;
}

bool PCLINK::openRegularFileStream(const quint8 cunit, const int handle, const uchar fmode, const char *newpath, const char *fname_expanded, bool is_first, struct stat *outStat) {
  Q_UNUSED(cunit);
  Q_UNUSED(fname_expanded);
  Q_UNUSED(is_first);
  if ((fmode & 0x0d) == 0x04)
    iodesc[handle].fps.file = fopen(newpath, "rb");
  else if ((fmode & 0x0d) == 0x08) {
    iodesc[handle].fps.file = fopen(newpath, "wb");
    if (iodesc[handle].fps.file)
      outStat->st_size = 0;
  } else if ((fmode & 0x0d) == 0x09) {
    iodesc[handle].fps.file = fopen(newpath, "rb+");
    if (iodesc[handle].fps.file)
      fseek(iodesc[handle].fps.file, outStat->st_size, SEEK_SET);
  } else if ((fmode & 0x0d) == 0x0c)
    iodesc[handle].fps.file = fopen(newpath, "rb+");

  return iodesc[handle].fps.file != nullptr;
}

void PCLINK::populateOpenDescriptor(const int handle, const quint8 cunit, const uchar fmode, const uchar fatr1, const uchar fatr2, const struct stat *sb, const char *fname_expanded, const char *newpath) {
  iodesc[handle].devno = PCLINK_CDEVIC;
  iodesc[handle].cunit = cunit;
  iodesc[handle].fpmode = fmode;
  iodesc[handle].fatr1 = fatr1;
  iodesc[handle].fatr2 = fatr2;
  iodesc[handle].t1 = device[cunit].parbuf.f1;
  iodesc[handle].t2 = device[cunit].parbuf.f2;
  iodesc[handle].t3 = device[cunit].parbuf.f3;
  iodesc[handle].d1 = device[cunit].parbuf.f4;
  iodesc[handle].d2 = device[cunit].parbuf.f5;
  iodesc[handle].d3 = device[cunit].parbuf.f6;
  iodesc[handle].fppos = 0L;
  strcpy(iodesc[handle].pathname, newpath);
  memcpy(&iodesc[handle].fpstat, sb, sizeof(struct stat));

  if (iodesc[handle].fpmode & 0x10)
    memcpy(iodesc[handle].fpname, device[cunit].parbuf.name, sizeof(iodesc[handle].fpname));
  else
    memcpy(iodesc[handle].fpname, fname_expanded, sizeof(iodesc[handle].fpname));

  iodesc[handle].fpstat.st_size = static_cast<off_t>(get_file_len(static_cast<uchar>(handle)));

  if ((iodesc[handle].fpmode & 0x1d) == 0x09)
    iodesc[handle].fppos = static_cast<size_t>(iodesc[handle].fpstat.st_size);
}

void PCLINK::buildOpenResponseFrame(const int handle, const struct stat *sb, const char *fname_expanded) {
  Q_UNUSED(sb);
  Q_UNUSED(fname_expanded);
  memset(pcl_dbf.dirbuf, 0, sizeof(pcl_dbf.dirbuf));

  if (handle > 15 || iodesc[handle].fps.file == nullptr) {
    if (D) qDebug() << "!n" << tr("FOPEN: bad handle 7 %1").arg(handle);
    device[iodesc[handle].cunit].status.err = static_cast<uchar>(PclinkError::BadHandle); /* bad file handle */
    pcl_dbf.handle = static_cast<uchar>(PclinkError::BadHandle);
  } else {
    pcl_dbf.handle = static_cast<uchar>(handle);

    uchar ob[7];
    unix_time_2_sdx(&iodesc[handle].fpstat.st_mtime, ob);

    if (D) qDebug() << "!n" << tr("FOPEN: %1 handle %2").arg(iodesc[handle].fpmode & 0x08 ? "write" : "read").arg(handle);

    memset(pcl_dbf.dirbuf, 0, sizeof(pcl_dbf.dirbuf));

    if (iodesc[handle].fpmode & 0x10) {
      int eof_sig;

      iodesc[handle].dir_cache = cache_dir(static_cast<uchar>(handle));
      iodesc[handle].fppos += dir_read(pcl_dbf.dirbuf, sizeof(pcl_dbf.dirbuf), static_cast<uchar>(handle), &eof_sig);

      if (eof_sig) {
        if (D) qDebug() << "!n" << tr("FOPEN: dir EOF?");
        device[iodesc[handle].cunit].status.err = static_cast<uchar>(PclinkError::Eof);
      } else if (iodesc[handle].fppos == static_cast<size_t>(iodesc[handle].fpstat.st_size)) {
        device[iodesc[handle].cunit].status.err = 3;
      }
    } else {
      auto dlen = iodesc[handle].fpstat.st_size;

      memset(pcl_dbf.dirbuf + 6, 0x20, 11);
      pcl_dbf.dirbuf[3] = static_cast<unsigned char>(dlen & 0x000000ffL);
      pcl_dbf.dirbuf[4] = static_cast<unsigned char>((dlen & 0x0000ff00L) >> 8);
      pcl_dbf.dirbuf[5] = static_cast<unsigned char>((dlen & 0x00ff0000L) >> 16);
      memcpy(pcl_dbf.dirbuf + 17, ob, 6);

      pcl_dbf.dirbuf[0] = 0x08;

      if ((iodesc[handle].fpstat.st_mode & S_IWUSR) == 0)
        pcl_dbf.dirbuf[0] |= 0x01; /* protected */
      if (S_ISDIR(iodesc[handle].fpstat.st_mode))
        pcl_dbf.dirbuf[0] |= 0x20; /* directory */

      int x = 0;
      while (iodesc[handle].fpname[x] && x < 11) {
        pcl_dbf.dirbuf[6 + x] = static_cast<quint8>(iodesc[handle].fpname[x]);
        x++;
      }
    }

    if (D) qDebug() << "!n" << tr("FOPEN: send %1, send $%2 $%3%4 $%5%6%7 %8%9%10%11%12%13%14%15%16%17%18 %19-%20-%21 %22:%23:%24").arg(pcl_dbf.handle).arg(pcl_dbf.dirbuf[0], 0, 16).arg(pcl_dbf.dirbuf[2], 0, 16).arg(pcl_dbf.dirbuf[1], 0, 16).arg(pcl_dbf.dirbuf[5]).arg(pcl_dbf.dirbuf[4]).arg(pcl_dbf.dirbuf[3]).arg(pcl_dbf.dirbuf[6]).arg(pcl_dbf.dirbuf[7]).arg(pcl_dbf.dirbuf[8]).arg(pcl_dbf.dirbuf[9]).arg(pcl_dbf.dirbuf[10]).arg(pcl_dbf.dirbuf[11]).arg(pcl_dbf.dirbuf[12]).arg(pcl_dbf.dirbuf[13]).arg(pcl_dbf.dirbuf[14]).arg(pcl_dbf.dirbuf[15]).arg(pcl_dbf.dirbuf[16]).arg(pcl_dbf.dirbuf[17]).arg(pcl_dbf.dirbuf[18]).arg(pcl_dbf.dirbuf[19]).arg(pcl_dbf.dirbuf[20]).arg(pcl_dbf.dirbuf[21]).arg(pcl_dbf.dirbuf[22]);
  }
}

void PCLINK::handleFOpen(const quint8 cunit, const uchar ccom, const ulong faux, const uchar handle, const uchar fno) {
  Q_UNUSED(faux);
  Q_UNUSED(handle);
  if (ccom == 'P') {
    if (D) qDebug() << "!n" << tr("mode: $%1, atr1: $%2, atr2: $%3, path: '%4', name: '%5'").arg(device[cunit].parbuf.fmode, 0, 16).arg(device[cunit].parbuf.fatr1, 0, 16).arg(device[cunit].parbuf.fatr2, 0, 16).arg(QString(reinterpret_cast<const char*>(device[cunit].parbuf.path)), QString(reinterpret_cast<const char*>(device[cunit].parbuf.name)));

    device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

    if (fno == 0x0a)
      device[cunit].parbuf.fmode |= 0x10;
    completeCommand(cunit);
    return;
  }

  if (ccom == 'R' && old_ccom == 'R') {
    sio->port()->writeCommandNak();
    if (D) qDebug() << "!n" << tr("serial communication error, abort");
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  char raw_name[12];
  memset(raw_name, 0, sizeof(raw_name));
  memcpy(raw_name, device[cunit].parbuf.name, 8 + 3);

  char newpath[1024];
  create_user_path(cunit, newpath);

  if (!validateOpenParameters(cunit, device[cunit].parbuf.fmode, newpath)) {
    QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
    sio->port()->writeComplete();
    sio->port()->writeDataFrame(data);
    return;
  }

  if (D) qDebug() << "!n" << tr("local path '%1'").arg(newpath);

  const int freeSlot = findFreeDescriptor();
  if (freeSlot < 0) {
    if (D) qDebug() << "!n" << tr("FOPEN: too many channels open");
    device[cunit].status.err = 161;
    QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
    sio->port()->writeComplete();
    sio->port()->writeDataFrame(data);
    return;
  }

  struct stat tempstat{};
  if (stat(newpath, &tempstat) < 0) {
    if (D) qDebug() << "!n" << tr("FOPEN: cannot stat '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
    sio->port()->writeComplete();
    sio->port()->writeDataFrame(data);
    return;
  }

  struct stat sb{};
  DIR *dh = opendir(newpath);
  if (device[cunit].parbuf.fmode & 0x10) {
    iodesc[freeSlot].fps.dir = dh;
    memcpy(&sb, &tempstat, sizeof(sb));
  } else {
    if (D) qDebug() << "!n" << tr(" ! fmode & 0x10");
    dirent *dp = nullptr;
    while ((dp = readdir(dh)) != nullptr) {
      if (check_dos_name(newpath, dp, &sb))
        continue;

      /* convert 8+3 to NNNNNNNNXXX */
      ugefina(dp->d_name, raw_name);

      /* match */
      if (match_dos_names(raw_name,
                          reinterpret_cast<char*>(device[cunit].parbuf.name),
                          device[cunit].parbuf.fatr1, &sb) == 0)
        break;
    }

    const size_t sl = strlen(newpath);
    if (sl && newpath[sl - 1] != HOST_SEPARATOR_CHAR) {
      newpath[sl] = HOST_SEPARATOR_CHAR;
      newpath[sl + 1] = 0;
    }

    if (dp) {
      if (is_fname_reserved(dp->d_name)) {
        strcat(newpath, RESERVED_NAME_PREFIX_STR);
      }
      strcat(newpath, dp->d_name);
      /* convert 8+3 to NNNNNNNNXXX */
      ugefina(dp->d_name, raw_name);
      if ((device[cunit].parbuf.fmode & 0x0c) == 0x08)
        sb.st_mtime = timestamp2mtime(&device[cunit].parbuf.f1);
    } else {
      if ((device[cunit].parbuf.fmode & 0x0c) == 0x04) {
        if (D) qDebug() << "!n" << tr("FOPEN: file not found");
        device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
        closedir(dh);
        QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
        sio->port()->writeComplete();
        sio->port()->writeDataFrame(data);
        return;
      }
      char name83[12];

      if (D) qDebug() << "!n" << tr("FOPEN: creating file");

      /* convert NNNNNNNNXXX to 8+3 */
      uexpand(device[cunit].parbuf.name, name83);

      if (validate_dos_name(name83)) {
        if (D) qDebug() << "!n" << tr("FOPEN: bad filename '%1'").arg(name83);
        device[cunit].status.err = static_cast<uchar>(PclinkError::BadFilename); /* bad filename */
        closedir(dh);
        QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
        sio->port()->writeComplete();
        sio->port()->writeDataFrame(data);
        return;
      }

      if (is_fname_reserved(name83)) {
        strcat(newpath, RESERVED_NAME_PREFIX_STR);
      }
      strcat(newpath, name83);
      /* convert 8+3 to NNNNNNNNXXX */
      ugefina(name83, raw_name);

      memset(&sb, 0, sizeof(struct stat));
      sb.st_mode = S_IFREG | S_IRUSR | S_IWUSR;
      sb.st_mtime = timestamp2mtime(&device[cunit].parbuf.f1);
    }

    if (D) qDebug() << "!n" << tr("FOPEN: full local path '%1'").arg(newpath);

    if (stat(newpath, &tempstat) < 0) {
      if ((device[cunit].parbuf.fmode & 0x0c) == 0x04) {
        if (D) qDebug() << "!n" << tr("FOPEN: cannot stat '%1'").arg(newpath);
        device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
        closedir(dh);
        QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
        sio->port()->writeComplete();
        sio->port()->writeDataFrame(data);
        return;
      }
    } else {
      if (device[cunit].parbuf.fmode & 0x08) {
        if ((tempstat.st_mode & S_IWUSR) == 0) {
          if (D) qDebug() << "!n" << tr("FOPEN: '%1' is read-only").arg(newpath);
          device[cunit].status.err = static_cast<uchar>(PclinkError::ReadOnly);
          closedir(dh);
          QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
          sio->port()->writeComplete();
          sio->port()->writeDataFrame(data);
          return;
        }
      }
    }

    if (!openRegularFileStream(cunit, freeSlot, device[cunit].parbuf.fmode, newpath, raw_name, dp != nullptr, &sb)) {
      // fopen handled
    }

    closedir(dh);
  }

  if (iodesc[freeSlot].fps.file == nullptr) {
    if (D) qDebug() << "!n" << tr("FOPEN: cannot open '%1', %2 (%3)").arg(newpath, strerror(errno), QString::number(errno));
    if (device[cunit].parbuf.fmode & 0x04)
      device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
    else
      device[cunit].status.err = static_cast<uchar>(PclinkError::ReadOnly);

    QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
    sio->port()->writeComplete();
    sio->port()->writeDataFrame(data);
    return;
  }

  device[cunit].parbuf.handle = static_cast<uchar>(freeSlot);

  populateOpenDescriptor(freeSlot, cunit, device[cunit].parbuf.fmode, device[cunit].parbuf.fatr1, device[cunit].parbuf.fatr2, &sb, raw_name, newpath);

  buildOpenResponseFrame(freeSlot, &sb, raw_name);

  QByteArray data(reinterpret_cast<const char*>(&pcl_dbf), sizeof(pcl_dbf));
  sio->port()->writeComplete();
  sio->port()->writeDataFrame(data);
}

void PCLINK::handleRename(const quint8 cunit, const uchar ccom) {
  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  char newpath[1024];
  create_user_path(cunit, newpath);

  if (!validate_user_path(device[cunit].dirname, newpath)) {
    if (D) qDebug() << "!n" << tr("invalid path 2 '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    completeCommand(cunit);
    return;
  }

  DIR *renamedir = opendir(newpath);
  if (renamedir == nullptr) {
    if (D) qDebug() << "!n" << tr("cannot open dir '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::General);
    completeCommand(cunit);
    return;
  }

  if (D) qDebug() << "!n" << tr("local path '%1', fatr1 $%2").arg(newpath).arg(device[cunit].parbuf.fatr1 | RA_NO_PROTECT, 0, 16);

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
  ulong fcnt = 0;
  dirent *dp = nullptr;
  struct stat sb{};

  while ((dp = readdir(renamedir)) != nullptr) {
    char raw_name[12];

    if (check_dos_name(newpath, dp, &sb))
      continue;

    /* convert 8+3 to NNNNNNNNXXX */
    ugefina(dp->d_name, raw_name);

    /* match */
    if (match_dos_names(raw_name, reinterpret_cast<char*>(device[cunit].parbuf.name),
                        device[cunit].parbuf.fatr1 | RA_NO_PROTECT, &sb) == 0) {
      char xpath[1024], xpath2[1024], newname[16];
      uchar names[12];
      struct stat dummy{};

      fcnt++;

      strcpy(xpath, newpath);
      strcat(xpath, HOST_SEPARATOR_STR);
      if (is_fname_reserved(dp->d_name)) {
        strcat(xpath, RESERVED_NAME_PREFIX_STR);
      }
      strcat(xpath, dp->d_name);

      memcpy(names, device[cunit].parbuf.names, 12);

      for (ushort x = 0; x < 12; x++) {
        if (names[x] == '?')
          names[x] = static_cast<quint8>(raw_name[x]);
      }

      /* convert NNNNNNNNXXX to 8+3 */
      uexpand(names, newname);

      strcpy(xpath2, newpath);
      strcat(xpath2, HOST_SEPARATOR_STR);
      if (is_fname_reserved(newname)) {
        strcat(xpath2, RESERVED_NAME_PREFIX_STR);
      }
      strcat(xpath2, newname);

      if (D) qDebug() << "!n" << tr("RENAME: renaming '%1' -> '%2'").arg(dp->d_name, newname);

      if (stat(xpath2, &dummy) == 0) {
        if (D) qDebug() << "!n" << tr("RENAME: '%1' already exists").arg(xpath2);
        device[cunit].status.err = static_cast<uchar>(PclinkError::ReadOnly);
        break;
      }

      if (rename(xpath, xpath2)) {
        if (D) qDebug() << "!n" << tr("RENAME: %1").arg(strerror(errno));
        device[cunit].status.err = static_cast<uchar>(PclinkError::General);
      }
    }
  }

  closedir(renamedir);

  if (fcnt == 0 && device[cunit].status.err == static_cast<uchar>(PclinkError::Success))
    device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
  completeCommand(cunit);
}

void PCLINK::handleRemove(const quint8 cunit, const uchar ccom) {
  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  char newpath[1024];
  create_user_path(cunit, newpath);

  if (!validate_user_path(device[cunit].dirname, newpath)) {
    if (D) qDebug() << "!n" << tr("invalid path 3 '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    completeCommand(cunit);
    return;
  }

  if (D) qDebug() << "!n" << tr("local path '%1'").arg(newpath);

  DIR *deldir = opendir(newpath);
  if (deldir == nullptr) {
    if (D) qDebug() << "!n" << tr("cannot open dir '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::General);
    completeCommand(cunit);
    return;
  }

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
  ulong delcnt = 0;
  dirent *dp = nullptr;
  struct stat sb{};

  while ((dp = readdir(deldir)) != nullptr) {
    char raw_name[12];

    if (check_dos_name(newpath, dp, &sb))
      continue;

    /* convert 8+3 to NNNNNNNNXXX */
    ugefina(dp->d_name, raw_name);

    /* match */
    if (match_dos_names(raw_name, reinterpret_cast<char*>(device[cunit].parbuf.name),
                        RA_NO_PROTECT | RA_NO_SUBDIR | RA_NO_HIDDEN, &sb) == 0) {
      char xpath[1024];

      strcpy(xpath, newpath);
      strcat(xpath, HOST_SEPARATOR_STR);
      if (is_fname_reserved(dp->d_name)) {
        strcat(xpath, RESERVED_NAME_PREFIX_STR);
      }
      strcat(xpath, dp->d_name);

      if (!S_ISDIR(sb.st_mode)) {
        if (D) qDebug() << "!n" << tr("REMOVE: delete '%1'").arg(xpath);
        if (unlink(xpath)) {
          if (D) qDebug() << "!n" << tr("REMOVE: cannot delete '%1'").arg(xpath);
          device[cunit].status.err = static_cast<uchar>(PclinkError::General);
        }
        delcnt++;
      }
    }
  }
  closedir(deldir);
  if (delcnt == 0)
    device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
  completeCommand(cunit);
}

void PCLINK::handleChmod(const quint8 cunit, const uchar ccom) {
  const uchar fatr2 = device[cunit].parbuf.fatr2;

  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  if (fatr2 & (SA_SUBDIR | SA_UNSUBDIR)) {
    if (D) qDebug() << "!n" << tr("illegal fatr2 $%1").arg(fatr2);
    device[cunit].status.err = static_cast<uchar>(PclinkError::UnsupportedMode);
    completeCommand(cunit);
    return;
  }

  char newpath[1024];
  create_user_path(cunit, newpath);

  if (!validate_user_path(device[cunit].dirname, newpath)) {
    if (D) qDebug() << "!n" << tr("invalid path 4 '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    completeCommand(cunit);
    return;
  }

  if (D) qDebug() << "!n" << tr("local path '%1', fatr1 $%2 fatr2 $%3").arg(newpath).arg(device[cunit].parbuf.fatr1, 0, 16).arg(fatr2);

  DIR *chmdir = opendir(newpath);
  if (chmdir == nullptr) {
    if (D) qDebug() << "!n" << tr("CHMOD: cannot open dir '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::General);
    completeCommand(cunit);
    return;
  }

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
  ulong fcnt = 0;
  dirent *dp = nullptr;
  struct stat sb{};

  while ((dp = readdir(chmdir)) != nullptr) {
    char raw_name[12];

    if (check_dos_name(newpath, dp, &sb))
      continue;

    /* convert 8+3 to NNNNNNNNXXX */
    ugefina(dp->d_name, raw_name);

    /* match */
    if (match_dos_names(raw_name, reinterpret_cast<char*>(device[cunit].parbuf.name),
                        device[cunit].parbuf.fatr1, &sb) == 0) {
      char xpath[1024];
      mode_t newmode = sb.st_mode;

      strcpy(xpath, newpath);
      strcat(xpath, HOST_SEPARATOR_STR);
      if (is_fname_reserved(dp->d_name)) {
        strcat(xpath, RESERVED_NAME_PREFIX_STR);
      }
      strcat(xpath, dp->d_name);
      if (D) qDebug() << "!n" << tr("CHMOD: change atrs in '%1'").arg(xpath);

      /* On Unix, ignore Hidden and Archive bits */
      if (fatr2 & SA_UNPROTECT)
        newmode |= S_IWUSR;
      if (fatr2 & SA_PROTECT)
        newmode &= ~S_IWUSR;
      if (chmod(xpath, newmode)) {
        if (D) qDebug() << "!n" << tr("CHMOD: failed on '%1'").arg(xpath);
        device[cunit].status.err |= static_cast<uchar>(PclinkError::General);
      }
      fcnt++;
    }
  }
  closedir(chmdir);
  if (fcnt == 0)
    device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
  completeCommand(cunit);
}

void PCLINK::handleMkdir(const quint8 cunit, const uchar ccom) {
  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  char newpath[1024];
  create_user_path(cunit, newpath);

  if (!validate_user_path(device[cunit].dirname, newpath)) {
    if (D) qDebug() << "!n" << tr("invalid path 5 '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    completeCommand(cunit);
    return;
  }

  char fname[12];
  /* convert NNNNNNNNXXX to 8+3 */
  uexpand(device[cunit].parbuf.name, fname);

  if (validate_dos_name(fname)) {
    if (D) qDebug() << "!n" << tr("bad dir name '%1'").arg(fname);
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadFilename);
    completeCommand(cunit);
    return;
  }

  strcat(newpath, HOST_SEPARATOR_STR);
  if (is_fname_reserved(fname)) {
    strcat(newpath, RESERVED_NAME_PREFIX_STR);
  }
  strcat(newpath, fname);

  uchar dt[6];
  memcpy(dt, &device[cunit].parbuf.f1, sizeof(dt));

  if (D) qDebug() << "!n" << tr("making dir '%1', time %2-%3-%4 %5:%6:%7").arg(newpath).arg(dt[0]).arg(dt[1]).arg(dt[2]).arg(dt[3]).arg(dt[4]).arg(dt[5]);

  struct stat dummy{};
  if (stat(newpath, &dummy) == 0) {
    if (D) qDebug() << "!n" << tr("MKDIR: '%1' already exists").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::ReadOnly);
    completeCommand(cunit);
    return;
  }

#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  if (mkdir(newpath, S_IRWXU | S_IRWXG | S_IRWXO))
#else
  if (mkdir(newpath))
#endif
  {
    if (D) qDebug() << "!n" << tr("MKDIR: cannot make dir '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::General);
  } else {
    const time_t mtime = timestamp2mtime(dt);
    device[cunit].status.err = static_cast<uchar>(PclinkError::Success);
    if (mtime) {
      utimbuf ub{};
      ub.actime = mtime;
      ub.modtime = mtime;
      utime(newpath, &ub);
    }
  }
  completeCommand(cunit);
}

void PCLINK::handleRmdir(const quint8 cunit, const uchar ccom) {
  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  char newpath[1024];
  create_user_path(cunit, newpath);

  if (!validate_user_path(device[cunit].dirname, newpath)) {
    if (D) qDebug() << "!n" << tr("invalid path 6 '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    completeCommand(cunit);
    return;
  }

  char fname[12];
  /* convert NNNNNNNNXXX to 8+3 */
  uexpand(device[cunit].parbuf.name, fname);

  if (validate_dos_name(fname)) {
    if (D) qDebug() << "!n" << tr("bad dir name '%1'").arg(fname);
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadFilename);
    completeCommand(cunit);
    return;
  }

  strcat(newpath, HOST_SEPARATOR_STR);
  if (is_fname_reserved(fname)) {
    strcat(newpath, RESERVED_NAME_PREFIX_STR);
  }
  strcat(newpath, fname);

  struct stat sb{};
  if (stat(newpath, &sb) < 0) {
    if (D) qDebug() << "!n" << tr("cannot stat '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
    completeCommand(cunit);
    return;
  }

#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  if (access(newpath, W_OK) < 0) {
    if (D) qDebug() << "!n" << tr("'%1' can't be accessed").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
    completeCommand(cunit);
    return;
  }
#endif

  if (!S_ISDIR(sb.st_mode)) {
    if (D) qDebug() << "!n" << tr("'%1' is not a directory").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
    completeCommand(cunit);
    return;
  }

  if ((sb.st_mode & S_IWUSR) == 0) {
    if (D) qDebug() << "!n" << tr("dir '%1' is write-protected").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::FileNotFound);
    completeCommand(cunit);
    return;
  }

  if (D) qDebug() << "!n" << tr("delete dir '%1'").arg(newpath);

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

  if (rmdir(newpath)) {
    if (D)
      qDebug() << "!n" << tr("RMDIR: cannot del '%1', %2 (%3)").arg(newpath, strerror(errno), QString::number(errno));
    if (errno == ENOTEMPTY)
      device[cunit].status.err = static_cast<uchar>(PclinkError::DirNotEmpty);
    else
      device[cunit].status.err = static_cast<uchar>(PclinkError::General);
  }
  completeCommand(cunit);
}

void PCLINK::handleChdir(const quint8 cunit, const uchar ccom) {
  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  char newpath[1024];
  create_user_path(cunit, newpath);

  if (!validate_user_path(device[cunit].dirname, newpath)) {
    if (D) qDebug() << "!n" << tr("invalid path 7 '%1'").arg(newpath);
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    completeCommand(cunit);
    return;
  }

  char oldwd[1024];
  (void) getcwd(oldwd, sizeof(oldwd));

  if (chdir(newpath)) {
    if (D)
      qDebug() << "!n" << tr("cannot access '%1', %2").arg(newpath, strerror(errno), QString::number(errno));
    device[cunit].status.err = static_cast<uchar>(PclinkError::InvalidPath);
    completeCommand(cunit);
    return;
  }

  char newwd[1024];
  (void) getcwd(newwd, sizeof(newwd));

  /* validate_user_path() guarantees that .dirname is part of newwd */
  const auto i = strlen(device[cunit].dirname);

  // copy the newwd (new working directory) to cwd (current working directory)
  // and remove RESERVED_NAME_PREFIX_CHAR (i.e. replace "!com1" with "com1")
  // to report proper names to SDX
  const char *src_ptr = newwd + i;
  auto dst_ptr = reinterpret_cast<char*>(device[cunit].cwd);
  while (*src_ptr) {
    if (*src_ptr != RESERVED_NAME_PREFIX_CHAR) {
      *dst_ptr++ = *src_ptr;
    }
    src_ptr++;
  }
  *dst_ptr = 0;

  if (D) qDebug() << "!n" << tr("new current dir '%1'").arg(reinterpret_cast<char*>(device[cunit].cwd));

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

  (void) chdir(oldwd);

  completeCommand(cunit);
}

void PCLINK::handleGetCwd(const quint8 cunit, const uchar ccom) {
  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

  if (ccom == 'P') {
    if (D) qDebug() << "!n" << tr("device $1").arg(cunit);
    completeCommand(cunit);
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  uchar tempcwd[65];
  tempcwd[0] = 0;

  int i = 0;
  for (i = 0; device[cunit].cwd[i] && i < 64; i++) {
    uchar a = static_cast<quint8>(toupper(device[cunit].cwd[i]));
    if (a == HOST_SEPARATOR_CHAR)
      a = '>';
    tempcwd[i] = a;
  }
  tempcwd[i] = 0;

  if (D) qDebug() << "!n" << tr("send '%1'").arg(reinterpret_cast<const char*>(tempcwd));

  QByteArray data(reinterpret_cast<const char*>(tempcwd), sizeof(tempcwd) - 1);
  sio->port()->writeComplete();
  sio->port()->writeDataFrame(data);
}

void PCLINK::handleDFree(const quint8 cunit, const uchar ccom) {
  static uchar dfree[65] = {
      0x21,                                           /* data format version */
      0x00, 0x00,                                     /* main directory ptr */
      0xff, 0xff,                                     /* total sectors */
      0xff, 0xff,                                     /* free sectors */
      0x00,                                           /* bitmap length */
      0x00, 0x00,                                     /* bitmap begin */
      0x00, 0x00,                                     /* filef */
      0x00, 0x00,                                     /* dirf */
      0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, /* volume name */
      0x00,                                           /* number of tracks */
      0x01,                                           /* bytes per sector, encoded */
      0x80,                                           /* version number */
      0x00, 0x02,                                     /* real bps */
      0x00, 0x00,                                     /* fmapen */
      0x01,                                           /* sectors per cluster */
      0x00, 0x00,                                     /* nr seq and rnd */
      0x00, 0x00,                                     /* bootp */
      0x00,                                           /* lock */

      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0,
      0 /* CRC */
  };

  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

  if (ccom == 'P') {
    if (D) qDebug() << "!n" << tr("device $%1").arg(cunit);
    completeCommand(cunit);
    return;
  }

  sio->port()->writeCommandAck(); /* ack the command */

  memset(dfree + 0x0e, 0x020, 8);

  char lpath[1024];
  strcpy(lpath, device[cunit].dirname);
  strcat(lpath, HOST_SEPARATOR_STR);
  strcat(lpath, DEVICE_LABEL);

  if (D) qDebug() << "!n" << tr("reading '%1'").arg(lpath);

  FILE *vf = fopen(lpath, "rb");
  if (vf) {
    uchar volname[8];
    const size_t r = fread(volname, sizeof(uchar), 8, vf);
    fclose(vf);

    for (size_t y = 0; y < r; y++) {
      const uchar a = volname[y];
      if (a == 0x9b)
        break;
      dfree[14 + y] = a;
    }
  }

  uchar c = 0;
  int x = 0;
  while (x < 8) {
    c |= dfree[14 + x];
    x++;
  }

  if (c == 0x20) {
    memcpy(dfree + 14, "PCLink  ", 8); // NOLINT(*-not-null-terminated-result)
    dfree[21] = cunit + 0x40;
  }

  if (D) qDebug() << "!n" << tr("DFREE: send info (%1 bytes)").arg(static_cast<int>(sizeof(dfree)) - 1);

  QByteArray data(reinterpret_cast<const char*>(dfree), sizeof(dfree) - 1);
  sio->port()->writeComplete();
  sio->port()->writeDataFrame(data);
}

void PCLINK::handleChvol(const quint8 cunit, const uchar ccom) {
  device[cunit].status.err = static_cast<uchar>(PclinkError::Success);

  if (ccom == 'R') {
    sio->port()->writeCommandAck(); /* ack the command */
    device[cunit].status.err = static_cast<uchar>(PclinkError::BadExec);
    if (D) qDebug() << "!n" << tr("bad exec");
    completeCommand(cunit);
    return;
  }

  const auto nl = strlen(reinterpret_cast<char*>(device[cunit].parbuf.name));
  if (nl == 0) {
    if (D) qDebug() << "!n" << tr("invalid name");
    device[cunit].status.err = 156;
    completeCommand(cunit);
    return;
  }

  char lpath[1024];
  strcpy(lpath, device[cunit].dirname);
  strcat(lpath, HOST_SEPARATOR_STR);
  strcat(lpath, DEVICE_LABEL);

  if (D) qDebug() << "!n" << tr("writing '%1'").arg(lpath);

  FILE *vf = fopen(lpath, "wb");
  if (vf) {
    for (int x = 0; x < 8; x++) {
      uchar a = device[cunit].parbuf.name[x];
      if (!a || a == 0x9b)
        a = 0x20;
      (void) fwrite(&a, sizeof(uchar), 1, vf);
    }
    fclose(vf);
  } else {
    if (D) qDebug() << "!n" << tr("CHVOL: %1").arg(strerror(errno));
    device[cunit].status.err = static_cast<uchar>(PclinkError::General);
  }
  completeCommand(cunit);
}

/*************************************************************************
 * Path Resolution & Validation Helpers
 *************************************************************************/

// ReSharper disable once CppMemberFunctionMayBeStatic
bool PCLINK::isSDXPathSeparator(const uchar c) { // NOLINT(*-convert-member-functions-to-static)
  return c == '>' || c == '\\';
}

void PCLINK::path_copy(uchar *dst, uchar *src) {
  bool in_file_name = false;
  bool in_file_suffix = false;

  auto src_prev_ptr = src;
  auto src_ptr = src;
  const auto dst_ptr = dst;

  while (true) {
    if (in_file_name) {
      if (!in_file_suffix) {
        if (*src_ptr && !isSDXPathSeparator(*src_ptr) && *src_ptr != SDX_GO_UP_DIR_CHAR && *src_ptr != '.') {
          src_ptr++;
        } else {
          const auto length = static_cast<size_t>(src_ptr - src_prev_ptr);
          if (length != 0 && is_fname_reserved(reinterpret_cast<char*>(src_prev_ptr), length)) *dst++ = RESERVED_NAME_PREFIX_CHAR;

          for (size_t i = 0; i < length; i++) {
            *dst++ = static_cast<quint8>(RespeqtSettings::instance()->capitalLettersInPCLINK() ? toupper(*src_prev_ptr) : tolower(*src_prev_ptr));
            src_prev_ptr++;
          }

          if (*src_ptr == '.') {
            *dst++ = *src_ptr++;
            src_prev_ptr = src_ptr;
            in_file_suffix = true;
          } else {
            in_file_name = false;
          }
        }
      } else// in a suffix
      {
        if (*src_ptr && !isSDXPathSeparator(*src_ptr) && *src_ptr != SDX_GO_UP_DIR_CHAR) {
          *dst++ = static_cast<quint8>(RespeqtSettings::instance()->capitalLettersInPCLINK() ? toupper(*src_ptr) : tolower(*src_ptr));
          src_ptr++;
        } else {
          in_file_name = false;
          in_file_suffix = false;
        }
      }
    } else// outside file name
    {
      if (*src_ptr == SDX_GO_UP_DIR_CHAR) {
        *dst++ = '.';
        *dst++ = '.';
        *dst++ = HOST_SEPARATOR_CHAR;
        src_ptr++;
      } else if (isSDXPathSeparator(*src_ptr)) {
        if (dst_ptr == dst || *(dst - 1) != HOST_SEPARATOR_CHAR) *dst++ = HOST_SEPARATOR_CHAR;// make sure that the last character was a slash
        while (*src_ptr && isSDXPathSeparator(*src_ptr)) src_ptr++;                           // skip superfluous seperators
      } else if (*src_ptr == 0) {
        *dst = 0;
        break;
      } else {
        in_file_name = true;
        src_prev_ptr = src_ptr;
      }
    }
  }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
int PCLINK::validate_user_path(const char *defwd, const char *newpath) { // NOLINT(*-convert-member-functions-to-static)
  char oldwd[1024], newwd[1024];

  (void) getcwd(oldwd, sizeof(oldwd));
  if (chdir(newpath) < 0)
    return 0;
  (void) getcwd(newwd, sizeof(newwd));
  (void) chdir(oldwd);

  const char* d = strstr(newwd, defwd);

  if (d == nullptr)
    return 0;
  if (d != newwd)
    return 0;

  return 1;
}

void PCLINK::create_user_path(const uchar cunit, char *newpath) {
  size_t sl;
  uchar upath[128];

  strcpy(newpath, device[cunit].dirname);

  path_copy(upath, device[cunit].parbuf.path);

  if (upath[0] != HOST_SEPARATOR_CHAR) {
    long cwdo = 0;
    sl = strlen(newpath);
    if (sl && newpath[sl - 1] != HOST_SEPARATOR_CHAR) {
      newpath[sl++] = HOST_SEPARATOR_CHAR;
      newpath[sl] = 0;
    }
    if (device[cunit].cwd[0] == HOST_SEPARATOR_CHAR)
      cwdo++;

    path_copy(reinterpret_cast<unsigned char*>(newpath) + sl, static_cast<unsigned char*>(device[cunit].cwd) + cwdo);

    sl = strlen(newpath);
    if (sl && newpath[sl - 1] != HOST_SEPARATOR_CHAR) {
      newpath[sl++] = HOST_SEPARATOR_CHAR;
      newpath[sl] = 0;
    }
  }
  strcat(newpath, reinterpret_cast<char*>(upath));
  sl = strlen(newpath);
  if (sl && newpath[sl - 1] == HOST_SEPARATOR_CHAR)
    newpath[sl - 1] = 0;
}

/*************************************************************************
 * Filename & Attribute Helpers
 *************************************************************************/

// ReSharper disable once CppMemberFunctionMayBeStatic
bool PCLINK::is_fname_reserved(const char *fname, size_t length) const { // NOLINT(*-convert-member-functions-to-static)
  bool fname_reserved = false;
  const char *const *p = invalid_file_names;

  if (length == ULONG_MAX) {
    const char *dot_ptr = strchr(fname, '.');
    length = dot_ptr == nullptr ? strlen(fname) : static_cast<size_t>(dot_ptr - fname);
  }

  while (const char *s = *p++) {
    if (const size_t reserved_str_len = strlen(s); length == reserved_str_len && strncasecmp(s, fname, reserved_str_len) == 0) {
      fname_reserved = true;
      break;
    }
  }
  return fname_reserved;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool PCLINK::is_fname_encoded(const char *fname) const { // NOLINT(*-convert-member-functions-to-static)
  bool fname_encoded = false;
  const char *const *p = invalid_file_names;

  const char *dot_ptr = strchr(fname, '.');
  const size_t length = dot_ptr == nullptr ? strlen(fname) : static_cast<size_t>(dot_ptr - fname);

  while (const char *s = *p++) {
    if (const size_t reserved_str_len = strlen(s); length == reserved_str_len + 1 &&
        RESERVED_NAME_PREFIX_CHAR == fname[0] &&
        strncasecmp(s, fname + 1, reserved_str_len) == 0) {
      fname_encoded = true;
      break;
    }
  }
  return fname_encoded;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool PCLINK::isSDXLegalChar(const uchar c) { // NOLINT(*-convert-member-functions-to-static)
#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  if (RespeqtSettings::instance()->capitalLettersInPCLINK())
    return isupper(c) || isdigit(c) || c == '_' || c == '@';

  return islower(c) || isdigit(c) || c == '_' || c == '@';
#else
  return isalpha(c) || isdigit(c) || (c == '_') || c == '@';
#endif
}

// ReSharper disable once CppMemberFunctionMayBeStatic
long PCLINK::dos_2_term(const uchar c) { // NOLINT(*-convert-member-functions-to-static)
  return c == 0 || c == 0x20;
}

long PCLINK::validate_fn(const uchar *name, const int len) {
  for (int x = 0; x < len; x++) {
    if (dos_2_term(name[x]))
      return x != 0;
    if (name[x] == '.')
      return 1;
    if (!isSDXLegalChar(name[x]))
      return 0;
  }

  return 1;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void PCLINK::ugefina(char *src, char *out) {
  ushort i;

  memset(out, 0x20, 8 + 3);

  if (const char* dot = strchr(src, '.')) {
    i = 1;
    while (dot[i] && i < 4) {
      out[i + 7] = static_cast<char>(toupper(dot[i]));
      i++;
    }
  }

  i = 0;
  while (src[i] != '.' && !dos_2_term(static_cast<quint8>(src[i])) && i < 8) {
    out[i] = static_cast<char>(toupper(src[i]));
    i++;
  }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void PCLINK::uexpand(const uchar *rawname, char *name83) { // NOLINT(*-convert-member-functions-to-static)
  ushort x;

  name83[0] = 0;

  for (x = 0; x < 8; x++) {
    if (const uchar t = rawname[x]; t && t != 0x20)
      name83[x] = static_cast<char>(RespeqtSettings::instance()->capitalLettersInPCLINK() ? toupper(t) : tolower(t));
    else
      break;
  }

  if (ushort y = 8; rawname[y] && rawname[y] != 0x20) {
    name83[x] = '.';
    x++;

    while (y < 11 && rawname[y] && rawname[y] != 0x20) {
      name83[x] = static_cast<char>(RespeqtSettings::instance()->capitalLettersInPCLINK() ? toupper(rawname[y]) : tolower(rawname[y]));
      x++;
      y++;
    }
  }

  name83[x] = 0;
}

int PCLINK::validate_dos_name(char *fname) {
  char *dot = strchr(fname, '.');
  long valid_xx;

  if (dot == nullptr && strlen(fname) > 8)
    return 1;
  if (dot) {
    const size_t dd = strlen(dot);

    if (dd > 4)
      return 1;
    if (dot - fname > 8)
      return 1;
    if (dot == fname && dd == 1)
      return 1;
    if (dd == 2 && dot[1] == '.')
      return 1;
    if (dd == 3 && (dot[1] == '.' || dot[2] == '.'))
      return 1;
    if (dd == 4 && (dot[1] == '.' || dot[2] == '.' || dot[3] == '.'))
      return 1;
  }

  const long valid_fn = validate_fn(reinterpret_cast<unsigned char*>(fname), 8);
  if (dot != nullptr)
    valid_xx = validate_fn(reinterpret_cast<unsigned char*>(dot + 1), 3);
  else
    valid_xx = 1;

  if (!valid_fn || !valid_xx)
    return 1;

  return 0;
}

int PCLINK::check_dos_name(char *newpath, dirent *dp, struct stat *sb) {
  char temp_fspec[1024], fname[256];

  strcpy(fname, dp->d_name);

  if (is_fname_encoded(dp->d_name)) {
    memmove(dp->d_name, dp->d_name + 1, strlen(dp->d_name));
  }

  if (D) qDebug() << "!n" << tr("%1: got fname '%2'").arg(__extension__ __FUNCTION__, dp->d_name);

  if (validate_dos_name(dp->d_name))
    return 1;

  /* stat() the file (fetches the length) */
  snprintf(temp_fspec, 1024, "%s/%s", newpath, fname);

  if (D) qDebug() << "!n" << tr("%1: stat '%2'").arg(__extension__ __FUNCTION__, dp->d_name);

  if (stat(temp_fspec, sb))
    return 1;

  if (!S_ISREG(sb->st_mode) && !S_ISDIR(sb->st_mode))
    return 1;

#if defined(Q_OS_LINUX) || defined(Q_OS_OSX)
  if (sb->st_mode == S_IFLNK) {
    if (D) qDebug() << "!n" << tr("'%1': is a symlink").arg(temp_fspec);
  }

  if (access(temp_fspec, R_OK) < 0) { /* belongs to us? */
    if (D) qDebug() << "!n" << tr("'%1': can't be accessed").arg(temp_fspec);
    if (D) qDebug() << "!n" << tr("access error code %1").arg(errno);
    return 1;
  }
#endif

  if ((sb->st_mode & S_IRUSR) == 0) /* unreadable? */
    return 1;

  if (S_ISDIR(sb->st_mode) && (sb->st_mode & S_IXUSR) == 0)
    return 1;

  return 0;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
int PCLINK::match_dos_names(const char *name, const char *mask, uchar fatr1, const struct stat *sb) { // NOLINT(*-convert-member-functions-to-static)
  if (D) qDebug() << "!n" << tr("match: %1%2%3%4%5%6%7%8%9%10%11 with %12%13%14%15%16%17%18%19%20%21%22: ").arg(name[0]).arg(name[1]).arg(name[2]).arg(name[3]).arg(name[4]).arg(name[5]).arg(name[6]).arg(name[7]).arg(name[8]).arg(name[9]).arg(name[10]).arg(mask[0]).arg(mask[1]).arg(mask[2]).arg(mask[3]).arg(mask[4]).arg(mask[5]).arg(mask[6]).arg(mask[7]).arg(mask[8]).arg(mask[9]).arg(mask[10]);

  for (ushort i = 0; i < 11; i++) {
    if (mask[i] != '?')
      if (toupper(static_cast<unsigned char>(name[i])) != toupper(static_cast<unsigned char>(mask[i]))) {
        if (D) qDebug() << "!n" << tr("no match");
        return 1;
      }
  }

  /* There are no such attributes in Unix */
  fatr1 &= ~(RA_NO_HIDDEN | RA_NO_ARCHIVED);

  /* Now check the attributes */
  if (fatr1 & (RA_HIDDEN | RA_ARCHIVED)) {
    if (D) qDebug() << "!n" << tr("atr mismatch: not HIDDEN or ARCHIVED");
    return 1;
  }

  if (fatr1 & RA_PROTECT) {
    if (sb->st_mode & S_IWUSR) {
      if (D) qDebug() << "!n" << tr("atr mismatch: not PROTECTED");
      return 1;
    }
  }

  if (fatr1 & RA_NO_PROTECT) {
    if ((sb->st_mode & S_IWUSR) == 0) {
      if (D) qDebug() << "!n" << tr("atr mismatch: not UNPROTECTED");
      return 1;
    }
  }

  if (fatr1 & RA_SUBDIR) {
    if (!S_ISDIR(sb->st_mode)) {
      if (D) qDebug() << "!n" << tr("atr mismatch: not SUBDIR");
      return 1;
    }
  }

  if (fatr1 & RA_NO_SUBDIR) {
    if (S_ISDIR(sb->st_mode)) {
      if (D) qDebug() << "!n" << tr("atr mismatch: not FILE");
      return 1;
    }
  }

  if (D) qDebug() << "!n" << tr("match");
  return 0;
}

/*************************************************************************
 * Timestamp Helpers
 *************************************************************************/

// ReSharper disable once CppMemberFunctionMayBeStatic
void PCLINK::unix_time_2_sdx(const time_t *todp, uchar *ob) { // NOLINT(*-convert-member-functions-to-static)
  memset(ob, 0, 6);

  if (*todp == 0 || *todp == -1)
    return;

  const tm* t = localtime(todp);

  int yy = t->tm_year;
  while (yy >= 100)
    yy -= 100;

  ob[0] = static_cast<quint8>(t->tm_mday);
  ob[1] = static_cast<quint8>(t->tm_mon + 1);
  ob[2] = static_cast<quint8>(yy);
  ob[3] = static_cast<quint8>(t->tm_hour);
  ob[4] = static_cast<quint8>(t->tm_min);
  ob[5] = static_cast<quint8>(t->tm_sec);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
time_t PCLINK::timestamp2mtime(const uchar *stamp) { // NOLINT(*-convert-member-functions-to-static)
  tm sdx_tm = {};

  sdx_tm.tm_sec = stamp[5];
  sdx_tm.tm_min = stamp[4];
  sdx_tm.tm_hour = stamp[3];
  sdx_tm.tm_mday = stamp[0];
  sdx_tm.tm_mon = stamp[1];
  sdx_tm.tm_year = stamp[2];

  if (sdx_tm.tm_mday == 0 || sdx_tm.tm_mon == 0)
    return 0;

  if (sdx_tm.tm_mon)
    sdx_tm.tm_mon--;

  if (sdx_tm.tm_year < 80)
    sdx_tm.tm_year += 2000;
  else
    sdx_tm.tm_year += 1900;

  sdx_tm.tm_year -= 1900;

  return mktime(&sdx_tm);
}

/*************************************************************************
 * Descriptor & Directory Cache Management
 *************************************************************************/

// ReSharper disable once CppMemberFunctionMayBeStatic
void PCLINK::fps_close(const int i) { // NOLINT(*-convert-member-functions-to-static)
  if (iodesc[i].fps.file != nullptr) {
    if (iodesc[i].fpmode & 0x10)
      closedir(iodesc[i].fps.dir);
    else
      fclose(iodesc[i].fps.file);
  }

  if (iodesc[i].dir_cache != nullptr) {
    free(iodesc[i].dir_cache);
    iodesc[i].dir_cache = nullptr;
  }

  iodesc[i].fps.file = nullptr;

  iodesc[i].devno = 0;
  iodesc[i].cunit = 0;
  iodesc[i].fpmode = 0;
  iodesc[i].fatr1 = 0;
  iodesc[i].fatr2 = 0;
  iodesc[i].t1 = 0;
  iodesc[i].t2 = 0;
  iodesc[i].t3 = 0;
  iodesc[i].d1 = 0;
  iodesc[i].d2 = 0;
  iodesc[i].d3 = 0;
  iodesc[i].fpname[0] = 0;
  iodesc[i].fppos = 0;
  iodesc[i].fpread = 0;
  iodesc[i].eof = 0;
  iodesc[i].pathname[0] = 0;
  memset(&iodesc[i].fpstat, 0, sizeof(struct stat));
}

off_t PCLINK::get_file_len(const uchar handle) {
  off_t filelen;
  struct stat sb{};

  if (iodesc[handle].fpmode & 0x10) /* directory */
  {
    dirent *dp;
    rewinddir(iodesc[handle].fps.dir);
    filelen = sizeof(DIRENTRY);

    while ((dp = readdir(iodesc[handle].fps.dir)) != nullptr) {
      if (check_dos_name(iodesc[handle].pathname, dp, &sb))
        continue;
      filelen += sizeof(DIRENTRY);
    }
    rewinddir(iodesc[handle].fps.dir);
  } else
    filelen = iodesc[handle].fpstat.st_size;

  if (filelen > SDX_MAXLEN)
    filelen = SDX_MAXLEN;

  return filelen;
}

DIRENTRY *PCLINK::cache_dir(const uchar handle) {
  uchar dirnode = 0x00;
  ushort node;
  // ReSharper disable once CppRedundantCastExpression
  const size_t dirlen = static_cast<size_t>(iodesc[handle].fpstat.st_size);
  DIRENTRY *dbuf;
  dirent *dp;
  struct stat sb{};

  if (iodesc[handle].dir_cache != nullptr) {
    if (D) qDebug() << "!n" << tr("Internal error: dir_cache should be nullptr!");
    free(iodesc[handle].dir_cache);
    iodesc[handle].dir_cache = nullptr;
  }

  DIRENTRY* dir = dbuf = static_cast<DIRENTRY*>(malloc(dirlen + sizeof(DIRENTRY)));
  memset(dbuf, 0, dirlen + sizeof(DIRENTRY));

  dir->status = 0x28;
  dir->map_l = 0x00; /* low 11 bits: file number, high 5 bits: dir number */
  dir->map_h = dirnode;
  dir->len_l = dirlen & 0x000000ffL;
  dir->len_m = (dirlen & 0x0000ff00L) >> 8;
  dir->len_h = (dirlen & 0x00ff0000L) >> 16;

  memset(dir->fname, 0x20, 11);

  const ulong sl = strlen(device[iodesc[handle].cunit].dirname);

  const char* cwd = iodesc[handle].pathname + sl;

  // ReSharper disable once CppRedundantCastExpression
  if (const auto bs = const_cast<char*>(strrchr(cwd, HOST_SEPARATOR_CHAR)); bs == nullptr)
  {
    //(dir->fname, "MAIN", 4);
  } else {
    const char *cp = cwd;

    /* convert 8+3 to NNNNNNNNXXX */
    ugefina(bs + 1, dir->fname);

    if (is_fname_encoded(bs + 1)) {
      memmove(dir->fname, dir->fname + 1, strlen(dir->fname));
    }

    node = 0;

    while (cp <= bs) {
      if (*cp == HOST_SEPARATOR_CHAR)
        dirnode++;
      cp++;
    }

    dir->map_h = static_cast<quint8>((dirnode & 0x1f) << 3);
  }

  unix_time_2_sdx(&iodesc[handle].fpstat.st_mtime, dir->stamp);

  dir++;
  ulong flen = sizeof(DIRENTRY);

  node = 1;

  while ((dp = readdir(iodesc[handle].fps.dir)) != nullptr) {
    if (check_dos_name(iodesc[handle].pathname, dp, &sb))
      continue;

    off_t dlen = sb.st_size;
    if (dlen > SDX_MAXLEN)
      dlen = SDX_MAXLEN;

    dir->status = sb.st_mode & S_IWUSR ? 0x08 : 0x09;

    if (S_ISDIR(sb.st_mode)) {
      dir->status |= 0x20; /* directory */
      dlen = sizeof(DIRENTRY);
    }

    ushort map = static_cast<ushort>(dirnode << 11);
    map |= node & 0x07ff;

    dir->map_l = map & 0x00ff;
    dir->map_h = (map & 0xff00) >> 8;
    dir->len_l = dlen & 0x000000ffL;
    dir->len_m = (dlen & 0x0000ff00L) >> 8;
    dir->len_h = (dlen & 0x00ff0000L) >> 16;

    /* convert 8+3 to NNNNNNNNXXX */
    ugefina(dp->d_name, dir->fname);

    unix_time_2_sdx(&sb.st_mtime, dir->stamp);

    node++;
    dir++;
    flen += sizeof(DIRENTRY);

    if (flen >= dirlen)
      break;
  }

  return dbuf;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
size_t PCLINK::dir_read(uchar *mem, ulong blk_size, const uchar handle, int *eof_sig) { // NOLINT(*-convert-member-functions-to-static)
  const auto db = reinterpret_cast<unsigned char*>(iodesc[handle].dir_cache);
  const auto dirlen = iodesc[handle].fpstat.st_size;

  eof_sig[0] = 0;

  if (const auto newblk = static_cast<size_t>(dirlen) - iodesc[handle].fppos; newblk < blk_size) {
    blk_size = newblk;
    eof_sig[0] = 1;
  }

  if (blk_size)
    memcpy(mem, db + iodesc[handle].fppos, blk_size);

  return blk_size;
}

void PCLINK::do_pclink_init(const int force) {
  if (force == 0) {
    if (D) qDebug() << "!n" << tr("closing all files");
  }

  for (uchar handle = 0; handle < 16; handle++) {
    if (force)
      iodesc[handle].fps.file = nullptr;
    fps_close(handle);
    memset(&device[handle].parbuf, 0, sizeof(PARBUF));
  }
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void PCLINK::set_status_size(const uchar cunit, const ushort size) { // NOLINT(*-convert-member-functions-to-static)
  device[cunit].status.tmot = size & 0x00ff;
  device[cunit].status.none = (size & 0xff00) >> 8;
}

/*************************************************************************/

} // namespace SIO
