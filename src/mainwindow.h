#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

class Dashboard;
class TrackLocation;
class Appliances;
class CareRing;
class Resources;
class MongoDBManager;
class GPSManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MongoDBManager* mongoManager, GPSManager* gpsManager, QWidget* parent = nullptr);

    MongoDBManager* mongoManager() const { return m_mongoManager; }
    GPSManager* gpsManager() const { return m_gpsManager; }
    ~MainWindow() override = default;

private slots:
    void navigateToDashboard();
    void navigateToTrackLocation();
    void navigateToAppliances();
    void navigateToCareRing();
    void navigateToResources();
    void updateNavigation(int index);

private:
    void setupUi();
    void createSidebar();
    void createPages();
    QPushButton* createNavButton(const QString &text, const QString &iconPath = "");

    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    QFrame *sidebar;
    QStackedWidget *pageStack;
    
    // Navigation buttons
    QPushButton *btnDashboard;
    QPushButton *btnTrackLocation;
    QPushButton *btnAppliances;
    QPushButton *btnCareRing;
    QPushButton *btnResources;
    QList<QPushButton*> navButtons;
    
    // Pages
    Dashboard *dashboardPage;
    TrackLocation *trackLocationPage;
    Appliances *appliancesPage;
    CareRing *careRingPage;
    Resources *resourcesPage;
    MongoDBManager* m_mongoManager;
    GPSManager* m_gpsManager;
};

#endif // MAINWINDOW_H
