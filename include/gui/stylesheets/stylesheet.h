#ifndef STYLESHEET_BASE_H
#define STYLESHEET_BASE_H

#include <QHash>
#include <QIcon>
#include <QString>

namespace gui {
    namespace stylesheets {

        /**
         * @brief The Stylesheet class represents a single stylesheet.
         *
         * This class encapsulates the content and functionality of a stylesheet.
         * It provides methods for managing the stylesheet content.
         */
        class Stylesheet
        {
        public:
            // Icon functions for consistent naming
            enum IconFunction: uint16_t
            {
                // File system functions
                IconFolder,
                IconFolderEdit,
                IconFolderExplore,
                IconFolderImage,
                IconFolderHtml,
                IconFile,
                IconFileText,
                IconFileC,
                IconImageFile,
                IconAutoSave,

                // Drive functions
                IconDrive,
                IconDriveAdd,
                IconDriveDelete,
                IconDriveRename,
                IconDriveEject,
                IconDriveGo,
                IconDriveOpen,
                IconDriveClose,
                IconDriveSave,
                IconDriveDisk,
                IconFloppyMount,
                IconFloppyUnmount,
                IconFloppyFront,
                IconFloppyBack,
                IconMediaFloppy,
                IconMediaTape,
                IconCassette,

                // General functions
                IconConnect,
                IconDisconnect,
                IconNetworkConnect,
                IconNetworkDisconnect,
                IconNetwork,
                IconPrinter,
                IconPrinterDelete,
                IconExit,
                IconHelp,
                IconAbout,
                IconMonitor,
                IconMonitorGo,
                IconFont,
                IconFontGo,
                IconFontSizeMore,
                IconPackageDelete,
                IconPackageGo,
                IconCamera,
                IconRun,
                IconApplication,
                IconShield,
                IconShieldGo,
                IconToggleOSB,
                IconSideB,

                // Control functions
                IconAdd,
                IconDelete,
                IconCut,
                IconMoveRight,
                IconMoveLeft,
                IconMoveUp,
                IconMove,
                IconArrowUndo,
                IconArrowRight,
                IconDoubleArrowRight,
                IconViewLeftClose,
                IconViewRightClose,
                IconLock,
                IconLockOpen,
                IconGo,
                IconSave,
                IconSaveAll,
                IconFadeIn,
                IconShape,

                // Status icons
                IconFlagGreen,
                IconFlagRed,
                IconHappy,
                IconUnhappy,

                // Special symbols
                IconMainApp16,
                IconMainApp,
                IconAtari,
                IconMiniCooper,
                IconTextList,
                IconTextAlignLeft,

                // Maximum number of icons
                IconCount
            };

            /**
             * @brief Default constructor.
             */
            Stylesheet();

            /**
             * @brief Constructor with initial content.
             * @param content The initial content of the stylesheet.
             */
            explicit Stylesheet(const QString &content);

            /**
             * @brief Destructor.
             */
            virtual ~Stylesheet();

            /**
             * @brief Get the stylesheet content
             * @return The content of the stylesheet as a QString
             */
            QString getContent() const;

            /**
             * @brief Set the stylesheet content
             * @param content The new content for the stylesheet
             */
            void setContent(const QString &content);

            /**
             * @brief Check if the stylesheet is empty
             * @return True if the stylesheet has no content, false otherwise
             */
            bool isEmpty() const;

            /**
             * @brief Append content to the stylesheet
             * @param content The content to add to the existing stylesheet
             */
            void append(const QString &content);

            /**
             * @brief Clear the stylesheet content
             */
            void clear();

            /**
             * @brief Generate the full stylesheet content based on component parts
             * @return The generated full stylesheet
             */
            virtual void generateStylesheet();

            /**
             * @brief Get base styles for windows and dialogs
             * @return CSS string with base styles
             */
            virtual QString getBaseStyles() const;

            /**
             * @brief Get styles for menus, menu bars, and toolbars
             * @return CSS string with menu styles
             */
            virtual QString getMenuStyles() const;

            /**
             * @brief Get styles for buttons
             * @return CSS string with button styles
             */
            virtual QString getButtonStyles() const;

            /**
             * @brief Get styles for form elements
             * @return CSS string with form styles
             */
            virtual QString getFormStyles() const;

            /**
             * @brief Get styles for tabs and group boxes
             * @return CSS string with tab styles
             */
            virtual QString getTabStyles() const;

            /**
             * @brief Get styles for drive widgets
             * @return CSS string with drive styles
             */
            virtual QString getDriveStyles() const;

            /**
             * @brief Get styles for scrollbars
             * @return CSS string with scrollbar styles
             */
            virtual QString getScrollbarStyles() const;

            /**
             * @brief Returns the path to the icon for the specified function.
             * @param iconFunction The icon function for which to return the path.
             * @return The path to the icon as a QString.
             */
            QString getIconPath(IconFunction iconFunction);

            /**
             * @brief Initializes the mapping of icon functions to their corresponding icons.
             *
             * This pure virtual method is intended to set up a mapping structure that associates
             * specific icon functions with their respective icons. Implementations of this method
             * should define and populate the mapping required for consistent icon handling.
             */
            virtual void initializeIconMap() = 0;

            /**
             * @brief Creates an icon with the specified size.
             * @param function The icon function to use for the icon.
             * @return The created icon as a QIcon.
             */
            QIcon createMultipleSizeIcon(const IconFunction &function);
            
            QSize calculateScreenScale();
        
        protected:
            QString content{""};
            QHash<IconFunction, QString> iconPathMap{};

            static constexpr int sizes[] = {24, 32, 48, 64, 128, 256};
        };

        using StylesheetPtr = QSharedPointer<Stylesheet>;
    
    }
}  // namespace gui

#endif  // STYLESHEET_BASE_H
