#ifndef MONITORAPPLIANCEWINDOW_H
#define MONITORAPPLIANCEWINDOW_H

#include <QWidget>
#include <QPushButton>

class MonitorApplianceWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorApplianceWindow(QWidget* parent = nullptr);

signals:
    void backToMainRequested();
};

#endif // MONITORAPPLIANCEWINDOW_H