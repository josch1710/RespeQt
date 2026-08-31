#include "printers/atari1029.h"
#include "respeqtsettings.h"
#include <qglobal.h>
#include <utility>

#include <QFontDatabase>
#include <QString>
#include <QFont>
#include <QGraphicsTextItem>

namespace Printers {
  Atari1029::Atari1029(SioWorkerPtr worker)
      : AtariPrinter(std::move(worker))
  {
    mLineHeight = 10.5;
  }

  void Atari1029::setupFont() {
    if (!mIsFontInitialized)
    {
      // Load the font from the resources
      auto fontId{QFontDatabase::addApplicationFont(":/fonts/1029")};

      if (fontId != -1) {
        // Retrieve the name of the loaded font
        auto family{QFontDatabase::applicationFontFamilies(fontId).at(0)};

        // Set the font for the whole application
        QFont font{family};
        // Goal: 80 characters in 480 pixels
        int max_width = 480;
        int max_chars = 80;
        QString testString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"; // Test string

        // Start with a default size and shrink it if necessary
        font.setPointSize(14);

        // Simple loop for adjusting the size (or use a binary search for performance)
        while (font.pointSize() > 1) {
          QGraphicsTextItem* tempItem = new QGraphicsTextItem(testString);
          tempItem->setFont(font);

          // Calculate the width for 80 characters based on the test string
          // (assumption: the test string represents the average width)
          double averageCharWidth = tempItem->boundingRect().width() / testString.length();
          if (averageCharWidth * max_chars > max_width) {
            font.setPointSize(font.pointSize() - 1);
          } else {
            break; // Size fits!
          }
          delete tempItem;
        }

        mFont = font;
        mIsFontInitialized = true;

      } else {
        qDebug() << "Font could not be loaded!";
      }
    }
  }

  QRectF Atari1029::printerDimension() const
  {
    return {0, 0, 480, 0};
  }

  bool Atari1029::handleBuffer(const QByteArray &buffer, const unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
      auto b = static_cast<unsigned char>(buffer.at(static_cast<int>(i)));
      if (mGraphicsMode == GraphicsMode::NOT_GRAPHICS) {
        switch (b) {
          case 13:// CTRL+N could be ESC code
          case 14:// CTRL+O could be ESC code
          case 23:// CTRL+W could be ESC code
          case 24:// CTRL+X could be ESC code
          case 25:// CTRL+Y could be ESC code
          case 26:// CTRL+Z could be ESC code
          case 54:// 6 could be ESC code
          case 57:// 9 could be ESC code
          case 65:// A could be ESC code
            if (mESC) {
              if (!handleEscapedCodes(b)) {
                handlePrintableCodes(b);
              }
            } else {
              handlePrintableCodes(b);
            }
            break;

          case 155:// EOL
          {
            mESC = false;
            setElongatedMode(false); // TODO Check whether it does this and other resets

            if (mGraphicsMode == GraphicsMode::NOT_GRAPHICS)
            {
              // Do a newline in text mode
              mPenPoint.setX(0);
              mPenPoint.setY(mPenPoint.y() + mLineHeight);
            }

            // Play the scene
            mOutputWindow->playScene();

            // Drop the rest of the buffer
            return true;
          }

          case 27:     // ESC could be starting something
            if (mESC) {// ESC from last buffer
              mESC = false;
              handlePrintableCodes(b);
            } else {// No ESC codes from last buffer
              mESC = true;
              if (i + 1 < len) {
                i++;
                b = static_cast<unsigned char>(buffer.at(static_cast<int>(i)));
                if (!handleEscapedCodes(b)) {
                  handlePrintableCodes(b);
                }
              }
            }
            break;

          default:// Everything else
            handlePrintableCodes(b);
            break;
        }
      } else
        handleGraphicsMode(b);
    }
    return true;
  }

  bool Atari1029::handleEscapedCodes(const unsigned char b) {
    // At this time we have seen an ESC.
    switch (b) {
      case 25:// CTRL+Y starts underline mode
      {
        /*if (mOutput->font())
                {
                    mOutput->font()->setUnderline(true);
                    mOutput->applyFont();
                }*/
        mESC = false;
        qDebug() << "!d"
                 << "ESC Underline on";
        return true;
      }
      case 26:// CTRL+Z ends underline mode
      {
        /*if (mOutput->font())
                {
                    mOutput->font()->setUnderline(false);
                    mOutput->applyFont();
                }*/
        mESC = false;
        qDebug() << "!d"
                 << "ESC Underline off";
        return true;
      }
      case 23:// CTRL+W starts international mode
        setInternationalMode(true);
        mESC = false;
        return true;

      case 24:// CTRL+X ends international mode
        setInternationalMode(false);
        mESC = false;
        return true;

      case 13:// CTRL+N starts elongated mode
        setElongatedMode(true);
        mESC = false;
        return true;

      case 14:// CTRL+O ends elongated mode
        setElongatedMode(false);
        mESC = false;
        return true;

      case 54:// 6 sets LPI to 6
        mESC = false;
        mLineHeight = 10.5;
        return true;

      case 57:// 9 sets LPI to 9
        mESC = false;
        mLineHeight = 7;
        return true;

      case 65:// A starts graphics mode
        mGraphicsMode = GraphicsMode::FETCH_MSB;
        mESC = false;
        return true;
    }
    return false;
  }

  bool Atari1029::handlePrintableCodes(const unsigned char b) {
    auto qb{translateAtascii(b & 127)}; // Masking inverse characters.
    auto item{new QGraphicsTextItem(qb)};
    item->setPos(mPenPoint);
    item->setFont(mFont);
    item->setVisible(true);
    item->setDefaultTextColor(Qt::black);
    emit addItem(item);
    qDebug() << "Adding item at: " << mPenPoint;
    mPenPoint.setX(mPenPoint.x() + 6);
    if (mPenPoint.x() > 480)
    {
      mPenPoint.setX(0);
      mPenPoint.setY(mPenPoint.y() + mLineHeight);
    }
    return true;
  }

  void Atari1029::setElongatedMode(bool elongatedMode) {
    mElongatedMode = elongatedMode;
    if (mElongatedMode) {
      //mOutput->calculateFixedFontSize(40);
    } else {
      //mOutput->calculateFixedFontSize(80);
    }
  }

  bool Atari1029::handleGraphicsMode(const unsigned char b) {
    switch (mGraphicsMode) {
      case GraphicsMode::FETCH_MSB:
        // b is the MSB of the count of following columns
        mGraphicsColumns = static_cast<uint16_t>(b << 8);
        mGraphicsMode = GraphicsMode::FETCH_LSB;
        break;

      case GraphicsMode::FETCH_LSB:
        // b is the LSB of the count of following columns
        mGraphicsColumns += b;
        mGraphicsMode = GraphicsMode::PLOT_DOTS;
        break;

      case GraphicsMode::PLOT_DOTS: {
        // Now we fetch the graphics data, until mGraphicsColumns is 0
        // Paint the dots;
        QPoint point(mPenPoint.x(), mPenPoint.y() + 6);
        for(int i = 0; i < 7; i++)
        {
            // Mask the point we want to draw.
            auto line{new QGraphicsLineItem(point.x(), point.y(), point.x(), point.y())};
            line->setPen(QPen(b & (1 << i) ? Qt::black : Qt::white, 1));
            emit addItem(line);
            point.setY(point.y() - 1);
        }
        mGraphicsColumns --;
        mPenPoint.setX(mPenPoint.x() + 1); // Move to next column;

        if (mGraphicsColumns == 0)
        {
            mGraphicsMode = GraphicsMode::NOT_GRAPHICS;
        }
      }
      break;

      case GraphicsMode::NOT_GRAPHICS://Should not happen.
        Q_ASSERT(false);
    }

    return true;
  }
}// namespace Printers
