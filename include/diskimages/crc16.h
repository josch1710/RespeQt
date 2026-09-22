//
// Crc16 class
// (c) 2016 Eric BACHER
//

#ifndef CRC16_HPP
#define CRC16_HPP 1

namespace DiskImages {

// This class computes CRC16.
class Crc16 {

public:
  // constructor and destructor
  Crc16();
  virtual ~Crc16() = default;

  virtual unsigned short GetCrc() { return m_crc; }

  virtual void Reset();
  virtual unsigned char Add(unsigned char data);

private:
  unsigned short m_crc;

};

} // namespace DiskImages

#endif
