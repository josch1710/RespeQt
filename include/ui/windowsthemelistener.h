/*
 * windowsthemelistener.h
 *
 * Listens to Windows 10/11 system dark/light theme changes and sets the application palette.
 */

#ifndef WINDOWSTHEMELISTENER_H
#define WINDOWSTHEMELISTENER_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QObject>
#include <QPalette>

namespace UI {

class WindowsThemeListener : public QObject, public QAbstractNativeEventFilter {
  Q_OBJECT

public:
  static void init(QApplication *app);

  explicit WindowsThemeListener(QObject *parent = nullptr);
  ~WindowsThemeListener() override;

  bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
  bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
  void updateTheme();

private:
  void updateTitleBar(QWidget *widget) const;
  void updateAllTitleBars() const;

  QPalette m_defaultPalette;
  bool m_isDark{false};
};

} // namespace UI

#endif // Q_OS_WIN

#endif // WINDOWSTHEMELISTENER_H
