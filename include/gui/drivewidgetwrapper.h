
#ifndef GUIDRIVEWIDGETWRAPPER_H
#define GUIDRIVEWIDGETWRAPPER_H

#include "gui/drivewidget.h"
#include <QQuickWidget>
#include <QWidget>

namespace gui {

class DriveWidgetWrapper : public QWidget {
    Q_OBJECT
public:
    explicit DriveWidgetWrapper(int driveNum, QWidget *parent = nullptr);
    
    // Wrapper-Methoden für DriveWidget
    QString driveName() const;
    void setDriveName(const QString &name);
    
    bool isMounted() const;
    void setMounted(bool mounted);
    
    int driveNumber() const;
    void setDriveNumber(int number);

    void setup();

public slots:
    // Öffentliche Slots für Aktionen
    void mount();
    void unmount();
    void toggleWriteProtection();
    
signals:
    void driveNameChanged();
    void mountedChanged();
    void mountRequested();
    void unmountRequested();

private:
    QQuickWidget *m_quickWidget;
    DriveWidget *m_driveWidget;
    
    void initializeQml();
};

}  // namespace gui

#endif // GUIDRIVEWIDGETWRAPPER_H
