/*
 * pclink.h
 *
 * This file contains code from the SIO2BSD project by KMK (drac030)
 *
 * Copyright 2016 TheMontezuma
 *
 */

#ifndef PCLINK_H
#define PCLINK_H

#include <climits>
#include <ctime>
#include <sys/stat.h>
#include "sio/sdxprotocol.h"

struct dirent;

namespace SIO {

enum class PclinkFunction : quint8 {
  FRead   = 0x00,
  FWrite  = 0x01,
  FSeek   = 0x02,
  FTell   = 0x03,
  FLen    = 0x04,
  FNext   = 0x06,
  FClose  = 0x07,
  Init    = 0x08,
  FOpen   = 0x09,
  FFirst  = 0x0A,
  Rename  = 0x0B,
  Remove  = 0x0C,
  Chmod   = 0x0D,
  Mkdir   = 0x0E,
  Rmdir   = 0x0F,
  Chdir   = 0x10,
  GetCwd  = 0x11,
  SetBoot = 0x12,
  DFree   = 0x13,
  ChVol   = 0x14
};

enum class PclinkError : quint8 {
  Success         = 1,
  BadHandle       = 134,
  Eof             = 136,
  BlockCrc        = 143,
  UnsupportedMode = 146,
  InvalidPath     = 150,
  ReadOnly        = 151,
  BadFilename     = 165,
  SeekError       = 166,
  DirNotEmpty     = 167,
  FileNotFound    = 170,
  BadExec         = 176,
  ChannelLimit    = 179,
  General         = 255
};

class PCLINK : public SDXProtocol {
  Q_OBJECT

public:
  explicit PCLINK(const SioWorkerPtr &worker);
  ~PCLINK() override = default;
  void handleCommand(quint8 command, quint8 aux1, quint8 aux2) override;
  // links are numbered from 1 to 15
  bool hasLink(int no);
  void setLink(int no, const char *fileName);
  void swapLinks(int from, int to);
  void resetLink(int no);

private:
  // Command dispatcher & execution
  void do_pclink(uchar devno, uchar ccom, uchar caux1, uchar caux2);
  bool processParameterBuffer(uchar ccom, quint8 cunit, quint8 parsize);
  void completeCommand(quint8 cunit);

  // Command handlers
  void handleFRead(quint8 cunit, uchar ccom, ulong faux, uchar handle);
  void handleFWrite(quint8 cunit, uchar ccom, ulong faux, uchar handle);
  void handleFSeek(quint8 cunit, uchar ccom, ulong faux, uchar handle);
  void handleFTell(quint8 cunit, uchar ccom, uchar handle);
  void handleFLen(quint8 cunit, uchar ccom, uchar handle);
  void handleFNext(quint8 cunit, uchar ccom, uchar handle);
  void handleFClose(quint8 cunit, uchar ccom, uchar handle);
  void handleInit(quint8 cunit, uchar ccom);
  void handleFOpen(quint8 cunit, uchar ccom, ulong faux, uchar handle, uchar fno);
  void handleRename(quint8 cunit, uchar ccom);
  void handleRemove(quint8 cunit, uchar ccom);
  void handleChmod(quint8 cunit, uchar ccom);
  void handleMkdir(quint8 cunit, uchar ccom);
  void handleRmdir(quint8 cunit, uchar ccom);
  void handleChdir(quint8 cunit, uchar ccom);
  void handleGetCwd(quint8 cunit, uchar ccom);
  void handleDFree(quint8 cunit, uchar ccom);
  void handleChvol(quint8 cunit, uchar ccom);

  // FOpen / FFirst / FNext sub-methods
  bool validateOpenParameters(quint8 cunit, uchar fmode, const char *newpath);
  int findFreeDescriptor();
  bool openDirectoryStream(quint8 cunit, int handle, const char *newpath, const char *fname_expanded);
  bool openRegularFileStream(quint8 cunit, int handle, uchar fmode, const char *newpath, const char *fname_expanded, bool is_first, struct stat *outStat);
  void populateOpenDescriptor(int handle, quint8 cunit, uchar fmode, uchar fatr1, uchar fatr2, const struct stat *sb, const char *fname_expanded, const char *newpath);
  void buildOpenResponseFrame(int handle, const struct stat *sb, const char *fname_expanded);
  bool scanNextDirectoryEntry(quint8 cunit, uchar handle);

  // Path resolution & validation utilities
  void create_user_path(uchar cunit, char *newpath);
  int validate_user_path(const char *defwd, const char *newpath);
  void path_copy(uchar *dst, uchar *src);
  bool isSDXPathSeparator(uchar c);

  // Filename & attribute utilities
  void ugefina(char *src, char *out);
  void uexpand(const uchar *rawname, char *name83);
  int validate_dos_name(char *fname);
  int check_dos_name(char *newpath, struct dirent *dp, struct stat *sb);
  int match_dos_names(const char *name, const char *mask, uchar fatr1, const struct stat *sb);
  bool is_fname_reserved(const char *fname, size_t length = ULONG_MAX) const;
  bool is_fname_encoded(const char *fname) const;
  bool isSDXLegalChar(uchar c);
  long dos_2_term(uchar c);
  long validate_fn(const uchar *name, int len);

  // Timestamp conversion utilities
  void unix_time_2_sdx(const time_t *todp, uchar *ob);
  time_t timestamp2mtime(const uchar *stamp);

  // Descriptor & directory cache management
  void fps_close(int i);
  off_t get_file_len(uchar handle);
  DIRENTRY *cache_dir(uchar handle);
  size_t dir_read(uchar *mem, ulong blk_size, uchar handle, int *eof_sig);
  void do_pclink_init(int force);
  void set_status_size(uchar cunit, ushort size);
};

} // namespace SIO

#endif// PCLINK_H
