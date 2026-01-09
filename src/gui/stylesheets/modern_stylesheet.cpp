#include "include/gui/stylesheets/modern_stylesheet.h"

namespace gui {
    namespace stylesheets {

        // ModernStylesheet implementation
        ModernStylesheet::ModernStylesheet()
        {
            generateStylesheet();
            initializeIconMap();
        }

        void ModernStylesheet::generateStylesheet()
        {
            QString combinedStylesheet = R"(
                /* Modern Stylesheet - Clean, flat design with blue accents */
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

        QString ModernStylesheet::getBaseStyles() const
        {
            return R"(
                QMainWindow, QDialog {
                    background-color: #ffffff;
                    color: #333333;
                    font-family: "Segoe UI", Arial, sans-serif;
                }
            )";
        }

        QString ModernStylesheet::getMenuStyles() const
        {
            return R"(
                QMenuBar {
                    background-color: #ffffff;
                    border-bottom: 1px solid #e0e0e0;
                    padding: 2px;
                }

                QMenuBar::item {
                    background-color: transparent;
                    padding: 6px 12px;
                    border-radius: 4px;
                }

                QMenuBar::item:selected {
                    background-color: #f0f0f0;
                }

                QMenu {
                    background-color: #ffffff;
                    border: 1px solid #e0e0e0;
                    border-radius: 4px;
                    padding: 4px;
                }

                QMenu::item {
                    padding: 6px 30px 6px 30px;
                    border-radius: 2px;
                }

                QMenu::item:selected {
                    background-color: #f0f0f0;
                }

                QToolBar {
                    background-color: #ffffff;
                    border-bottom: 1px solid #e0e0e0;
                    padding: 4px;
                    spacing: 4px;
                }

                QToolButton {
                    background-color: transparent;
                    border: 1px solid lightgrey;
                    border-radius: 0px;
                    padding: 4px;
                    margin: 1px;
                }

                QToolButton:hover {
                    background-color: #a0a0a0;
                }

                QToolButton:pressed {
                    background-color: #e0e0e0;
                }

                QStatusBar {
                    background-color: #ffffff;
                    border-top: 1px solid #e0e0e0;
                    color: #666666;
                }
            )";
        }

        QString ModernStylesheet::getButtonStyles() const
        {
            return R"(
                QPushButton {
                    background-color: #4285f4;
                    color: white;
                    border: none;
                    border-radius: 4px;
                    padding: 6px 16px;
                    font-weight: bold;
                }

                QPushButton:hover {
                    background-color: #3275e4;
                }

                QPushButton:pressed {
                    background-color: #2265d4;
                }
            )";
        }

        QString ModernStylesheet::getFormStyles() const
        {
            return R"(
                QLineEdit, QTextEdit, QComboBox {
                    background-color: white;
                    border: 1px solid #e0e0e0;
                    border-radius: 4px;
                    padding: 6px;
                    selection-background-color: #4285f4;
                    selection-color: white;
                }

                QLineEdit:focus, QTextEdit:focus, QComboBox:focus {
                    border: 1px solid #4285f4;
                }

                QCheckBox {
                    spacing: 6px;
                }

                QCheckBox::indicator {
                    width: 18px;
                    height: 18px;
                    border-radius: 3px;
                }

                QCheckBox::indicator:unchecked {
                    border: 2px solid #e0e0e0;
                    background-color: white;
                }

                QCheckBox::indicator:checked {
                    border: 2px solid #4285f4;
                    background-color: #4285f4;
                }

                QRadioButton {
                    spacing: 6px;
                }

                QRadioButton::indicator {
                    width: 18px;
                    height: 18px;
                }

                QRadioButton::indicator:unchecked {
                    border: 2px solid #e0e0e0;
                    border-radius: 9px;
                    background-color: white;
                }

                QRadioButton::indicator:checked {
                    border: 2px solid #4285f4;
                    border-radius: 9px;
                    background-color: #4285f4;
                }

                QProgressBar {
                    border: none;
                    border-radius: 4px;
                    background-color: #f0f0f0;
                    padding: 1px;
                    text-align: center;
                    height: 8px;
                    color: transparent;
                }

                QProgressBar::chunk {
                    background-color: #4285f4;
                    border-radius: 4px;
                }
            )";
        }

        QString ModernStylesheet::getTabStyles() const
        {
            return R"(
                QTabWidget::pane {
                    border: 1px solid #e0e0e0;
                    border-radius: 4px;
                    background-color: white;
                }

                QTabBar::tab {
                    background-color: #f8f8f8;
                    border: 1px solid #e0e0e0;
                    border-bottom: none;
                    border-top-left-radius: 4px;
                    border-top-right-radius: 4px;
                    padding: 8px 16px;
                    margin-right: 2px;
                    color: #666666;
                }

                QTabBar::tab:selected {
                    background-color: white;
                    color: #4285f4;
                }

                QGroupBox {
                    border: 1px solid #e0e0e0;
                    border-radius: 4px;
                    margin-top: 1.5ex;
                    padding-top: 1ex;
                    font-weight: bold;
                    color: #333333;
                }

                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top center;
                    padding: 0 5px;
                    background-color: white;
                }
            )";
        }

        QString ModernStylesheet::getDriveStyles() const
        {
            return R"(
                /* Drive Styling */
                QFrame#diskWidget {
                    border: 1px solid #e0e0e0;
                    border-radius: 6px;
                    background-color: #ffffff;
                    padding: 6px;
                }

                QLabel#labelFileName {
                    font-weight: bold;
                    color: #4285f4;
                    font-size: 11pt;
                }
            )";
        }

        QString ModernStylesheet::getScrollbarStyles() const
        {
            return R"(
                /* Modern Scrollbar Styling */
                QScrollBar:vertical {
                    background-color: #f5f5f5;
                    width: 10px;
                    margin: 0px;
                    border-radius: 5px;
                }

                QScrollBar::handle:vertical {
                    background-color: #dadada;
                    min-height: 20px;
                    border-radius: 5px;
                }

                QScrollBar::handle:vertical:hover {
                    background-color: #c0c0c0;
                }

                QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                    height: 0px;
                }

                QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                    background: none;
                }

                QScrollBar:horizontal {
                    background-color: #f5f5f5;
                    height: 10px;
                    margin: 0px;
                    border-radius: 5px;
                }

                QScrollBar::handle:horizontal {
                    background-color: #dadada;
                    min-width: 20px;
                    border-radius: 5px;
                }

                QScrollBar::handle:horizontal:hover {
                    background-color: #c0c0c0;
                }

                QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                    width: 0px;
                }

                QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
                    background: none;
                }
            )";
        }

        void ModernStylesheet::initializeIconMap()
        {
            // Dateisystem-Funktionen
            iconPathMap[IconFolder]        = ":/iconoir/folder";
            iconPathMap[IconFolderEdit]    = ":/iconoir/folder-settings";
            iconPathMap[IconFolderExplore] = ":/iconoir/folder-search";
            iconPathMap[IconFolderImage]   = ":/iconoir/folder-settings";
            iconPathMap[IconFolderHtml]    = ":/iconoir/folder";
            iconPathMap[IconFile]          = ":/iconoir/file";
            iconPathMap[IconFileText]      = ":/iconoir/file-text";
            iconPathMap[IconFileC]         = ":/iconoir/file-code";
            iconPathMap[IconImageFile]     = ":/iconoir/media-image";

            // Laufwerk-Funktionen
            iconPathMap[IconDrive]         = ":/iconoir/database";
            iconPathMap[IconDriveAdd]      = ":/iconoir/database-settings";
            iconPathMap[IconDriveDelete]   = ":/iconoir/database-remove";
            iconPathMap[IconDriveRename]   = ":/iconoir/database-restore";
            iconPathMap[IconDriveEject]    = ":/iconoir/eject";
            iconPathMap[IconDriveGo]       = ":/iconoir/play";
            iconPathMap[IconDriveOpen]     = ":/iconoir/database-export";
            iconPathMap[IconDriveClose]    = ":/iconoir/database-backup";
            iconPathMap[IconDriveSave]     = ":/iconoir/save-floppy-disk";
            iconPathMap[IconDriveDisk]     = ":/iconoir/cd";
            iconPathMap[IconFloppyMount]   = ":/iconoir/save-floppy-disk";
            iconPathMap[IconFloppyUnmount] = ":/iconoir/delete-circle";
            iconPathMap[IconFloppyFront]   = ":/iconoir/save-floppy-disk";
            iconPathMap[IconFloppyBack]    = ":/iconoir/save-floppy-disk";
            iconPathMap[IconMediaFloppy]   = ":/iconoir/save-floppy-disk";
            iconPathMap[IconMediaTape]     = ":/iconoir/media-video";
            iconPathMap[IconCassette]      = ":/iconoir/media-video";
            iconPathMap[IconAutoSave]      = ":/iconoir/auto-save";

            // Allgemeine Funktionen
            iconPathMap[IconConnect]           = ":/iconoir/plug-connect";
            iconPathMap[IconDisconnect]        = ":/iconoir/plug-disconnect";
            iconPathMap[IconNetworkConnect]    = ":/iconoir/internet";
            iconPathMap[IconNetworkDisconnect] = ":/iconoir/no-link";
            iconPathMap[IconNetwork]           = ":/iconoir/internet";
            iconPathMap[IconPrinter]           = ":/iconoir/printer";
            iconPathMap[IconPrinterDelete]     = ":/iconoir/printer-remove";
            iconPathMap[IconExit]              = ":/iconoir/log-out";
            iconPathMap[IconHelp]              = ":/iconoir/help-circle";
            iconPathMap[IconAbout]             = ":/iconoir/info-circle";
            iconPathMap[IconMonitor]           = ":/iconoir/desktop";
            iconPathMap[IconMonitorGo]         = ":/iconoir/desktop-apps";
            iconPathMap[IconFont]              = ":/iconoir/text";
            iconPathMap[IconFontGo]            = ":/iconoir/text-size";
            iconPathMap[IconFontSizeMore]      = ":/iconoir/text-size";
            iconPathMap[IconPackageDelete]     = ":/iconoir/package-remove";
            iconPathMap[IconPackageGo]         = ":/iconoir/package";
            iconPathMap[IconCamera]            = ":/iconoir/camera";
            iconPathMap[IconRun]               = ":/iconoir/play";
            iconPathMap[IconApplication]       = ":/iconoir/apps";
            iconPathMap[IconShield]            = ":/iconoir/shield";
            iconPathMap[IconShieldGo]          = ":/iconoir/shield-check";

            // Steuerungsfunktionen
            iconPathMap[IconAdd]              = ":/iconoir/add-circle";
            iconPathMap[IconDelete]           = ":/iconoir/delete-circle";
            iconPathMap[IconCut]              = ":/iconoir/cut";
            iconPathMap[IconMoveRight]        = ":/iconoir/arrow-right";
            iconPathMap[IconMoveLeft]         = ":/iconoir/arrow-left";
            iconPathMap[IconMoveUp]           = ":/iconoir/arrow-up";
            iconPathMap[IconMove]             = ":/iconoir/move";
            iconPathMap[IconArrowUndo]        = ":/iconoir/undo";
            iconPathMap[IconArrowRight]       = ":/iconoir/arrow-right";
            iconPathMap[IconDoubleArrowRight] = ":/iconoir/fast-arrow-right";
            iconPathMap[IconViewLeftClose]    = ":/iconoir/arrow-left-circle";
            iconPathMap[IconViewRightClose]   = ":/iconoir/arrow-right-circle";
            iconPathMap[IconLock]             = ":/iconoir/lock";
            iconPathMap[IconLockOpen]         = ":/iconoir/lock-open";
            iconPathMap[IconGo]               = ":/iconoir/play";
            iconPathMap[IconSave]             = ":/iconoir/save-floppy-disk";
            iconPathMap[IconSaveAll]          = ":/iconoir/save-action-floppy";

            // Status-Icons
            iconPathMap[IconFlagGreen]     = ":/iconoir/check-circle";
            iconPathMap[IconFlagRed]       = ":/iconoir/warning-circle";
            iconPathMap[IconHappy]         = ":/iconoir/emoji-satisfied";
            iconPathMap[IconUnhappy]       = ":/iconoir/emoji-sad";

            // Spezielle Symbole
            iconPathMap[IconMainApp16]     = ":/iconoir/apps";
            iconPathMap[IconMainApp]       = ":/iconoir/apps";
            iconPathMap[IconTextList]      = ":/iconoir/list";
            iconPathMap[IconTextAlignLeft] = ":/iconoir/text-align-left";
        }

    }
} // namespace gui