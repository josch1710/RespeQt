#include "include/gui/stylesheets/classic_stylesheet.h"

namespace gui {
    namespace stylesheets {
    
        // ClassicStylesheet implementation
        ClassicStylesheet::ClassicStylesheet()
        {
            generateStylesheet();
            initializeIconMap();
        }

        void ClassicStylesheet::generateStylesheet()
        {
            QString combinedStylesheet = R"(
                /* Classic Stylesheet - Traditional UI with subtle blue accents */
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

        QString ClassicStylesheet::getBaseStyles() const
        {
            return R"(
                QMainWindow, QDialog {
                    background-color: #f0f0f0;
                }
            )";
        }

        QString ClassicStylesheet::getMenuStyles() const
        {
            return R"(
                QMenuBar {
                    background-color: #f0f0f0;
                    border-bottom: 1px solid #d0d0d0;
                }

                QMenuBar::item {
                    background-color: transparent;
                    padding: 4px 10px;
                }

                QMenuBar::item:selected {
                    background-color: #e0e0e0;
                    border: 1px solid #c0c0c0;
                    border-radius: 3px;
                }

                QMenu {
                    background-color: #f8f8f8;
                    border: 1px solid #c0c0c0;
                }

                QMenu::item {
                    padding: 5px 30px 5px 30px;
                }

                QMenu::item:selected {
                    background-color: #e0e0e0;
                }

                QToolBar {
                    background-color: #f5f5f5;
                    border: 1px solid #d0d0d0;
                    spacing: 3px;
                    padding: 3px;
                }

                QToolButton {
                    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                      stop: 0 #f6f7fa, stop: 1 #dadbde);
                    border: 1px solid #D0D0D0;
                    border-radius: 3px;
                    padding: 3px;
                }

                QToolButton:hover {
                    background-color: #e0e0e0;
                    border: 1px solid #c0c0c0;
                }

                QToolButton:pressed {
                    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                      stop: 0 #dadbde, stop: 1 #f6f7fa);
                }

                QStatusBar {
                    background-color: #f0f0f0;
                    border-top: 1px solid #d0d0d0;
                }

                QStatusBar::item {
                    border: none;
                }
            )";
        }

        QString ClassicStylesheet::getButtonStyles() const
        {
            return R"(
                QPushButton {
                    background-color: #e8e8e8;
                    border: 1px solid #c0c0c0;
                    border-radius: 3px;
                    padding: 4px 15px;
                }

                QPushButton:hover {
                    background-color: #d8d8d8;
                }

                QPushButton:pressed {
                    background-color: #c0c0c0;
                }
            )";
        }

        QString ClassicStylesheet::getFormStyles() const
        {
            return R"(
                QLineEdit, QTextEdit {
                    background-color: white;
                    border: 1px solid #c0c0c0;
                    border-radius: 3px;
                    padding: 2px;
                }

                QCheckBox {
                    spacing: 5px;
                }

                QCheckBox::indicator {
                    width: 13px;
                    height: 13px;
                }

                QCheckBox::indicator:unchecked {
                    border: 1px solid #c0c0c0;
                    background-color: white;
                }

                QCheckBox::indicator:checked {
                    border: 1px solid #c0c0c0;
                    background-color: #6ca6d0;
                }

                QRadioButton {
                    spacing: 5px;
                }

                QRadioButton::indicator {
                    width: 13px;
                    height: 13px;
                }

                QRadioButton::indicator:unchecked {
                    border: 1px solid #c0c0c0;
                    border-radius: 6px;
                    background-color: white;
                }

                QRadioButton::indicator:checked {
                    border: 1px solid #c0c0c0;
                    border-radius: 6px;
                    background-color: #6ca6d0;
                }

                QProgressBar {
                    border: 1px solid #c0c0c0;
                    border-radius: 3px;
                    background-color: #f0f0f0;
                    padding: 1px;
                    text-align: center;
                }

                QProgressBar::chunk {
                    background-color: #6ca6d0;
                    width: 10px;
                }
            )";
        }

        QString ClassicStylesheet::getTabStyles() const
        {
            return R"(
                QTabWidget::pane {
                    border: 1px solid #c0c0c0;
                    background-color: white;
                }

                QTabBar::tab {
                    background-color: #e0e0e0;
                    border: 1px solid #c0c0c0;
                    border-bottom: none;
                    border-top-left-radius: 3px;
                    border-top-right-radius: 3px;
                    padding: 5px 10px;
                    margin-right: 2px;
                }

                QTabBar::tab:selected {
                    background-color: white;
                }

                QGroupBox {
                    border: 1px solid #c0c0c0;
                    border-radius: 3px;
                    margin-top: 1.5ex;
                    padding-top: 1ex;
                }

                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top center;
                    padding: 0 3px;
                }
            )";
        }

        QString ClassicStylesheet::getDriveStyles() const
        {
            return R"(
                /* Drive Styling */
                QFrame#diskWidget {
                    border: 1px solid #c0c0c0;
                    border-radius: 4px;
                    background-color: #f8f8f8;
                }

                QLabel#labelFileName {
                    font-weight: bold;
                    color: #366a96;
                }
            )";
        }

        QString ClassicStylesheet::getScrollbarStyles() const
        {
            return R"(
                /* Empty for classic style */
            )";
        }

        void ClassicStylesheet::initializeIconMap()
        {
            // File system functions
            iconPathMap[IconFolder]        = ":/icons/silk-icons/icons/folder.png";
            iconPathMap[IconFolderEdit]    = ":/icons/silk-icons/icons/folder_edit.png";
            iconPathMap[IconFolderExplore] = ":/icons/silk-icons/icons/folder_explore.png";
            iconPathMap[IconFolderImage]   = ":/icons/silk-icons/icons/folder_image.png";
            iconPathMap[IconFolderHtml]    = ":/icons/oxygen-icons/16x16/filesystems/folder_html.png";
            iconPathMap[IconFile]          = ":/icons/silk-icons/icons/page_white.png";
            iconPathMap[IconFileText]      = ":/icons/silk-icons/icons/page_white_text.png";
            iconPathMap[IconFileC]         = ":/icons/silk-icons/icons/page_white_c.png";
            iconPathMap[IconImageFile]     = ":/icons/silk-icons/icons/image.png";

            // Drive functions
            iconPathMap[IconDrive]         = ":/icons/silk-icons/icons/drive.png";
            iconPathMap[IconDriveAdd]      = ":/icons/silk-icons/icons/drive_add.png";
            iconPathMap[IconDriveDelete]   = ":/icons/silk-icons/icons/drive_delete.png";
            iconPathMap[IconDriveRename]   = ":/icons/silk-icons/icons/drive_rename.png";
            iconPathMap[IconDriveEject]    = ":/icons/silk-icons/icons/control_eject.png";
            iconPathMap[IconDriveGo]       = ":/icons/silk-icons/icons/drive_go.png";
            iconPathMap[IconDriveOpen]     = ":/icons/silk-icons/icons/drive_open.png";
            iconPathMap[IconDriveClose]    = ":/icons/silk-icons/icons/drive_close.png";
            iconPathMap[IconDriveSave]     = ":/icons/silk-icons/icons/drive_save.png";
            iconPathMap[IconDriveDisk]     = ":/icons/silk-icons/icons/drive_disk.png";
            iconPathMap[IconFloppyMount]   = ":/icons/oxygen-icons/16x16/devices/3floppy_mount.png";
            iconPathMap[IconFloppyUnmount] = ":/icons/oxygen-icons/16x16/devices/3floppy_unmount.png";
            iconPathMap[IconFloppyFront]   = ":/icons/other-icons/floppy_front.png";
            iconPathMap[IconFloppyBack]    = ":/icons/other-icons/floppy_back.png";
            iconPathMap[IconMediaFloppy]   = ":/icons/oxygen-icons/16x16/devices/media_floppy.png";
            iconPathMap[IconMediaTape]     = ":/icons/oxygen-icons/16x16/devices/media_tape.png";
            iconPathMap[IconCassette]      = ":/icons/other-icons/casette.gif";
            iconPathMap[IconAutoSave]      = ":/icons/silk-icons/icons/drive_go.png";

            // General functions
            iconPathMap[IconConnect]           = ":/icons/silk-icons/icons/connect.png";
            iconPathMap[IconDisconnect]        = ":/icons/silk-icons/icons/disconnect.png";
            iconPathMap[IconNetworkConnect]    = ":/icons/oxygen-icons/16x16/actions/network_connect.png";
            iconPathMap[IconNetworkDisconnect] = ":/icons/oxygen-icons/16x16/actions/network_disconnect.png";
            iconPathMap[IconNetwork]           = ":/icons/oxygen-icons/16x16/actions/network.png";
            iconPathMap[IconPrinter]           = ":/icons/silk-icons/icons/printer.png";
            iconPathMap[IconPrinterDelete]     = ":/icons/silk-icons/icons/printer_delete.png";
            iconPathMap[IconExit]              = ":/icons/oxygen-icons/16x16/actions/exit.png";
            iconPathMap[IconHelp]              = ":/icons/oxygen-icons/16x16/actions/help_contents.png";
            iconPathMap[IconAbout]             = ":/icons/oxygen-icons/16x16/actions/help_about.png";
            iconPathMap[IconMonitor]           = ":/icons/silk-icons/icons/monitor.png";
            iconPathMap[IconMonitorGo]         = ":/icons/silk-icons/icons/monitor_go.png";
            iconPathMap[IconFont]              = ":/icons/silk-icons/icons/font.png";
            iconPathMap[IconFontGo]            = ":/icons/silk-icons/icons/font_go.png";
            iconPathMap[IconFontSizeMore]      = ":/icons/oxygen-icons/16x16/actions/format_font_size_more.png";
            iconPathMap[IconPackageDelete]     = ":/icons/silk-icons/icons/package_delete.png";
            iconPathMap[IconPackageGo]         = ":/icons/silk-icons/icons/package_go.png";
            iconPathMap[IconCamera]            = ":/icons/silk-icons/icons/camera.png";
            iconPathMap[IconRun]               = ":/icons/other-icons/run.ico";
            iconPathMap[IconApplication]       = ":/icons/silk-icons/icons/application.png";
            iconPathMap[IconShield]            = ":/icons/silk-icons/icons/shield.png";
            iconPathMap[IconShieldGo]          = ":/icons/silk-icons/icons/shield_go.png";
            iconPathMap[IconToggleOSB]         = ":/icons/other-icons/toggleosb.png";
            iconPathMap[IconSideB]             = ":/icons/other-icons/sideb.png";

            // Control functions
            iconPathMap[IconAdd]              = ":/icons/silk-icons/icons/add.png";
            iconPathMap[IconDelete]           = ":/icons/silk-icons/icons/delete.png";
            iconPathMap[IconCut]              = ":/icons/silk-icons/icons/cut_red.png";
            iconPathMap[IconMoveRight]        = ":/icons/silk-icons/icons/arrow_right.png";
            iconPathMap[IconMoveLeft]         = ":/icons/silk-icons/icons/arrow_undo.png";
            iconPathMap[IconMoveUp]           = ":/icons/silk-icons/icons/arrow_up.png";
            iconPathMap[IconMove]             = ":/icons/other-icons/move.png";
            iconPathMap[IconArrowUndo]        = ":/icons/silk-icons/icons/arrow_undo.png";
            iconPathMap[IconArrowRight]       = ":/icons/silk-icons/icons/arrow_right.png";
            iconPathMap[IconDoubleArrowRight] = ":/icons/oxygen-icons/16x16/actions/2rightarrow.png";
            iconPathMap[IconViewLeftClose]    = ":/icons/oxygen-icons/16x16/actions/view_left_close.png";
            iconPathMap[IconViewRightClose]   = ":/icons/oxygen-icons/16x16/actions/view_right_close.png";
            iconPathMap[IconLock]             = ":/icons/silk-icons/icons/lock.png";
            iconPathMap[IconLockOpen]         = ":/icons/silk-icons/icons/lock_open.png";
            iconPathMap[IconGo]               = ":/icons/silk-icons/icons/page_white_go.png";
            iconPathMap[IconSave]             = ":/icons/silk-icons/icons/drive_save.png";
            iconPathMap[IconSaveAll]          = ":/icons/oxygen-icons/16x16/actions/document_save_all.png";
            iconPathMap[IconFadeIn]           = ":/icons/other-icons/fade.png";
            iconPathMap[IconShape]            = ":/icons/silk-icons/icons/shape_square.png";

            // Status icons
            iconPathMap[IconFlagGreen] = ":/icons/oxygen-icons/16x16/actions/flag_green.png";
            iconPathMap[IconFlagRed]   = ":/icons/oxygen-icons/16x16/actions/flag_red.png";
            iconPathMap[IconHappy]     = ":/icons/silk-icons/icons/emoticon_happy.png";
            iconPathMap[IconUnhappy]   = ":/icons/silk-icons/icons/emoticon_unhappy.png";

            // Special symbols
            iconPathMap[IconMainApp16]     = ":/icons/main-icon/main16.png";
            iconPathMap[IconMainApp]       = ":/icons/main-icon/RespeQt.png";
            iconPathMap[IconAtari]         = ":/icons/other-icons/Atari.png";
            iconPathMap[IconMiniCooper]    = ":/icons/other-icons/MiniCooper 22.ico";
            iconPathMap[IconTextList]      = ":/icons/silk-icons/icons/text_list_numbers.png";
            iconPathMap[IconTextAlignLeft] = ":/icons/silk-icons/icons/text_align_left.png";
        }
    }
} // namespace gui
