#ifndef DARK_STYLESHEET_H
#define DARK_STYLESHEET_H

#include "stylesheet.h"

namespace gui {
    namespace stylesheets {
        /**
         * @brief DarkStylesheet provides a modern dark theme with blue accents
         */
        class DarkStylesheet: public Stylesheet
        {
        public:
            DarkStylesheet();

            virtual void generateStylesheet() override;
            virtual QString getBaseStyles() const override;
            virtual QString getMenuStyles() const override;
            virtual QString getButtonStyles() const override;
            virtual QString getFormStyles() const override;
            virtual QString getTabStyles() const override;
            virtual QString getDriveStyles() const override;
            virtual QString getScrollbarStyles() const override;

            void initializeIconMap() override;
        };
    }
}  // namespace gui

#endif  // DARK_STYLESHEET_H
