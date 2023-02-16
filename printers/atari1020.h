#ifndef ATARI1020_H
#define ATARI1020_H

#include "common/atariprinter.h"
#include "graphicsprimitive.h"

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
    unsigned char command;          // command character
    int parameters;                 // number of parameters
    bool repeat;                    // allows a list of parameters to repeat the same command using the ';' character
    enum AUTOMATA_STATES automata;  // initial state of the automata for this command
};

namespace Printers
{
    class Atari1020 : public AtariPrinter
    {
        Q_OBJECT
    public:
        Atari1020(SioWorkerPtr sio);

        virtual void handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) override;
        virtual bool handleBuffer(const QByteArray &buffer, const unsigned int len) override;

        static QString typeName() {
            return "Atari 1020";
        }

    protected:
        enum class PenColor : int {
            BLACK = 0,
            BLUE = 1,
            GREEN = 2,
            RED = 3
        };

        static const std::map<PenColor, QString> sColorNames;
        bool mEsc;
        bool mStartOfLogicalLine;
        bool mGraphicsMode;
        //QPoint mPenPoint;
        QPen mPen;
        QFont mFont;
        int mTextOrientation;
        enum AUTOMATA_STATES mAutomataState;
        unsigned char mCurrentCommand;
        bool mRepeatAllowed;
        int mParametersExpected;
        bool mFirstNegative;
        bool mSecondNegative;
        bool mThirdNegative;
        QByteArray mFirstNumber;
        QByteArray mSecondNumber;
        QByteArray mThirdNumber;

        void executeGraphicsCommand();
        void resetGraphics();
        void executeAndRepeatCommand();
        bool checkGraphicsCommand(const unsigned char b);
        void handleGraphicsCodes(const unsigned char b);
        bool handlePrintableCodes(const unsigned char b);
        bool handleGraphicsMode(const QByteArray &buffer, const unsigned int len, unsigned int &i);
        inline int getFirstNumber(const int defaultValue = 0) { return getNumber(mFirstNumber, mFirstNegative, defaultValue); }
        inline int getSecondNumber(const int defaultValue = 0) { return getNumber(mSecondNumber, mSecondNegative, defaultValue); }
        inline int getThirdNumber(const int defaultValue = 0) { return getNumber(mThirdNumber, mThirdNegative, defaultValue); }
        int getNumber(const QString number, const bool negative, const int defaultValue = 0);
        bool drawAxis(bool xAxis, int size, int count);
        bool drawText();
        bool newTextLine();
        QPoint computeTextCoordinates(const QPoint point, int orientation);
        virtual void createOutputButtons() override;

        void setOutputWindow(OutputWindowPtr outputWindow) override;
        void applyResizing(QResizeEvent *e) override;

    public slots:
        void changeColor(int index);

    signals:
        void setCursorColor(const QColor &color);
        void setColorSelection(int colorIndex);
    };
}
#endif // ATARI1020_H
