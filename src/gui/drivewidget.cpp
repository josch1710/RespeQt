#include "gui/drivewidget.h"
#include "respeqtsettings.h"
#include <QQuickItem>

namespace gui
{

DriveWidget::DriveWidget(QQuickItem *parent)
    : QQuickItem(parent),
      m_mounted(false),
      m_driveNum(0)
{
    registerQmlTypes();
}

void DriveWidget::registerQmlTypes()
{
    // QML-Komponente muss als Typ registriert werden
    qmlRegisterType<DriveWidget>("RespeQt", 1, 0, "DriveWidget");
}

void DriveWidget::setDriveName(const QString &name) {
    if (m_driveName != name) {
        m_driveName = name;
        emit driveNameChanged();
    }
}

void DriveWidget::setMounted(bool mounted) {
    if (m_mounted != mounted) {
        m_mounted = mounted;
        emit mountedChanged();
    }
}

void DriveWidget::mount() {
    // Implementierung der Mount-Logik
    emit mountRequested();
}

void DriveWidget::unmount()
{
    
}

void DriveWidget::toggleWriteProtection()
{
    
}

void DriveWidget::setup()
{
    RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk();
}

}