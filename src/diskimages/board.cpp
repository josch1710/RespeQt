
#include "diskimages/board.h"

namespace DiskImages {
  /* Board (Happy or Super Archiver) */

  Board::Board()
      : QObject() {
    m_chipOpen = false;
    m_happyEnabled = false;
    m_happy1050 = false;
    m_lastArchiverUploadCrc16 = 0;
    m_lastHappyUploadCrc16 = 0;
    m_happyPatchInProgress = false;
    m_translatorActive = false;
    m_translatorState = BOOT_STATE::NOT_BOOTED;
    m_toolDiskActive = false;
  }

  Board::~Board() {
  }

  Board *Board::getCopy() {
    auto copy = new Board();
    copy->m_chipOpen = m_chipOpen;
    memcpy(copy->m_chipRam, m_chipRam, sizeof(m_chipRam));
    copy->m_lastArchiverUploadCrc16 = m_lastArchiverUploadCrc16;
    copy->m_trackData.append(m_trackData);
    copy->m_lastArchiverSpeed = m_lastArchiverSpeed;
    copy->m_happyEnabled = m_happyEnabled;
    copy->m_happy1050 = m_happy1050;
    copy->m_happyRam.append(m_happyRam);
    copy->m_lastHappyUploadCrc16 = m_lastHappyUploadCrc16;
    copy->m_happyPatchInProgress = m_happyPatchInProgress;
    copy->m_translatorActive = false;
    copy->m_translatorState = m_translatorState;
    copy->m_toolDiskActive = false;
    return copy;
  }

  void Board::setFromCopy(Board *copy) {
    m_chipOpen = copy->m_chipOpen;
    memcpy(m_chipRam, copy->m_chipRam, sizeof(m_chipRam));
    m_lastArchiverUploadCrc16 = copy->m_lastArchiverUploadCrc16;
    m_trackData.clear();
    m_trackData.append(copy->m_trackData);
    m_lastArchiverSpeed = copy->m_lastArchiverSpeed;
    m_happyEnabled = copy->m_happyEnabled;
    m_happy1050 = copy->m_happy1050;
    m_happyRam.clear();
    m_happyRam.append(copy->m_happyRam);
    m_lastHappyUploadCrc16 = copy->m_lastHappyUploadCrc16;
    m_happyPatchInProgress = copy->m_happyPatchInProgress;
    m_translatorActive = copy->m_translatorActive;
    m_translatorState = copy->m_translatorState;
    m_toolDiskActive = copy->m_toolDiskActive;
  }

  bool Board::hasHappySignature() {
    auto ram = (unsigned char *) m_happyRam.data();
    for (unsigned int i = 0; i < sizeof(HAPPY_SIGNATURE); i++) {
      if (ram[i] != HAPPY_SIGNATURE[i]) {
        return false;
      }
    }
    return true;
  }

}