#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget> // QStackedWidget is used to manage multiple screens
#include <QPushButton>

// Forward declarations to avoid circular dependencies
class TrackLocationWindow;
class MonitorApplianceWindow;
class ResourcesWindow;
class CareRingWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and destructor
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // Slots for button clicks
    void onTrackLocationClicked();
    void onMonitorApplianceClicked();
    void onResourcesClicked();
    void onCareRingClicked();
    void onBackToMainClicked(); // Slot for the "Back" button

private:
    // Central widget to hold and switch between different screens
    QStackedWidget* m_stackedWidget;

    // Pointers to each of the application's screens
    QWidget* m_mainMenu;
    TrackLocationWindow* m_trackLocationWindow;
    MonitorApplianceWindow* m_monitorApplianceWindow;
    ResourcesWindow* m_resourcesWindow;
    CareRingWindow* m_careRingWindow;

    // Helper function to create the main menu layout
    void setupMainMenu();
};

#endif // MAINWINDOW_H