#include "include/gui/stylesheets/stylesheet.h"
#include <QIcon>
#include <QString>
#include <QApplication>
#include <QDesktopWidget>

namespace gui {
    namespace stylesheets {
        constexpr int Stylesheet::sizes[];

        // Base Stylesheet class implementation
        Stylesheet::Stylesheet()
        {
        }

        Stylesheet::Stylesheet(const QString& content)
            : content(content)
        {
        }

        Stylesheet::~Stylesheet()
        {
        }

        QString Stylesheet::getContent() const
        {
            return content;
        }

        void Stylesheet::setContent(const QString& content)
        {
            this->content = content;
        }

        bool Stylesheet::isEmpty() const
        {
            return content.isEmpty();
        }

        void Stylesheet::append(const QString& content)
        {
            this->content.append(content);
        }

        void Stylesheet::clear()
        {
            content.clear();
        }

        void Stylesheet::generateStylesheet()
        {
            // Base implementation just combines all components
            QString combinedStylesheet;
            combinedStylesheet += getBaseStyles();
            combinedStylesheet += getMenuStyles();
            combinedStylesheet += getButtonStyles();
            combinedStylesheet += getFormStyles();
            combinedStylesheet += getTabStyles();
            combinedStylesheet += getDriveStyles();
            combinedStylesheet += getScrollbarStyles();

            setContent(combinedStylesheet);
        }

        QString Stylesheet::getBaseStyles() const
        {
            return "";
        }

        QString Stylesheet::getMenuStyles() const
        {
            return "";
        }

        QString Stylesheet::getButtonStyles() const
        {
            return "";
        }

        QString Stylesheet::getFormStyles() const
        {
            return "";
        }

        QString Stylesheet::getTabStyles() const
        {
            return "";
        }

        QString Stylesheet::getDriveStyles() const
        {
            return "";
        }

        QString Stylesheet::getScrollbarStyles() const
        {
            return "";
        }

        QString Stylesheet::getIconPath(IconFunction iconFunction)
        {
            return iconPathMap.value(iconFunction, "");
        }

        QIcon Stylesheet::createMultipleSizeIcon(const IconFunction &function)
        {
            QIcon icon;
            /*auto*/QString path{getIconPath(function)};
    
            for(int size : Stylesheet::sizes) {
                QImage image(path);
                QImage scaledImage = image.scaled(QSize(size, size), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                icon.addPixmap(QPixmap::fromImage(scaledImage));
            }
            return icon;
        }

        QSize Stylesheet::calculateScreenScale()
        {
            auto desktopWidget = QApplication::desktop();
            auto scale = desktopWidget->logicalDpiX() / 96.0;
            return QSize(24.0 * scale, 24.0 * scale);
        }
    }  // namespace stylesheets
} // namespace gui
