#ifndef TRACKLOCATIONWINDOW_H
#define TRACKLOCATIONWINDOW_H

#include <QWidget>
#include <QPushButton>

class TrackLocationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TrackLocationWindow(QWidget* parent = nullptr);

signals:
    // Signal to request returning to the main menu
    void backToMainRequested();
};

#endif // TRACKLOCATIONWINDOW_H