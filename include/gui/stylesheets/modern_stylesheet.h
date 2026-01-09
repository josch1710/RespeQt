#ifndef MODERN_STYLESHEET_H
#define MODERN_STYLESHEET_H

#include "stylesheet.h"

namespace gui {
    namespace stylesheets {

        /**
         * @brief ModernStylesheet provides a clean, flat design with blue accents
         */
        class ModernStylesheet: public Stylesheet
        {
        public:
            ModernStylesheet();

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

#endif  // MODERN_STYLESHEET_H
