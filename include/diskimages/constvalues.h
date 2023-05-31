#ifndef RESPEQT_CONSTVALUES
#define RESPEQT_CONSTVALUES

#include <QtGlobal>

namespace DiskImages {
  // sector position in track for single and enhanced density
  static constexpr quint16 ATX_SECTOR_POSITIONS_SD[] = {810, 2165, 3519, 4866, 6222, 7575, 8920, 10281, 11632, 12984, 14336, 15684, 17028, 20115, 21456, 22806, 24160, 25500};
  static constexpr quint16 ATX_SECTOR_POSITIONS_ED[] = {540, 1443, 2346, 3244, 4148, 5050, 5946, 6854, 7754, 8656, 9557, 10456, 11352, 13410, 14304, 15204, 16106, 17000, 17898, 18796, 19694, 20592, 21490, 22388, 23286, 24184};
  // pattern to write when Super Archiver formats a track with $08 as a fill byte in a sector ($08 is interpreted as the CRC byte by FDC)

  static constexpr quint8 FDC_CRC_PATTERN[] = {0x08, 0x60, 0x07};

  static constexpr quint8 ARCHIVER_SPEED_CHECK[] = {
          0x31, 0x74, 0x02, 0x00, 0x68, 0xF8, 0xA9, 0x00, 0x85, 0x8E, 0xA9, 0x01, 0x85, 0x8F, 0xAD, 0x00,
          0x04, 0x30, 0x16, 0x20, 0xF2, 0xF9, 0x20, 0x2B, 0x10, 0x20, 0x2E, 0x10, 0x20, 0x2E, 0x10, 0xAD,
          0x96, 0x02, 0x49, 0xFF, 0x8D, 0xF8, 0x10, 0x18, 0x60, 0x38, 0x60, 0x6C, 0x96, 0xF2, 0x20, 0x52,
          0xF9, 0xA9, 0xFF, 0x8D, 0x9F, 0x02, 0xA9, 0x88, 0x8D, 0x00, 0x04, 0xAD, 0x9C, 0x02, 0x2C, 0x80,
          0x02, 0x50, 0x06, 0x10, 0xF6, 0x20, 0x52, 0xF9, 0x60, 0xAD, 0x00, 0x04, 0x29, 0x01, 0xD0, 0xEB,
          0x68, 0x68, 0x20, 0x52, 0xF9, 0x20, 0x1E, 0xF7, 0x20, 0x3D, 0xF7, 0x38, 0x60, 0xA9, 0x5E, 0xA2,
          0x64, 0x20, 0x37, 0x63, 0x20, 0x49, 0x65, 0xA9, 0x85, 0xA2, 0x64, 0x20, 0x37, 0x63, 0xA5, 0x54,
          0x8D, 0x79, 0x68, 0xA9, 0x00, 0x8D, 0x59, 0x69, 0xA9, 0x31, 0x8D, 0x00, 0x03, 0xAD, 0x59, 0x64,
          0x8D, 0x01, 0x03, 0xA9, 0x75, 0x8D, 0x02, 0x03, 0xA9, 0x80, 0x8D, 0x03, 0x03, 0xA9, 0x84, 0x8D,
          0x04, 0x03, 0xA9, 0x66, 0x8D, 0x05, 0x03, 0xA9, 0x0A, 0x8D, 0x06, 0x03, 0xA9, 0x00, 0x8D, 0x08,
          0x03, 0xA9, 0x01, 0x8D, 0x09, 0x03, 0x20, 0x59, 0xE4, 0x10, 0x0A, 0xC0, 0x90, 0xD0, 0x03, 0x4C,
          0x8D, 0x69, 0x4C, 0x5A, 0x69, 0xA9, 0x74, 0x8D, 0x02, 0x03, 0xA9, 0x40, 0x8D, 0x03, 0x03, 0xA9,
          0x00, 0x8D, 0x04, 0x03, 0xA9, 0x5F, 0x8D, 0x05, 0x03, 0x20, 0x59, 0xE4, 0x30, 0xE4, 0xAD, 0xF8,
          0x5F, 0x85, 0xD4, 0xA9, 0x00, 0x85, 0xD5, 0x20, 0xAA, 0xD9, 0x20, 0xB6, 0xDD, 0xA0, 0x05, 0xA9,
          0x00, 0x99, 0x71, 0x68, 0x88, 0x10, 0xFA, 0xA2, 0x5E, 0xA0, 0x68, 0x20, 0x89, 0xDD, 0x20, 0x28,
          0xDB, 0x20, 0xD2, 0xD9, 0xA5, 0xD4, 0x8D, 0x77, 0xCC, 0xA5, 0xD5, 0x8D, 0x78, 0x68, 0x20, 0xAA};

  static constexpr quint8 ARCHIVER_DIAGNOSTIC[] = {
          0x31, 0x74, 0x02, 0x00, 0x30, 0x8D, 0xFF, 0x10, 0x49, 0xFF, 0xAA, 0x20, 0x37, 0x10, 0xD0, 0x25,
          0xAE, 0xFF, 0x10, 0x20, 0x37, 0x10, 0xD0, 0x1D, 0xEE, 0x38, 0x10, 0xEE, 0x3B, 0x10, 0xEE, 0x3E,
          0x10, 0xD0, 0xDD, 0xEE, 0x39, 0x10, 0xEE, 0x3C, 0x10, 0xEE, 0x3F, 0x10, 0xAD, 0x3F, 0x10, 0xC9,
          0x18, 0x90, 0xCD, 0xB0, 0x0C, 0x38, 0x60, 0xAD, 0x00, 0x18, 0x8E, 0x00, 0x18, 0xEC, 0x00, 0x18,
          0x60, 0xA9, 0x00, 0x8D, 0xF8, 0x10, 0x8D, 0xF9, 0x10, 0xA8, 0xB9, 0x00, 0x20, 0x18, 0x6D, 0xF8,
          0x10, 0x8D, 0xF8, 0x10, 0xAD, 0xF9, 0x10, 0x69, 0x00, 0x8D, 0xF9, 0x10, 0xC8, 0xD0, 0xEB, 0xEE,
          0x4C, 0x10, 0xAD, 0x4C, 0x10, 0xC9, 0x20, 0x90, 0xE1, 0xAD, 0x00, 0xF1, 0x8D, 0xFA, 0x10, 0xAD,
          0x01, 0xF1, 0x8D, 0xFB, 0x10, 0x18, 0x60, 0x45, 0x3A, 0x9B, 0x20, 0x52, 0x10, 0xD0, 0x1F, 0x8D,
          0xFF, 0x10, 0x49, 0xFF, 0xAA, 0x20, 0x52, 0x10, 0xD0, 0x14, 0xAE, 0xFF, 0x10, 0x20, 0x52, 0x10,
          0xD0, 0x0C, 0xEE, 0x53, 0x10, 0xEE, 0x55, 0x10, 0xEE, 0x57, 0x10, 0x10, 0xDD, 0x2C, 0x38, 0x60,
          0x20, 0x52, 0x10, 0xD0, 0x25, 0x8D, 0xFF, 0x10, 0x49, 0xFF, 0xAA, 0x20, 0x52, 0x10, 0xD0, 0x1A,
          0xAE, 0xFF, 0x10, 0x20, 0x52, 0x10, 0xD0, 0x12, 0xEE, 0x53, 0x10, 0xEE, 0x55, 0x10, 0xEE, 0x57,
          0x10, 0xAD, 0x57, 0x10, 0xC9, 0xF0, 0xD0, 0xD8, 0x18, 0x60, 0x38, 0x60, 0xA5, 0x00, 0x86, 0x00,
          0xE4, 0x00, 0x60, 0x85, 0xCB, 0x86, 0xCC, 0xAD, 0x20, 0x01, 0xC9, 0x50, 0xD0, 0xEE, 0xA9, 0x0B,
          0xA2, 0x00, 0x9D, 0x42, 0x03, 0x8E, 0x48, 0x03, 0x8E, 0x49, 0x03, 0xA0, 0x00, 0xB1, 0xCB, 0xF0,
          0x0C, 0x20, 0x56, 0xE4, 0xE6, 0xCB, 0xD0, 0xF3, 0xB9, 0xC7, 0x01, 0x06, 0x63, 0x60, 0x7D, 0xFF};

  static constexpr quint8 ARCHIVER_ADDRESS_CHECK[] = {
          0x31, 0x74, 0x00, 0xE9, 0x68, 0xF8, 0xA5, 0x90};

  // Signature sent by Happy at $0800 to execute code in drive
  static constexpr quint8 HAPPY_SIGNATURE[] = {0x26, 0x11, 0x34, 0x14, 0x15, 0x57, 0x37, 0x85, 0x86};

  static constexpr quint8 HAPPY_RAM_CHECK[] = {
          0x00, 0x00, 0x01, 0x60, 0xE6, 0x80, 0xD0, 0x02, 0xE6, 0x81, 0x60, 0xA9, 0xA9, 0x85, 0x80, 0xA9,
          0x08, 0x85, 0x81, 0x60, 0x49, 0x01, 0xA2, 0x08, 0x91, 0x80, 0xD1, 0x80, 0xD0, 0xD1, 0x18, 0xC9,
          0x00, 0x10, 0x01, 0x38, 0x2A, 0xCA, 0xD0, 0xF0, 0x60, 0x26, 0x11, 0x34, 0x14, 0x15, 0x57, 0x37,
          0x85, 0x86, 0x4C, 0x37, 0x1F, 0x4C, 0x21, 0x08, 0x4C, 0x80, 0x14, 0x4C, 0x36, 0x1D, 0x4C, 0xC3,
          0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0x2D, 0x14, 0x20, 0x77, 0x1C, 0xA9, 0x00, 0x85,
          0x84, 0xA9, 0x14, 0x85, 0x85, 0xA9, 0xFD, 0x85, 0x86, 0xA9, 0x17, 0x85, 0x87, 0xAD, 0xDE, 0x1D,
          0xC9, 0x07, 0xD0, 0x08, 0xA9, 0x01, 0x85, 0x82, 0xA9, 0x9D, 0xD0, 0x06, 0xA9, 0x0C, 0x85, 0x82,
          0xA9, 0xD5, 0x85, 0x83, 0x20, 0x81, 0x08, 0xA9, 0x00, 0x85, 0x84, 0xA9, 0x18, 0x85, 0x85, 0xA9};

  static constexpr quint8 HAPPY_ROM810_CHECK[] = {
          0x38, 0xFA, 0x38, 0xFA, 0xCA, 0xF0, 0xED, 0xE6, 0x93, 0xA5, 0x93, 0xC9, 0x20, 0xD0, 0xC0, 0x60,
          0x38, 0xFA, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  static constexpr quint8 HAPPY_ROM1050_CHECK[] = {
          0x2D, 0x80, 0x43, 0x16, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0xC8, 0xD8, 0xBA, 0x24, 0xB7, 0x7C, 0x8F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD, 0x80, 0xFF,
          0x97, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x09, 0x00, 0x40, 0xCD};

  static constexpr quint8 HAPPY_RAM_EXTENDED_CHECK[] = {
          0x00, 0x00, 0xA9, 0x02, 0xFF, 0x1F, 0xFF, 0x1F, 0x84, 0x18, 0x65, 0x80, 0x85, 0x80, 0xA5, 0x81,
          0x69, 0x00, 0x18, 0x10, 0x01, 0x38, 0x26, 0x80, 0x2A, 0x50, 0x07, 0x38, 0x30, 0x01, 0x18, 0x26,
          0x80, 0x2A, 0x49, 0x12, 0x85, 0x81, 0xA5, 0x80, 0x49, 0x5A, 0x85, 0x80, 0xE6, 0x84, 0xD0, 0x02,
          0xE6, 0x85, 0xA5, 0x84, 0xC5, 0x86, 0xD0, 0xCF, 0xA5, 0x85, 0xC5, 0x87, 0xD0, 0xC9, 0xA5, 0x80,
          0xC5, 0x82, 0xD0, 0xBA, 0xA5, 0x81, 0xC5, 0x83, 0xD0, 0xB4, 0x60, 0x26, 0x11, 0x34, 0x14, 0x15,
          0x57, 0x37, 0x85, 0x86, 0x4C, 0x37, 0x1F, 0x4C, 0x21, 0x08, 0x4C, 0x80, 0x14, 0x4C, 0x36, 0x1D,
          0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0x2D, 0x14, 0x20, 0x77, 0x1C, 0xA9,
          0x01, 0x85, 0x83, 0xA9, 0x00, 0x85, 0x86, 0xA9, 0x56, 0x85, 0x87, 0xA9, 0x5D, 0x85, 0x88, 0xA2};

  static constexpr quint8 HAPPY_DIAGNOSTIC[] = {
          0x00, 0x00, 0x00, 0x02, 0x03, 0xD0, 0x01, 0x01, 0x05, 0xF8, 0xF0, 0x1F, 0xCC, 0x9C, 0x03, 0xD0,
          0x20, 0xE4, 0x86, 0x90, 0x22, 0xE4, 0x88, 0xB0, 0x24, 0x98, 0xF0, 0x08, 0x88, 0xA6, 0x87, 0x86,
          0x86, 0x4C, 0x00, 0x09, 0x60, 0x85, 0x80, 0xA9, 0x00, 0xF0, 0x16, 0x85, 0x80, 0xA9, 0x01, 0xD0,
          0x10, 0x85, 0x80, 0xA9, 0x02, 0xD0, 0x0A, 0x85, 0x80, 0xA9, 0x03, 0xD0, 0x04, 0x85, 0x80, 0xA9,
          0x04, 0x85, 0x84, 0xAD, 0x9C, 0x03, 0x85, 0x85, 0x86, 0x81, 0x84, 0x82, 0x4C, 0x51, 0x19, 0x26,
          0x11, 0x34, 0x14, 0x15, 0x57, 0x37, 0x85, 0x86, 0x4C, 0x37, 0x1F, 0x4C, 0x21, 0x08, 0x4C, 0x80,
          0x14, 0x4C, 0x36, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0x2D, 0x14,
          0x20, 0x77, 0x1C, 0xAD, 0xF9, 0x1F, 0x20, 0x43, 0x08, 0xA5, 0x90, 0x85, 0x82, 0xA5, 0x91, 0x85};

  static constexpr quint8 HAPPY_SPEED_CHECK[] = {
          0xF7, 0x16, 0xD0, 0xED, 0x4C, 0x67, 0x19, 0x98, 0xC5, 0x03, 0xA0, 0x01, 0xB8, 0x70, 0xF1, 0xCD,
          0x9C, 0x03, 0x2C, 0x80, 0x03, 0x10, 0xF6, 0xC5, 0x03, 0xC8, 0x10, 0xF6, 0x85, 0x81, 0x86, 0x80,
          0x4C, 0x51, 0x19, 0x26, 0x11, 0x34, 0x14, 0x15, 0x57, 0x37, 0x85, 0x86, 0x4C, 0x37, 0x1F, 0x4C,
          0x21, 0x08, 0x4C, 0x80, 0x14, 0x4C, 0x36, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3,
          0x1D, 0x4C, 0x2D, 0x14, 0x20, 0x77, 0x1C, 0x20, 0x8B, 0x08, 0xA0, 0x00, 0xA9, 0x00, 0x91, 0x80,
          0x20, 0x75, 0x08, 0x90, 0xF7, 0x20, 0x8B, 0x08, 0xB1, 0x80, 0xCD, 0x74, 0x08, 0xD0, 0x33, 0x20,
          0x94, 0x08, 0xAD, 0x74, 0x08, 0x49, 0xFF, 0x20, 0x94, 0x08, 0xAD, 0x74, 0x08, 0x49, 0xFF, 0x91,
          0x80, 0x20, 0x75, 0x08, 0x90, 0xE2, 0xAD, 0x74, 0x08, 0x49, 0xFF, 0x8D, 0x74, 0x08, 0xD0, 0xD5};

  static constexpr quint8 HAPPY_RW_CHECK[] = {
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  static constexpr quint8 HAPPY_BACKUP_BUF1[] = {
          0x99, 0x90, 0x44, 0x77, 0x85, 0x00, 0xAD, 0x9C, 0x03, 0xC9, 0x8E, 0x90, 0x13, 0x2C, 0x80, 0x03,
          0x10, 0xF4, 0xA5, 0x03, 0xCA, 0xD0, 0xF6, 0x24, 0x00, 0xF0, 0xFC, 0xA5, 0x00, 0x85, 0x81, 0x60,
          0xA9, 0x45, 0x4C, 0x53, 0x19, 0xAD, 0x2D, 0x06, 0x10, 0x38, 0x20, 0xF5, 0x26, 0x9B, 0x9B, 0x43,
          0x41, 0x4E, 0x4E, 0x4F, 0x54, 0x20, 0x44, 0x4F, 0x20, 0x54, 0x48, 0x49, 0x53, 0x20, 0x57, 0x49,
          0x54, 0x48, 0x20, 0x53, 0x54, 0x41, 0x4E, 0x44, 0x41, 0x52, 0x44, 0x9B, 0x52, 0x41, 0x4D, 0x20,
          0x44, 0x49, 0x53, 0x4B, 0x20, 0x45, 0x4E, 0x41, 0x42, 0x4C, 0x45, 0x44, 0x9B, 0x9B, 0x00, 0x4C,
          0xC6, 0x26, 0x20, 0x17, 0x29, 0xA2, 0x01, 0xBD, 0x28, 0x06, 0x10, 0x03, 0xE8, 0xD0, 0xF8, 0x8A,
          0x8E, 0x01, 0x03, 0x8E, 0x8D, 0x29, 0x18, 0x69, 0x30, 0x8D, 0x78, 0x2C, 0x8D, 0xA4, 0x2C, 0x20};

  static constexpr quint8 HAPPY_BACKUP_BUF2[] = {
          0x90, 0x44, 0x34, 0x14, 0x15, 0x57, 0x37, 0x85, 0x86, 0x4C, 0x37, 0x1F, 0x4C, 0x21, 0x08, 0x4C,
          0x80, 0x14, 0x4C, 0x36, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0x2D,
          0x14, 0x20, 0x77, 0x1C, 0xAD, 0xFE, 0x17, 0x85, 0x80, 0xAD, 0xFF, 0x17, 0x85, 0x81, 0xA2, 0x00,
          0xCA, 0xD0, 0xFD, 0x4C, 0x51, 0x19, 0x26, 0x11, 0x34, 0x14, 0x15, 0x57, 0x37, 0x85, 0x86, 0x4C,
          0x37, 0x1F, 0x4C, 0x21, 0x08, 0x4C, 0x80, 0x14, 0x4C, 0x36, 0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0xC3,
          0x1D, 0x4C, 0xC3, 0x1D, 0x4C, 0x2D, 0x14, 0x20, 0x77, 0x1C, 0x20, 0xAF, 0x17, 0x20, 0xCD, 0x1F,
          0xAD, 0xDD, 0x01, 0x4A, 0x4A, 0x4A, 0x90, 0x05, 0xA0, 0x2B, 0x20, 0x8B, 0x1C, 0xA9, 0xFF, 0x20,
          0x1B, 0x1C, 0x20, 0x18, 0x1F, 0x20, 0x63, 0x1F, 0xA9, 0x6C, 0x20, 0xE1, 0x18, 0x20, 0x64, 0x08};

  static quint8 HAPPY_INITIAL_SECTOR_LIST[] = {
          0x00, 0x18, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x43, 0xFF, 0x12,
          0x00, 0x01, 0x23, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF9, 0xFB, 0xFD, 0xEE, 0xF0, 0xF2,
          0xF4, 0xF6, 0xF8, 0xFA, 0xFC, 0xFE, 0xED, 0xEF, 0xF1, 0xF3, 0xF5, 0xF7, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#if HAPPY_PATCH_V53
  static constexpr quint8 HAPPY_DOUBLESPEEDREV5_PATCH1[] = {
          0x54, 0x8D, 0x02, 0x03, 0xA9, 0x80, 0x8D, 0x03, 0x03, 0x85, 0xEE, 0x20, 0x48, 0x01, 0xAD, 0x0B,
          0x03, 0xCE, 0x0B, 0x03, 0xC9, 0xED, 0xD0, 0x0B, 0x20, 0x92, 0x06, 0xCE, 0x0A, 0x03, 0xA9, 0xFE,
          0x8D, 0x0B, 0x03, 0x20, 0xD0, 0x06, 0x90, 0xD2, 0x08, 0x20, 0x92, 0x06, 0x28, 0xF0, 0x12, 0xA5,
          0xEC, 0x85, 0xE5, 0xA5, 0xED, 0x85, 0xE6, 0xA5, 0xE0, 0xD0, 0x03, 0x4C, 0xDE, 0x04, 0x4C, 0xFE,
          0x04, 0x20, 0x80, 0x00, 0x44, 0x4F, 0x4E, 0x45, 0x2C, 0x20, 0x00, 0x20, 0x64, 0x06, 0x4C, 0x03,
          0x04, 0x20, 0x80, 0x00, 0x50, 0x55, 0x54, 0x20, 0x44, 0x45, 0x53, 0x20, 0x44, 0x53, 0x4B, 0x20,
          0x44, 0x00, 0x9B, 0x00, 0x20, 0x80, 0x00, 0x48, 0x49, 0x54, 0x20, 0x52, 0x45, 0x54, 0x4E, 0x9B,
          0x9B, 0x00, 0x4C, 0x73, 0x07, 0x18, 0xA5, 0xF0, 0xF0, 0x17, 0xAD, 0x04, 0x03, 0x8D, 0x89, 0x06};

  static constexpr quint8 HAPPY_DOUBLESPEEDREV5_PATCH2[] = {
          0x9B, 0x9B, 0x43, 0x41, 0x4E, 0x4E, 0x4F, 0x54, 0x20, 0x43, 0x4F, 0x4E, 0x54, 0x49, 0x4E, 0x55,
          0x45, 0x00, 0x4C, 0x12, 0x0C, 0xA9, 0x10, 0x8D, 0x04, 0xD2, 0xAD, 0x03, 0x03, 0x10, 0x0F, 0x20,
          0x00, 0x00, 0x20, 0x00, 0x00, 0xA5, 0x30, 0xC9, 0x01, 0xD0, 0xE7, 0x4C, 0x00, 0x00, 0x4C, 0x00,
          0x00, 0xA5, 0xE1, 0x85, 0xEE, 0xA5, 0xE2, 0x85, 0xEF, 0x4C, 0x4F, 0x05, 0xA0, 0x54, 0x20, 0x00,
          0x13, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xA9, 0x00, 0x85, 0xF3, 0x85, 0xF1, 0xAD, 0xE5, 0x02,
          0x38, 0xE9, 0x15, 0x85, 0xE7, 0xAD, 0xE6, 0x02, 0xE9, 0x08, 0x85, 0xE8, 0xA5, 0xE7, 0x0A, 0xA5,
          0xE8, 0x2A, 0x85, 0xE7, 0xA9, 0x00, 0x2A, 0x85, 0xE8, 0xA9, 0x00, 0x85, 0xE2, 0xA9, 0x01, 0x85,
          0xE1, 0xA9, 0x02, 0x85, 0xE4, 0xA9, 0xD0, 0x85, 0xE3, 0xA9, 0x00, 0x85, 0xF0, 0xA0, 0x00, 0xA2};

  static constexpr quint8 HAPPY_DOUBLESPEEDREV5_PATCH3[] = {
          0xB9, 0x33, 0x12, 0x99, 0x7F, 0x00, 0x88, 0xD0, 0xF7, 0xA2, 0x0B, 0xBD, 0x39, 0x13, 0x9D, 0x00,
          0x03, 0xCA, 0x10, 0xF7, 0xA9, 0xE0, 0x8D, 0x7E, 0x13, 0x20, 0x59, 0xE4, 0xAE, 0x3A, 0x13, 0xAD,
          0x7E, 0x13, 0x9D, 0x40, 0x01, 0xCA, 0xF0, 0x05, 0xCE, 0x3A, 0x13, 0xD0, 0xDC, 0xA2, 0x2A, 0xBD,
          0x45, 0x13, 0x9D, 0x48, 0x01, 0xCA, 0x10, 0xF7, 0x60, 0x31, 0x04, 0x53, 0x40, 0x7C, 0x13, 0x0F,
          0x00, 0x04, 0x00, 0xCD, 0xAB, 0xA9, 0x04, 0x85, 0xF6, 0xAD, 0x03, 0x03, 0x85, 0xF7, 0x20, 0x63,
          0x01, 0x30, 0x01, 0x60, 0xA5, 0xF7, 0x8D, 0x03, 0x03, 0xC6, 0xF6, 0xD0, 0xF1, 0x4C, 0x3F, 0x07,
          0xAC, 0x01, 0x03, 0xB9, 0x40, 0x01, 0xC9, 0xEB, 0xF0, 0x03, 0x4C, 0x89, 0x07, 0x4C, 0x59, 0xE4,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

  static constexpr quint8 HAPPY_DOUBLESPEEDREV7_PATCH1[] = {
          0x8E, 0x32, 0x41, 0x8C, 0x33, 0x41, 0x8E, 0x8A, 0x41, 0x8C, 0x8B, 0x41, 0x20, 0x00, 0x4C, 0x8E,
          0xE5, 0x40, 0x8C, 0xE6, 0x40, 0x8E, 0x7B, 0x41, 0x8C, 0x7C, 0x41, 0xA8, 0x10, 0x49, 0xA5, 0xF3,
          0x24, 0xF3, 0x50, 0x05, 0x20, 0x81, 0x40, 0x30, 0x49, 0x24, 0xF4, 0x50, 0x11, 0xA9, 0x64, 0x8D,
          0xD9, 0x41, 0xA5, 0xF4, 0x20, 0x81, 0x40, 0x30, 0x39, 0xA9, 0x60, 0x8D, 0xD9, 0x41, 0xA9, 0xFF,
          0x8D, 0x9A, 0x4B, 0xA5, 0xF1, 0xC9, 0x03, 0x90, 0x2E, 0xA5, 0xF5, 0x24, 0xF5, 0x50, 0x05, 0x20,
          0x81, 0x40, 0x30, 0x1E, 0xA5, 0xF1, 0xC9, 0x04, 0x90, 0x1D, 0xA5, 0xF6, 0x24, 0xF6, 0x50, 0x17,
          0x20, 0x81, 0x40, 0x30, 0x0D, 0x10, 0x10, 0xA5, 0xF1, 0x24, 0xF1, 0x50, 0x06, 0x20, 0x81, 0x40,
          0x10, 0x01, 0x60, 0xA5, 0xF3, 0xF0, 0x04, 0xA9, 0x00, 0x10, 0xF7, 0xA5, 0xF5, 0x24, 0xF5, 0x50};

  static constexpr quint8 HAPPY_DOUBLESPEEDREV7_PATCH2[] = {
          0x8D, 0x9C, 0x4B, 0x8E, 0x78, 0x4C, 0x8C, 0x79, 0x4C, 0xA2, 0x0B, 0xBD, 0x00, 0x03, 0x9D, 0x50,
          0x01, 0xCA, 0x10, 0xF7, 0xA2, 0x0B, 0xBD, 0x5C, 0x4C, 0x9D, 0x00, 0x03, 0xCA, 0x10, 0xF7, 0xA9,
          0xE0, 0x8D, 0x82, 0x4C, 0x20, 0x59, 0xE4, 0xAE, 0x5D, 0x4C, 0xAD, 0x82, 0x4C, 0x9D, 0x40, 0x01,
          0xCA, 0xF0, 0x05, 0xCE, 0x5D, 0x4C, 0xD0, 0xDC, 0xA2, 0x0B, 0xBD, 0x50, 0x01, 0x9D, 0x00, 0x03,
          0xCA, 0x10, 0xF7, 0xA2, 0x14, 0xBD, 0x68, 0x4C, 0x9D, 0x48, 0x01, 0xCA, 0x10, 0xF7, 0xA2, 0x48,
          0xA0, 0x01, 0x8E, 0x1D, 0x1E, 0x8C, 0x1E, 0x1E, 0xAD, 0x9C, 0x4B, 0x60, 0x31, 0x04, 0x53, 0x40,
          0x80, 0x4C, 0x0F, 0x00, 0x04, 0x00, 0xCD, 0xAB, 0x2C, 0x03, 0x03, 0x10, 0x0A, 0xAC, 0x01, 0x03,
          0xB9, 0x40, 0x01, 0xC9, 0xEB, 0xF0, 0x03, 0x4C, 0xAA, 0xAA, 0x4C, 0x59, 0xE4, 0x00, 0x00, 0x00};


}
#endif