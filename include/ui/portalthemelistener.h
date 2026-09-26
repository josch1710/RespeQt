/*
 * portalthemelistener.h
 *
 * Listens to FreeDesktop XDG Desktop Portal theme/color-scheme changes
 * over D-Bus on Linux platforms.
 */

#ifndef PORTALTHEMELISTENER_H
#define PORTALTHEMELISTENER_H

#include <QDBusVariant>
#include <QObject>
#include <QPalette>

namespace UI {

class PortalThemeListener : public QObject {
  Q_OBJECT

public:
  static void init(QObject *parent = nullptr);

public slots:
  void onSettingChanged(const QString &namespaceName, const QString &key, const QDBusVariant &value);

private:
  explicit PortalThemeListener(QObject *parent = nullptr);
  void queryCurrentSetting();
  void applyColorScheme(uint scheme);

  QPalette m_defaultPalette;
  bool m_hasDefaultPalette{false};
};

} // namespace UI

#endif // PORTALTHEMELISTENER_H
