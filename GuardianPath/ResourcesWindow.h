#ifndef RESOURCESWINDOW_H
#define RESOURCESWINDOW_H

#include <QWidget>
#include <QPushButton>

class ResourcesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ResourcesWindow(QWidget* parent = nullptr);

signals:
    void backToMainRequested();
};

#endif // RESOURCESWINDOW_H