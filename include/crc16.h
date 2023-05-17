//
// Crc16 class
// (c) 2016 Eric BACHER
//

#ifndef CRC16_HPP
#define CRC16_HPP 1

// This class computes CRC16.
class Crc16 {

private:
  unsigned short m_crc;

public:
  // constructor and destructor
  Crc16();
  virtual ~Crc16() {}

  virtual unsigned short GetCrc(void) { return m_crc; }

  virtual void Reset(void);
  virtual unsigned char Add(unsigned char data);
};

#endif
