/*
 * portalthemelistener.cpp
 *
 * Listens to FreeDesktop XDG Desktop Portal theme/color-scheme changes
 * over D-Bus on Linux platforms.
 */

#include "ui/portalthemelistener.h"

#include <QApplication>
#include <QColor>
#include <QDBusConnection>
#include <QDBusMessage>

namespace UI {

static uint extractColorScheme(const QVariant &variant) {
  QVariant v = variant;
  while (v.userType() == qMetaTypeId<QDBusVariant>() || v.canConvert<QDBusVariant>()) {
    const auto dbusVar = qvariant_cast<QDBusVariant>(v);
    if (!dbusVar.variant().isValid()) {
      break;
    }
    v = dbusVar.variant();
  }
  return v.toUInt();
}

void PortalThemeListener::init(QObject *parent) {
  if (QDBusConnection::sessionBus().isConnected()) {
    new PortalThemeListener(parent);
  }
}

PortalThemeListener::PortalThemeListener(QObject *parent) : QObject(parent) {
  m_defaultPalette = QApplication::palette();
  m_hasDefaultPalette = true;

  QDBusConnection::sessionBus().connect(
      QString(),
      QStringLiteral("/org/freedesktop/portal/desktop"),
      QStringLiteral("org.freedesktop.portal.Settings"),
      QStringLiteral("SettingChanged"),
      this,
      SLOT(onSettingChanged(QString, QString, QDBusVariant)));

  queryCurrentSetting();
}

void PortalThemeListener::queryCurrentSetting() {
  QDBusMessage message = QDBusMessage::createMethodCall(
      QStringLiteral("org.freedesktop.portal.Desktop"),
      QStringLiteral("/org/freedesktop/portal/desktop"),
      QStringLiteral("org.freedesktop.portal.Settings"),
      QStringLiteral("Read"));
  message << QStringLiteral("org.freedesktop.appearance") << QStringLiteral("color-scheme");

  const QDBusMessage reply = QDBusConnection::sessionBus().call(message);
  if (reply.type() == QDBusMessage::ReplyMessage && !reply.arguments().isEmpty()) {
    applyColorScheme(extractColorScheme(reply.arguments().at(0)));
  }
}

void PortalThemeListener::onSettingChanged(const QString &namespaceName, const QString &key, const QDBusVariant &value) {
  if (namespaceName == QLatin1String("org.freedesktop.appearance") && key == QLatin1String("color-scheme")) {
    applyColorScheme(extractColorScheme(value.variant()));
  }
}

void PortalThemeListener::applyColorScheme(const uint scheme) {
  if (!m_hasDefaultPalette) {
    m_defaultPalette = QApplication::palette();
    m_hasDefaultPalette = true;
  }

  if (scheme == 1) { // 1: Prefer Dark
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
    QApplication::setPalette(darkPalette);
  } else { // 0: No preference, 2: Prefer Light
    QApplication::setPalette(m_defaultPalette);
  }
}

} // namespace UI
