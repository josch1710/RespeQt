#ifndef CLASSIC_STYLESHEET_H
#define CLASSIC_STYLESHEET_H

#include "stylesheet.h"

namespace gui {
    namespace stylesheets {

        /**
         * @brief ClassicStylesheet provides a traditional UI with subtle blue accents
         */
        class ClassicStylesheet: public Stylesheet
        {
        public:
            ClassicStylesheet();

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

#endif  // CLASSIC_STYLESHEET_H
