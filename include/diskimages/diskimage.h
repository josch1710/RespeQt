/*
 * diskimage.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef DISKIMAGE_H
#define DISKIMAGE_H

#include <QElapsedTimer>
#include <QFile>
#include <QList>
#include <QTemporaryFile>
#include <QtDebug>

#include "crc16.h"
#include "diskimages/atxsectorinfo.h"
#include "diskimages/atxtrackinfo.h"
#include "diskimages/board.h"
#include "diskimages/diskgeometry.h"
#include "diskimages/prosectorinfo.h"
#include "include/diskimages/disassembly1050.h"
#include "include/diskimages/disassembly810.h"

#include "miscutils.h"
#include "sioworker.h"

class DiskEditDialog;

namespace DiskImages {
  class DiskImage;
  using DiskImagePtr = QSharedPointer<DiskImage>;

  class DiskImage : public SioDevice {
    Q_OBJECT

  public:
    explicit DiskImage(SioWorkerPtr worker, bool gzipped);
    ~DiskImage() override;

    virtual bool open(const QString &fileName);
    virtual bool openImage(const QString &fileName) = 0;
    virtual bool create(int untitledName) = 0;
    virtual bool save() = 0;
    virtual bool format(const DiskGeometry &geo) = 0;
    virtual bool readHappySectorAtPosition(int trackNumber, int sectorNumber, int afterSectorNumber, int &index, QByteArray &data) = 0;
    virtual bool readHappySkewAlignment(bool happy1050) = 0;
    virtual bool resetTrack(quint16 aux) = 0;
    virtual void readTrack(quint16 aux, QByteArray &data, int length) = 0;
    virtual bool readSectorStatuses(QByteArray &data) = 0;
    virtual bool readSectorUsingIndex(quint16 aux, QByteArray &data) = 0;
    virtual bool readSector(quint16 aux, QByteArray &data) = 0;
    virtual bool readSkewAlignment(quint16 aux, QByteArray &data, bool timingOnly) = 0;
    virtual bool writeTrack(quint16 aux, const QByteArray &data) = 0;
    virtual bool writeTrackWithSkew(quint16 aux, const QByteArray &data) = 0;
    virtual bool writeSectorUsingIndex(quint16 aux, const QByteArray &data, bool fuzzy) = 0;
    virtual bool writeFuzzySector(quint16 aux, const QByteArray &data) = 0;
    virtual bool writeSector(quint16 aux, const QByteArray &data) = 0;
    virtual int sectorsInCurrentTrack() = 0;
    virtual bool writeHappyTrack(int trackNumber, bool happy1050) = 0;
    virtual QByteArray readHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) = 0;
    virtual bool writeHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) = 0;
    virtual bool writeSectorExtended(int bitNumber, quint8 dataType, quint8 trackNumber, quint8 sideNumber, quint8 sectorNumber, quint8 sectorSize, const QByteArray &data, bool crcError, int weakOffset) = 0;
    virtual bool saveAs(const QString &fileName) final;
    virtual bool saveImageAs() = 0;
    virtual quint8 writeSectorHeader(quint8 dataSize, quint16 sectorSlot, quint8 postDataCrc, quint8 preIDField, quint8 postIDCrc, quint8 track, quint8 index, quint8 nextSector) = 0;
    virtual bool findMappingInTrack(int nbSectors, QByteArray &mapping) = 0;

    void readHappyTrack(int trackNumber, bool happy1050);
    virtual void close();
    inline bool isOpen() const { return file.isOpen(); }
    inline bool isReadOnly() const { return m_isReadOnly; }
    inline void setReadOnly(bool readOnly) { m_isReadOnly = readOnly; }
    inline bool isLeverOpen() const { return m_isLeverOpen; }
    inline bool isModified() const { return m_isModified; }
    inline bool isUnmodifiable() const { return m_isUnmodifiable; }
    inline bool isReady() const { return m_isReady; }
    inline bool isHappyEnabled() const { return m_board.isHappyEnabled(); }
    inline bool isChipOpen() const { return m_board.isChipOpen(); }
    inline bool isTranslatorActive() const { return m_board.isTranslatorActive(); }
    inline bool isToolDiskActive() const { return m_board.isToolDiskActive(); }
    inline bool hasSeveralSides() const { return m_numberOfSides > 1; }
    virtual QString getNextSideLabel();
    inline void setEditDialog(DiskEditDialog *aDialog) {
      m_editDialog = aDialog;
      emit statusChanged(m_deviceNo);
    }
    inline DiskEditDialog *editDialog() { return m_editDialog; }
    inline void setBoardInfo(Board *info) { m_board.setFromCopy(info); }
    void handleCommand(quint8 command, quint8 aux8, quint8 aux2) override;
    virtual void getStatus(QByteArray &status);
    virtual void setReady(bool bReady);
    virtual void setChipMode(bool enable);
    virtual void setHappyMode(bool enable);
    virtual void setOSBMode(bool enable);
    virtual void setDisplayTransmission(bool active);
    virtual void setSpyMode(bool enable);
    virtual void setTrackLayout(bool enable);
    virtual void setDisassembleUploadedCode(bool enable);
    virtual void setTranslatorAutomaticDetection(bool enable);
    virtual void setTranslatorDiskImagePath(const QString &filename);
    virtual void setToolDiskImagePath(const QString &filename);
    virtual void setActivateChipModeWithTool(bool activate);
    virtual void setActivateHappyModeWithTool(bool activate);

    inline DiskGeometry geometry() const { return m_geometry; }
    virtual QString originalFileName() const;
    virtual QString description() const;

    virtual int getUploadCodeStartAddress(quint8 command, quint16 aux, QByteArray &data);
    virtual void disassembleCode(QByteArray &data, unsigned short address, bool drive1050, bool happy);

    int defaultFileSystem();

   virtual void reopen();
    __attribute__((unused)) virtual void setLeverOpen(bool open);
    inline bool isUnnamed() const { return m_isUnnamed; }
    inline QString getNextSideFilename() { return m_nextSideFilename; }
    inline Board *getBoardInfo() { return m_board.getCopy(); }
    virtual void setToolDiskMode(bool enable);

  protected:
    DiskGeometry m_geometry, m_newGeometry;
    QTemporaryFile file;
    bool m_isReadOnly;
    bool m_isLeverOpen;
    bool m_isModified;
    bool m_isUnmodifiable;
    bool m_isUnnamed;
    bool m_isReady;
    QString m_originalFileName;
    QByteArray m_originalFileHeader;
    bool m_gzipped;
    DiskEditDialog *m_editDialog;
    int m_currentSide;
    int m_numberOfSides{1};
    QString m_nextSideFilename;
    bool m_displayTransmission;
    bool m_dumpDataFrame;
    bool m_displayTrackLayout;
    bool m_disassembleUploadedCode;
    bool m_translatorAutomaticDetection;
    QString m_translatorDiskImagePath;
    bool m_OSBMode;
    QString m_toolDiskImagePath;
    bool m_toolDiskMode;
    quint16 m_trackNumber;
    qint64 m_lastTime;
    qint64 m_lastDistance;
    quint16 m_lastSector;
    quint8 m_driveStatus;
    quint8 m_wd1771Status;
    int m_sectorsInTrack;
    QElapsedTimer m_timer;
    QByteArray m_diagData;
    bool m_conversionInProgress;
    disassembly810 m_disassembly810;
    disassembly1050 m_disassembly1050;
    int m_remainingAddress;
    QByteArray m_remainingBytes;
    quint16 m_trackContent[200];
    // data for Happy or Archiver
    Board m_board;
    // Translator and tool disk
    DiskImagePtr m_translatorDisk;
    DiskImagePtr m_toolDisk;
    bool m_activateChipModeWithTool;
    bool m_activateHappyModeWithTool;

    bool seekToSector(quint16 sector);
    void refreshNewGeometry();
    bool sameSoftware(const QString &fileName, const QString &otherFileName);

    bool writeCommandAck();
    bool writeDataAck();
    bool writeCommandNak();
    bool writeDataNak();
    bool writeComplete();
    bool writeError();
    QByteArray readDataFrame(uint size);
    bool writeDataFrame(QByteArray data);
    quint8 computeVersionByte();
    quint16 getBigEndianWord(QByteArray &array, int offset);
    quint16 getLittleEndianWord(QByteArray &array, int offset);
    quint32 getLittleEndianLong(QByteArray &array, int offset);
    void setLittleEndianWord(QByteArray &array, int offset, quint16 value);
    void setLittleEndianLong(QByteArray &array, int offset, quint32 value);
    void fillBuffer(char *line, unsigned char *buf, int len, int ofs, bool dumpAscii);
    void dumpBuffer(unsigned char *buf, int len);
    bool executeArchiverCode(quint16 aux, QByteArray &data);
    int findNearestSpeed(int speed);
    void setTranslatorActive(bool resetTranslatorState);
    bool translatorDiskImageAvailable();
    void closeTranslator();
    void setToolDiskActive();
    bool toolDiskImageAvailable();
    void closeToolDisk();

  };
}
#endif// DISKIMAGE_H
