#ifndef CARERINGWINDOW_H
#define CARERINGWINDOW_H

#include <QWidget>
#include <QPushButton>

class CareRingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CareRingWindow(QWidget* parent = nullptr);

signals:
    void backToMainRequested();
};

#endif // CARERINGWINDOW_H