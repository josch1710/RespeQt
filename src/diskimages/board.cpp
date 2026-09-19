
#include "diskimages/board.h"

namespace DiskImages {
  /* Board (Happy or Super Archiver) */

  Board::Board()
  {
    m_chipOpen = false;
    m_happyEnabled = false;
    m_happy1050 = false;
    m_lastArchiverUploadCrc16 = 0;
    m_lastHappyUploadCrc16 = 0;
    m_happyPatchInProgress = false;
    m_translatorActive = false;
    m_translatorState = BOOT_STATE::NOT_BOOTED;
    m_toolDiskActive = false;
    m_lastArchiverSpeed = 0;
  }

  Board::~Board() = default;

  Board *Board::getCopy() const
  {
    const auto copy {new Board()};
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

  void Board::setFromCopy(const Board *other) { // TODO Copy ctor / operator ??
    m_chipOpen = other->m_chipOpen;
    memcpy(m_chipRam, other->m_chipRam, sizeof(m_chipRam));
    m_lastArchiverUploadCrc16 = other->m_lastArchiverUploadCrc16;
    m_trackData.clear();
    m_trackData.append(other->m_trackData);
    m_lastArchiverSpeed = other->m_lastArchiverSpeed;
    m_happyEnabled = other->m_happyEnabled;
    m_happy1050 = other->m_happy1050;
    m_happyRam.clear();
    m_happyRam.append(other->m_happyRam);
    m_lastHappyUploadCrc16 = other->m_lastHappyUploadCrc16;
    m_happyPatchInProgress = other->m_happyPatchInProgress;
    m_translatorActive = other->m_translatorActive;
    m_translatorState = other->m_translatorState;
    m_toolDiskActive = other->m_toolDiskActive;
  }

  bool Board::hasHappySignature() {
    const auto ram {reinterpret_cast<unsigned char*>(m_happyRam.data())};
    for (unsigned int i = 0; i < sizeof(HAPPY_SIGNATURE); i++) {
      if (ram[i] != HAPPY_SIGNATURE[i]) {
        return false;
      }
    }
    return true;
  }

}