# StylesheetManager Documentation

## Overview

The `StylesheetManager` is a class for managing stylesheets and icons in the RespeQt project. It uses a singleton pattern and offers the following main functionalities:

1. Management of different styles (classic, modern, dark, custom)
2. Access to icons via functional keys instead of filenames
3. Application of styles to the entire application

## Using the StylesheetManager

### Applying Styles

```cpp
// Instantiate StylesheetManager
StylesheetManager* styleManager = StylesheetManager::getInstance();

// Apply classic style
styleManager->applyStyleToApplication(StylesheetManager::ClassicStyle);

// Apply modern style
styleManager->applyStyleToApplication(StylesheetManager::ModernStyle);

// Apply dark style
styleManager->applyStyleToApplication(StylesheetManager::DarkStyle);

// Set and apply custom style
styleManager->setCustomStylesheet("QMainWindow { background-color: lightblue; }");
styleManager->applyStyleToApplication(StylesheetManager::CustomStyle);
```

### Using Icons via Functional Keys

```cpp
// Instantiate StylesheetManager
StylesheetManager* styleManager = StylesheetManager::getInstance();

// Create icons using functional keys instead of filenames
QAction* openAction = new QAction(QIcon(styleManager->getIconPath(StylesheetManager::IconFolder)), "Open", this);
QAction* saveAction = new QAction(QIcon(styleManager->getIconPath(StylesheetManager::IconSave)), "Save", this);
QAction* driveAction = new QAction(QIcon(styleManager->getIconPath(StylesheetManager::IconDrive)), "Drive", this);
```

## Advantages of Using the StylesheetManager

1. **Consistency**: Using predefined styles ensures a uniform appearance throughout the application.

2. **Maintainability**: Changes to styles or icon paths only need to be made in one central location.

3. **Semantics**: Icons are referenced by their function (e.g., `IconSave`) instead of their filename (`disk.png`), which improves code readability and maintainability.

4. **Flexibility**: Easy switching between different styles at runtime.

## Icon Functions

The StylesheetManager organizes icons into functional groups:

### File System Functions
- `IconFolder`, `IconFolderEdit`, `IconFolderExplore`, etc.

### Drive Functions
- `IconDrive`, `IconDriveAdd`, `IconDriveDelete`, `IconDriveRename`, etc.

### General Functions
- `IconConnect`, `IconDisconnect`, `IconPrinter`, `IconExit`, etc.

### Control Functions
- `IconAdd`, `IconDelete`, `IconMove`, `IconLock`, `IconLockOpen`, etc.

### Status Icons
- `IconFlagGreen`, `IconFlagRed`, `IconHappy`, `IconUnhappy`

### Special Symbols
- `IconMainApp`, `IconAtari`, etc.

## Extending the StylesheetManager

To add new icons:

1. Add a new entry to the `IconFunction` enumeration in `stylesheetmanager.h`.
2. Assign a path to the new icon key in the `initializeIconMap()` method in `stylesheetmanager.cpp`.

To add a new style:

1. Add a new entry to the `Style` enumeration in `stylesheetmanager.h`.
2. Create a new stylesheet variable in `stylesheetmanager.cpp`.
3. Update the `getStylesheet()` method to return the new style.
