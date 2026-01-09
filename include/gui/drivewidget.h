#ifndef GUIDRIVEWIDGET_H
#define GUIDRIVEWIDGET_H

#include <QQuickItem>

namespace gui
{

// DriveWidgetItem.h
class DriveWidget : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QString driveName READ driveName WRITE setDriveName NOTIFY driveNameChanged)
    Q_PROPERTY(bool mounted READ isMounted WRITE setMounted NOTIFY mountedChanged)
    
public:
    explicit DriveWidget(QQuickItem *parent = nullptr);
    
    QString driveName() const { return m_driveName; }
    void setDriveName(const QString &name);

    int getDriveNumber() { return m_driveNum; }
    void setDriveNumber(int driveNum) { m_driveNum = driveNum; }
    
    bool isMounted() const { return m_mounted; }
    void setMounted(bool mounted);

    void setup();
    
    // QML aufrufbare Methoden
    Q_INVOKABLE void mount();
    Q_INVOKABLE void unmount();
    Q_INVOKABLE void toggleWriteProtection();

    static void registerQmlTypes();
    
signals:
    void driveNameChanged();
    void mountedChanged();
    void mountRequested();
    void unmountRequested();
    
private:
    QString m_driveName;
    bool m_mounted;
    int m_driveNum;
};

}

#endif // GUIDRIVEWIDGET_H