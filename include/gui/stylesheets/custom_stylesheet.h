#ifndef CUSTOM_STYLESHEET_H
#define CUSTOM_STYLESHEET_H

#include "stylesheet.h"

namespace gui {
    namespace stylesheets {
        /**
         * @brief CustomStylesheet provides a user-defined stylesheet
         */
        class CustomStylesheet: public Stylesheet
        {
        public:
            CustomStylesheet();
            ~CustomStylesheet() override;

            void initializeIconMap() override;
        };
    }
}  // namespace gui

#endif  // CUSTOM_STYLESHEET_H
