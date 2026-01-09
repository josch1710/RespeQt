#ifndef STYLESHEETMANAGER_H
#define STYLESHEETMANAGER_H

#include <QObject>
#include <QString>
#include <QApplication>

#include "classic_stylesheet.h"
#include "modern_stylesheet.h"
#include "dark_stylesheet.h"
#include "custom_stylesheet.h"

namespace gui {
    namespace stylesheets {

        /**
         * @brief The StylesheetManager manages the various stylesheets of the application.
         *
         * This class is implemented as a singleton and provides access to various
         * predefined stylesheet designs (Classic, Modern, Dark) as well as a custom
         * stylesheet. It also manages the mapping between icon functions and paths.
         */
        class StylesheetManager: public QObject
        {
            Q_OBJECT

        public:
            // Available styles
            enum Style
            {
                ClassicStyle,
                ModernStyle,
                DarkStyle,
                CustomStyle
            };
            
            /**
             * @brief Returns the singleton instance of the StylesheetManager.
             * @return Pointer to the StylesheetManager instance.
             */
            static StylesheetManager *getInstance();

            /**
             * @brief Returns the content of the specified stylesheet.
             * @param style The stylesheet style to return.
             * @return The stylesheet content as a QString.
             */
            QString getStylesheetContent(Style style = ClassicStyle);

            /**
             * @brief Returns a object of the specified stylesheet.
             * @param style The stylesheet style to return.
             * @return The stylesheet content as a Stylesheet.
             */
            StylesheetPtr getStylesheet(Style style = ClassicStyle);

            /**
             * @brief Applies the specified stylesheet to the entire application.
             * @param style The stylesheet style to apply.
             */
            void applyStyleToApplication(QApplication *app, Style style = ClassicStyle);

            /**
             * @brief Sets the content of the custom stylesheet.
             * @param stylesheet The new content of the custom stylesheet.
             */
            void setCustomStylesheet(const QString &stylesheet);

        private:
            StylesheetManager(QObject *parent = nullptr);
            ~StylesheetManager();

            static StylesheetManager *instance;

            // Predefined stylesheets with lazy initialization using Qt smart pointers
            mutable StylesheetPtr m_classicStylesheet;  // Traditional UI with subtle blue accents
            mutable StylesheetPtr m_modernStylesheet;    // Clean, flat design with blue accents
            mutable StylesheetPtr m_darkStylesheet;        // Modern dark theme with blue accents
            mutable StylesheetPtr m_customStylesheet;    // User-defined stylesheet
        };
    }
}  // namespace gui

#endif  // STYLESHEETMANAGER_H
