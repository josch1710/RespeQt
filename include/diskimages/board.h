
#ifndef RESPEQT_BOARD_H
#define RESPEQT_BOARD_H

#include <QtGlobal>
#include <QObject>

#include "diskimages/constvalues.h"

namespace DiskImages {

  enum class BOOT_STATE:quint8 {
    NOT_BOOTED = 0,
    FIRST_SECTOR_1,
    READ_OTHER_SECTOR,
    SECOND_SECTOR_1
  };

  class Board : public QObject {
    Q_OBJECT

  protected:
    // For Chip 810 and Super Archiver 1050 emulation
    bool m_chipOpen;
    unsigned short m_lastArchiverUploadCrc16;
    quint16 m_lastArchiverSpeed;
    // For Happy 810/1050 emulation
    bool m_happyEnabled;
    bool m_happy1050;
    unsigned short m_lastHappyUploadCrc16;
    bool m_happyPatchInProgress;
    // For translator
    bool m_translatorActive;
    BOOT_STATE m_translatorState;
    // For tool disk
    bool m_toolDiskActive;

  public:
    // For Chip 810 and Super Archiver 1050 emulation
    unsigned char m_chipRam[32];
    QByteArray m_trackData;
    // For Happy 810/1050 emulation
    QByteArray m_happyRam;

  public:
    Board();
    ~Board();
    Board *getCopy();
    void setFromCopy(Board *info);
    bool hasHappySignature();
    inline void setChipOpen(bool open) { m_chipOpen = open; }
    inline bool isChipOpen() const { return m_chipOpen; }
    inline void setTranslatorActive(bool active) { m_translatorActive = active; }
    inline bool isTranslatorActive() const { return m_translatorActive && (m_translatorState != BOOT_STATE::SECOND_SECTOR_1); }
    inline void setTranslatorState(enum BOOT_STATE state) { m_translatorState = state; }
    inline BOOT_STATE getTranslatorState() const { return m_translatorState; }
    inline void setToolDiskActive(bool active) { m_toolDiskActive = active; }
    inline bool isToolDiskActive() const { return m_toolDiskActive; }
    inline void setLastArchiverUploadCrc16(unsigned short crc16) { m_lastArchiverUploadCrc16 = crc16; }
    inline unsigned short getLastArchiverUploadCrc16() const { return m_lastArchiverUploadCrc16; }
    inline void setLastArchiverSpeed(quint16 speed) { m_lastArchiverSpeed = speed; }
    inline quint16 getLastArchiverSpeed() const { return m_lastArchiverSpeed; }
    inline void setHappyEnabled(bool enabled) { m_happyEnabled = enabled; }
    inline bool isHappyEnabled() const { return m_happyEnabled; }
    inline void setHappy1050(bool is1050) { m_happy1050 = is1050; }
    inline bool isHappy1050() const { return m_happy1050; }
    inline void setHappyPatchInProgress(bool progress) { m_happyPatchInProgress = progress; }
    inline bool isHappyPatchInProgress() const { return m_happyPatchInProgress; }
    inline void setLastHappyUploadCrc16(unsigned short crc16) { m_lastHappyUploadCrc16 = crc16; }
    inline unsigned short getLastHappyUploadCrc16() const { return m_lastHappyUploadCrc16; }
  };
}
#endif//RESPEQT_BOARD_H
