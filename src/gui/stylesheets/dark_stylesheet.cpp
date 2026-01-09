#include "include/gui/stylesheets/dark_stylesheet.h"

namespace gui {
    namespace stylesheets {

        // DarkStylesheet implementation
        DarkStylesheet::DarkStylesheet()
        {
            generateStylesheet();
        }

        void DarkStylesheet::generateStylesheet()
        {
            QString combinedStylesheet = R"(
                /* Dark Stylesheet - Modern dark theme with blue accents */
            )";

            combinedStylesheet += getBaseStyles();
            combinedStylesheet += getMenuStyles();
            combinedStylesheet += getButtonStyles();
            combinedStylesheet += getFormStyles();
            combinedStylesheet += getTabStyles();
            combinedStylesheet += getDriveStyles();
            combinedStylesheet += getScrollbarStyles();

            setContent(combinedStylesheet);
        }

        QString DarkStylesheet::getBaseStyles() const
        {
            return R"(
                QMainWindow, QDialog {
                    background-color: #303030;
                    color: #e0e0e0;
                    font-family: "Segoe UI", Arial, sans-serif;
                }
            )";
        }

        QString DarkStylesheet::getMenuStyles() const
        {
            return R"(
                QMenuBar {
                    background-color: #383838;
                    color: #e0e0e0;
                    border-bottom: 1px solid #505050;
                }

                QMenuBar::item {
                    background-color: transparent;
                    padding: 6px 12px;
                }

                QMenuBar::item:selected {
                    background-color: #505050;
                    border-radius: 4px;
                }

                QMenu {
                    background-color: #383838;
                    border: 1px solid #505050;
                    color: #e0e0e0;
                }

                QMenu::item {
                    padding: 6px 30px 6px 30px;
                }

                QMenu::item:selected {
                    background-color: #505050;
                }

                QToolBar {
                    background-color: #383838;
                    border-bottom: 1px solid #505050;
                    spacing: 4px;
                    padding: 4px;
                }

                QToolButton {
                    background-color: transparent;
                    border: none;
                    border-radius: 4px;
                    padding: 4px;
                    color: #e0e0e0;
                }

                QToolButton:hover {
                    background-color: #505050;
                }

                QToolButton:pressed {
                    background-color: #606060;
                }

                QStatusBar {
                    background-color: #383838;
                    color: #a0a0a0;
                    border-top: 1px solid #505050;
                }
            )";
        }

        QString DarkStylesheet::getButtonStyles() const
        {
            return R"(
                QPushButton {
                    background-color: #505050;
                    color: #e0e0e0;
                    border: none;
                    border-radius: 4px;
                    padding: 6px 16px;
                    font-weight: bold;
                }

                QPushButton:hover {
                    background-color: #606060;
                }

                QPushButton:pressed {
                    background-color: #707070;
                }
            )";
        }

        QString DarkStylesheet::getFormStyles() const
        {
            return R"(
                QLineEdit, QTextEdit, QComboBox {
                    background-color: #404040;
                    border: 1px solid #505050;
                    border-radius: 4px;
                    padding: 6px;
                    color: #e0e0e0;
                    selection-background-color: #707070;
                    selection-color: #ffffff;
                }

                QLineEdit:focus, QTextEdit:focus, QComboBox:focus {
                    border: 1px solid #707070;
                }

                QCheckBox {
                    spacing: 6px;
                    color: #e0e0e0;
                }

                QCheckBox::indicator {
                    width: 18px;
                    height: 18px;
                    border-radius: 3px;
                }

                QCheckBox::indicator:unchecked {
                    border: 2px solid #505050;
                    background-color: #383838;
                }

                QCheckBox::indicator:checked {
                    border: 2px solid #4da6ff;
                    background-color: #4da6ff;
                }

                QRadioButton {
                    spacing: 6px;
                    color: #e0e0e0;
                }

                QRadioButton::indicator {
                    width: 18px;
                    height: 18px;
                }

                QRadioButton::indicator:unchecked {
                    border: 2px solid #505050;
                    border-radius: 9px;
                    background-color: #383838;
                }

                QRadioButton::indicator:checked {
                    border: 2px solid #4da6ff;
                    border-radius: 9px;
                    background-color: #4da6ff;
                }

                QProgressBar {
                    border: none;
                    border-radius: 4px;
                    background-color: #505050;
                    padding: 1px;
                    text-align: center;
                    height: 8px;
                    color: transparent;
                }

                QProgressBar::chunk {
                    background-color: #4da6ff;
                    border-radius: 4px;
                }
            )";
        }

        QString DarkStylesheet::getTabStyles() const
        {
            return R"(
                QTabWidget::pane {
                    border: 1px solid #505050;
                    background-color: #383838;
                }

                QTabBar::tab {
                    background-color: #303030;
                    border: 1px solid #505050;
                    border-bottom: none;
                    border-top-left-radius: 4px;
                    border-top-right-radius: 4px;
                    padding: 8px 16px;
                    margin-right: 2px;
                    color: #a0a0a0;
                }

                QTabBar::tab:selected {
                    background-color: #383838;
                    color: #e0e0e0;
                }

                QGroupBox {
                    border: 1px solid #505050;
                    border-radius: 4px;
                    margin-top: 1.5ex;
                    padding-top: 1ex;
                    color: #e0e0e0;
                }

                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top center;
                    padding: 0 5px;
                    color: #e0e0e0;
                }
            )";
        }

        QString DarkStylesheet::getDriveStyles() const
        {
            return R"(
                /* Drive Styling */
                QFrame#diskWidget {
                    border: 1px solid #505050;
                    border-radius: 6px;
                    background-color: #383838;
                    padding: 6px;
                }

                QLabel#labelFileName {
                    font-weight: bold;
                    color: #4da6ff;
                }
            )";
        }

        QString DarkStylesheet::getScrollbarStyles() const
        {
            return R"(
                /* Scrollbar Styling */
                QScrollBar:vertical {
                    background-color: #383838;
                    width: 12px;
                    margin: 0px;
                    border-radius: 6px;
                }

                QScrollBar::handle:vertical {
                    background-color: #505050;
                    min-height: 20px;
                    border-radius: 6px;
                }

                QScrollBar::handle:vertical:hover {
                    background-color: #606060;
                }

                QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                    height: 0px;
                }

                QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                    background: none;
                }

                QScrollBar:horizontal {
                    background-color: #383838;
                    height: 12px;
                    margin: 0px;
                    border-radius: 6px;
                }

                QScrollBar::handle:horizontal {
                    background-color: #505050;
                    min-width: 20px;
                    border-radius: 6px;
                }

                QScrollBar::handle:horizontal:hover {
                    background-color: #606060;
                }

                QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                    width: 0px;
                }

                QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
                    background: none;
                }
            )";
        }

        void DarkStylesheet::initializeIconMap()
        {
            
        }

    }
} // namespace gui
