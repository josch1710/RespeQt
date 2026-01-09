
#include "gui/drivewidgetwrapper.h"
#include "gui/drivewidget.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <QVBoxLayout>

namespace gui {

DriveWidgetWrapper::DriveWidgetWrapper(int driveNum, QWidget *parent)
    : QWidget(parent)
    , m_quickWidget(new QQuickWidget(this))
    , m_driveWidget(nullptr)
{
    // Layout einrichten
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_quickWidget);
    
    // QQuickWidget konfigurieren
    m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    
    initializeQml();
    
    // Signal-Verbindungen einrichten
    connect(m_driveWidget, &DriveWidget::driveNameChanged,
            this, &DriveWidgetWrapper::driveNameChanged);
    connect(m_driveWidget, &DriveWidget::mountedChanged,
            this, &DriveWidgetWrapper::mountedChanged);
    connect(m_driveWidget, &DriveWidget::mountRequested,
            this, &DriveWidgetWrapper::mountRequested);
    connect(m_driveWidget, &DriveWidget::unmountRequested,
            this, &DriveWidgetWrapper::unmountRequested);
}

void DriveWidgetWrapper::initializeQml()
{
    // DriveWidget QML-Typ registrieren
    DriveWidget::registerQmlTypes();
    
    // QML-Datei laden
    m_quickWidget->setSource(QUrl("qrc:/qml/drivewidget.qml"));
    
    // DriveWidget-Instanz aus QML abrufen
    auto rootObject = m_quickWidget->rootObject();
    m_driveWidget = qobject_cast<DriveWidget*>(rootObject);
}

QString DriveWidgetWrapper::driveName() const
{
    return m_driveWidget ? m_driveWidget->driveName() : QString();
}

void DriveWidgetWrapper::setDriveName(const QString &name)
{
    if (m_driveWidget) {
        m_driveWidget->setDriveName(name);
    }
}

bool DriveWidgetWrapper::isMounted() const
{
    return m_driveWidget ? m_driveWidget->isMounted() : false;
}

void DriveWidgetWrapper::setMounted(bool mounted)
{
    if (m_driveWidget) {
        m_driveWidget->setMounted(mounted);
    }
}

int DriveWidgetWrapper::driveNumber() const
{
    return m_driveWidget ? m_driveWidget->getDriveNumber() : -1;
}

void DriveWidgetWrapper::setDriveNumber(int number)
{
    if (m_driveWidget) {
        m_driveWidget->setDriveNumber(number);
    }
}

void DriveWidgetWrapper::mount()
{
    if (m_driveWidget) {
        m_driveWidget->mount();
    }
}

void DriveWidgetWrapper::unmount()
{
    if (m_driveWidget) {
        m_driveWidget->unmount();
    }
}

void DriveWidgetWrapper::toggleWriteProtection()
{
    if (m_driveWidget) {
        m_driveWidget->toggleWriteProtection();
    }
}

void DriveWidgetWrapper::setup()
{
    m_driveWidget->setup();
}

}  // namespace gui
