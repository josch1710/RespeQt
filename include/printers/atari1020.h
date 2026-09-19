#ifndef ATARI1020_H
#define ATARI1020_H

#include "atariprinter.h"

enum AUTOMATA_STATES {
  AUTOMATA_START = 0,
  AUTOMATA_END,
  AUTOMATA_FIRST_NUM,
  AUTOMATA_FIRST_INT,
  AUTOMATA_COMA,
  AUTOMATA_SECOND_NUM,
  AUTOMATA_SECOND_INT,
  AUTOMATA_SECOND_COMA,
  AUTOMATA_THIRD_NUM,
  AUTOMATA_THIRD_INT,
  AUTOMATA_TEXT
};

struct GRAPHICS_COMMAND {
  unsigned char command;        // command character
  int parameters;               // number of parameters
  bool repeat;                  // allows a list of parameters to repeat the same command using the ';' character
  AUTOMATA_STATES automata;// initial state of the automata for this command
};

namespace Printers {
  class Atari1020 : public AtariPrinter {
  public:
    explicit Atari1020(const SioWorkerPtr &sio);

    void handleCommand(quint8 command, quint8 aux1, quint8 aux2) override;
    bool handleBuffer(const QByteArray &buffer, unsigned int len) override;

  protected:
    QRectF printerDimension() const override;

  public:
    // void setupFont() override;
    // void setupOutput() override;

    QRectF getSceneRect() const override;

    static QString typeName() {
      return "Atari 1020";
    }

  protected:
    bool mEsc{false};
    bool mStartOfLogicalLine{true};
    bool mGraphicsMode{false};
    int mTextOrientation{0};
    QByteArray mPrintText{""};
    AUTOMATA_STATES mAutomataState{AUTOMATA_START};
    unsigned char mCurrentCommand{};
    bool mRepeatAllowed{false};
    int mParametersExpected{0};
    bool mFirstNegative{false};
    bool mSecondNegative{false};
    bool mThirdNegative{false};
    QByteArray mFirstNumber;
    QByteArray mSecondNumber;
    QByteArray mThirdNumber;

    void executeGraphicsCommand();
    void resetGraphics();
    void executeAndRepeatCommand();
    bool checkGraphicsCommand(unsigned char b);
    void handleGraphicsCodes(unsigned char b);
    bool handlePrintableCodes(unsigned char b);
    //bool handleGraphicsMode(const QByteArray &buffer, unsigned int len, unsigned int &i);
    int getFirstNumber(const int defaultValue = 0) { return getNumber(mFirstNumber, mFirstNegative, defaultValue); }
    int getSecondNumber(const int defaultValue = 0) { return getNumber(mSecondNumber, mSecondNegative, defaultValue); }
    int getThirdNumber(const int defaultValue = 0) { return getNumber(mThirdNumber, mThirdNegative, defaultValue); }
    int getNumber(const QString& number, bool negative, int defaultValue = 0);
    bool drawAxis(bool xAxis, int size, int count);
    bool drawText();
  };
}// namespace Printers
#endif// ATARI1020_H
