/*
 * windowsthemelistener.cpp
 *
 * Listens to Windows 10/11 system dark/light theme changes and sets the application palette.
 */

#include "ui/windowsthemelistener.h"

#ifdef Q_OS_WIN

#include "ui/uicolors.h"
#include <QEvent>
#include <QWidget>
#include <windows.h>

namespace UI {

static bool isWindowsDarkTheme() {
  DWORD value = 1;
  DWORD size = sizeof(value);
  const LSTATUS status = RegGetValueW(
      HKEY_CURRENT_USER,
      L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
      L"AppsUseLightTheme",
      RRF_RT_REG_DWORD,
      nullptr,
      &value,
      &size);
  return (status == ERROR_SUCCESS && value == 0);
}

static void setDarkTitleBar(HWND hwnd, bool dark) {
  if (!hwnd) return;
  BOOL useDarkMode = dark ? TRUE : FALSE;
  HMODULE dwmapi = GetModuleHandleW(L"dwmapi.dll");
  if (!dwmapi) {
    dwmapi = LoadLibraryW(L"dwmapi.dll");
  }
  if (dwmapi) {
    typedef HRESULT (WINAPI *DwmSetWindowAttributeFunc)(HWND, DWORD, LPCVOID, DWORD);
    auto pDwmSetWindowAttribute = reinterpret_cast<DwmSetWindowAttributeFunc>(GetProcAddress(dwmapi, "DwmSetWindowAttribute"));
    if (pDwmSetWindowAttribute) {
      // 20 is DWMWA_USE_IMMERSIVE_DARK_MODE (Windows 10 20H1+ and Windows 11)
      // 19 is DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 (Windows 10 1809 - 1909)
      if (FAILED(pDwmSetWindowAttribute(hwnd, 20, &useDarkMode, sizeof(useDarkMode)))) {
        pDwmSetWindowAttribute(hwnd, 19, &useDarkMode, sizeof(useDarkMode));
      }
    }
  }
}

void WindowsThemeListener::init(QApplication *app) {
  if (app) {
    auto listener = new WindowsThemeListener(app);
    app->installNativeEventFilter(listener);
    app->installEventFilter(listener);
  }
}

WindowsThemeListener::WindowsThemeListener(QObject *parent) : QObject(parent) {
  m_defaultPalette = QApplication::palette();
  updateTheme();
}

WindowsThemeListener::~WindowsThemeListener() {
  if (qApp) {
    qApp->removeNativeEventFilter(this);
  }
}

bool WindowsThemeListener::nativeEventFilter(const QByteArray &eventType, void *message, long *result) {
  Q_UNUSED(result);
  if (eventType == "windows_generic_MSG") {
    const auto msg = static_cast<const MSG *>(message);
    if (msg->message == WM_SETTINGCHANGE || msg->message == WM_THEMECHANGED) {
      updateTheme();
    }
  }
  return false;
}

bool WindowsThemeListener::eventFilter(QObject *watched, QEvent *event) {
  if (event->type() == QEvent::Show || event->type() == QEvent::WinIdChange) {
    if (auto widget = qobject_cast<QWidget *>(watched)) {
      if (widget->isWindow()) {
        updateTitleBar(widget);
      }
    }
  }
  return QObject::eventFilter(watched, event);
}

void WindowsThemeListener::updateTheme() {
  m_isDark = isWindowsDarkTheme();
  if (m_isDark) {
    QApplication::setPalette(Colors::darkPalette());
  } else {
    QApplication::setPalette(m_defaultPalette);
  }
  updateAllTitleBars();
}

void WindowsThemeListener::updateTitleBar(QWidget *widget) const {
  if (widget && widget->isWindow()) {
    setDarkTitleBar(reinterpret_cast<HWND>(widget->winId()), m_isDark);
  }
}

void WindowsThemeListener::updateAllTitleBars() const {
  const auto topLevelWidgets = QApplication::topLevelWidgets();
  for (QWidget *widget : topLevelWidgets) {
    updateTitleBar(widget);
  }
}

} // namespace UI

#endif // Q_OS_WIN
