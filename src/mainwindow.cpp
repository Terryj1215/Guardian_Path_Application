#include "mainwindow.h"
#include "dashboard.h"
#include "tracklocation.h"
#include "appliances.h"
#include "carering.h"
#include "resources.h"
#include <QStyle>
#include "mongodbmanager.h"
#include "gpsmanager.h"

MainWindow::MainWindow(MongoDBManager* mongoManager, GPSManager* gpsManager, QWidget* parent)
    : QMainWindow(parent)
    , m_mongoManager(mongoManager)
    , m_gpsManager(gpsManager)
{
    setupUi();
}

void MainWindow::setupUi()
{
    setWindowTitle("Guardian Path - Safety Dashboard");
    setMinimumSize(1200, 800);
    resize(1400, 900);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    createSidebar();
    createPages();

    // Set initial page
    pageStack->setCurrentIndex(0);
    updateNavigation(0);
}

void MainWindow::createSidebar()
{
    sidebar = new QFrame(this);
    sidebar->setProperty("class", "sidebar");
    sidebar->setFixedWidth(260);

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(16, 24, 16, 24);
    sidebarLayout->setSpacing(8);

    // Logo/Title
    QLabel* logo = new QLabel("Guardian Path", sidebar);
    logo->setProperty("class", "title");
    logo->setStyleSheet("font-size: 20px; font-weight: 700; color: #4a6cf7; margin-bottom: 8px;");
    sidebarLayout->addWidget(logo);

    QLabel* tagline = new QLabel("Safety Dashboard", sidebar);
    tagline->setProperty("class", "muted");
    tagline->setStyleSheet("margin-bottom: 32px;");
    sidebarLayout->addWidget(tagline);

    // Navigation buttons
    btnDashboard = createNavButton("Dashboard");
    btnTrackLocation = createNavButton("Track Location");
    btnAppliances = createNavButton("Monitor Appliances");
    btnCareRing = createNavButton("Care Ring");
    btnResources = createNavButton("Resources");

    navButtons << btnDashboard << btnTrackLocation << btnAppliances << btnCareRing << btnResources;

    sidebarLayout->addWidget(btnDashboard);
    sidebarLayout->addWidget(btnTrackLocation);
    sidebarLayout->addWidget(btnAppliances);
    sidebarLayout->addWidget(btnCareRing);
    sidebarLayout->addWidget(btnResources);

    sidebarLayout->addStretch();

    // User info at bottom
    QFrame* userFrame = new QFrame(sidebar);
    userFrame->setStyleSheet("background-color: #232a3d; border-radius: 8px; padding: 12px;");
    QVBoxLayout* userLayout = new QVBoxLayout(userFrame);
    userLayout->setContentsMargins(12, 12, 12, 12);

    QLabel* userName = new QLabel("Welcome back", userFrame);
    userName->setStyleSheet("color: #a0a5b5; font-size: 12px;");
    QLabel* userNameValue = new QLabel("User", userFrame);
    userNameValue->setStyleSheet("color: #f0f0f5; font-weight: 600;");

    userLayout->addWidget(userName);
    userLayout->addWidget(userNameValue);
    sidebarLayout->addWidget(userFrame);

    // Connect signals
    connect(btnDashboard, &QPushButton::clicked, this, &MainWindow::navigateToDashboard);
    connect(btnTrackLocation, &QPushButton::clicked, this, &MainWindow::navigateToTrackLocation);
    connect(btnAppliances, &QPushButton::clicked, this, &MainWindow::navigateToAppliances);
    connect(btnCareRing, &QPushButton::clicked, this, &MainWindow::navigateToCareRing);
    connect(btnResources, &QPushButton::clicked, this, &MainWindow::navigateToResources);

    mainLayout->addWidget(sidebar);
}

void MainWindow::createPages()
{
    pageStack = new QStackedWidget(this);

    dashboardPage = new Dashboard(this);

    // Pass the mongoManager to the trackLocationPage
    trackLocationPage = new TrackLocation(m_mongoManager, this);

    // UPDATED: Pass the mongoManager to the careRingPage to enable MongoDB integration
    careRingPage = new CareRing(m_mongoManager, this);

    // Future consideration: Pass m_mongoManager to appliancesPage for persistent state
    appliancesPage = new Appliances(this);

    resourcesPage = new Resources(this);

    pageStack->addWidget(dashboardPage);
    pageStack->addWidget(trackLocationPage);
    pageStack->addWidget(appliancesPage);
    pageStack->addWidget(careRingPage);
    pageStack->addWidget(resourcesPage);

    mainLayout->addWidget(pageStack);
}

QPushButton* MainWindow::createNavButton(const QString& text, const QString& iconPath)
{
    QPushButton* btn = new QPushButton(text, sidebar);
    btn->setProperty("class", "nav-item");
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedHeight(44);

    if (!iconPath.isEmpty()) {
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(20, 20));
    }

    return btn;
}

void MainWindow::updateNavigation(int index)
{
    for (int i = 0; i < navButtons.size(); ++i) {
        navButtons[i]->setProperty("active", i == index);
        navButtons[i]->style()->unpolish(navButtons[i]);
        navButtons[i]->style()->polish(navButtons[i]);
    }
}

void MainWindow::navigateToDashboard()
{
    pageStack->setCurrentIndex(0);
    updateNavigation(0);
}

void MainWindow::navigateToTrackLocation()
{
    pageStack->setCurrentIndex(1);
    updateNavigation(1);
}

void MainWindow::navigateToAppliances()
{
    pageStack->setCurrentIndex(2);
    updateNavigation(2);
}

void MainWindow::navigateToCareRing()
{
    pageStack->setCurrentIndex(3);
    updateNavigation(3);
}

void MainWindow::navigateToResources()
{
    pageStack->setCurrentIndex(4);
    updateNavigation(4);
}