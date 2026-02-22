#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>

// Include headers for the individual screens
#include "tracklocationwindow.h"
#include "monitorappliancewindow.h"
#include "resourceswindow.h"
#include "careringwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Create the QStackedWidget and set it as the central widget
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Create the main menu and add it to the stacked widget
    setupMainMenu();
    m_stackedWidget->addWidget(m_mainMenu);

    // Create the other screens and add them to the stacked widget
    m_trackLocationWindow = new TrackLocationWindow(this);
    m_stackedWidget->addWidget(m_trackLocationWindow);

    m_monitorApplianceWindow = new MonitorApplianceWindow(this);
    m_stackedWidget->addWidget(m_monitorApplianceWindow);

    m_resourcesWindow = new ResourcesWindow(this);
    m_stackedWidget->addWidget(m_resourcesWindow);

    m_careRingWindow = new CareRingWindow(this);
    m_stackedWidget->addWidget(m_careRingWindow);

    // Connect the "Back" button on each secondary screen to the onBackToMainClicked slot
    connect(m_trackLocationWindow, &TrackLocationWindow::backToMainRequested, this, &MainWindow::onBackToMainClicked);
    connect(m_monitorApplianceWindow, &MonitorApplianceWindow::backToMainRequested, this, &MainWindow::onBackToMainClicked);
    connect(m_resourcesWindow, &ResourcesWindow::backToMainRequested, this, &MainWindow::onBackToMainClicked);
    connect(m_careRingWindow, &CareRingWindow::backToMainRequested, this, &MainWindow::onBackToMainClicked);

    // Initially show the main menu
    m_stackedWidget->setCurrentWidget(m_mainMenu);
}

MainWindow::~MainWindow()
{
    // The QStackedWidget will automatically delete its child widgets
    // when it is destroyed, so we don't need to manually delete them.
}

void MainWindow::setupMainMenu()
{
    m_mainMenu = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(m_mainMenu);

    // Create and style the main menu label
    QLabel* titleLabel = new QLabel("Guardian Path", m_mainMenu);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font = titleLabel->font();
    font.setPointSize(24);
    font.setBold(true);
    titleLabel->setFont(font);
    layout->addWidget(titleLabel);

    // Create the four buttons
    QPushButton* trackLocationBtn = new QPushButton("Track Location", m_mainMenu);
    QPushButton* monitorApplianceBtn = new QPushButton("Monitor Appliance", m_mainMenu);
    QPushButton* resourcesBtn = new QPushButton("Resources", m_mainMenu);
    QPushButton* careRingBtn = new QPushButton("Care Ring", m_mainMenu);

    layout->addWidget(trackLocationBtn);
    layout->addWidget(monitorApplianceBtn);
    layout->addWidget(resourcesBtn);
    layout->addWidget(careRingBtn);

    // Connect the button clicks to their corresponding slots
    connect(trackLocationBtn, &QPushButton::clicked, this, &MainWindow::onTrackLocationClicked);
    connect(monitorApplianceBtn, &QPushButton::clicked, this, &MainWindow::onMonitorApplianceClicked);
    connect(resourcesBtn, &QPushButton::clicked, this, &MainWindow::onResourcesClicked);
    connect(careRingBtn, &QPushButton::clicked, this, &MainWindow::onCareRingClicked);
}

void MainWindow::onTrackLocationClicked()
{
    // Switch to the Track Location screen
    m_stackedWidget->setCurrentWidget(m_trackLocationWindow);
}

void MainWindow::onMonitorApplianceClicked()
{
    // Switch to the Monitor Appliance screen
    m_stackedWidget->setCurrentWidget(m_monitorApplianceWindow);
}

void MainWindow::onResourcesClicked()
{
    // Switch to the Resources screen
    m_stackedWidget->setCurrentWidget(m_resourcesWindow);
}

void MainWindow::onCareRingClicked()
{
    // Switch to the Care Ring screen
    m_stackedWidget->setCurrentWidget(m_careRingWindow);
}

void MainWindow::onBackToMainClicked()
{
    // Switch back to the main menu screen
    m_stackedWidget->setCurrentWidget(m_mainMenu);
}