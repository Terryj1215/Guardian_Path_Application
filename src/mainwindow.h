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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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
};

#endif // MAINWINDOW_H
