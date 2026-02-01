#include "tracklocation.h"
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QQmlContext>
#include <QQuickItem>

TrackLocation::TrackLocation(MongoDBManager* mongo, QWidget* parent)
    : QWidget(parent),
    m_mongoManager(mongo),
    currentLatitude(40.7128),
    currentLongitude(-74.0060),
    homeLatitude(40.7128),
    homeLongitude(-74.0060),
    safeZoneRadius(500)
{
    setupUi();
    updateLocationDisplay();
}

void TrackLocation::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    QLabel* title = new QLabel("Track Location", contentWidget);
    title->setStyleSheet("color: #f0f0f5; font-size: 32px; font-weight: 700;");
    contentLayout->addWidget(title);

    QHBoxLayout* mainContent = new QHBoxLayout();

    // MAP WIDGET
    mapWidget = new QQuickWidget(contentWidget);
    mapWidget->setMinimumSize(600, 500);
    mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mapWidget->rootContext()->setContextProperty("trackLocationWidget", this);
    mapWidget->setSource(QUrl::fromLocalFile("../../../src/mapview.qml"));

    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(mapWidget);

    QFrame* coordsFrame = new QFrame(contentWidget);
    coordsFrame->setStyleSheet("background-color: #232a3d; border-radius: 12px; padding: 15px;");
    QHBoxLayout* coordsLayout = new QHBoxLayout(coordsFrame);

    latitudeLabel = new QLabel(coordsFrame);
    longitudeLabel = new QLabel(coordsFrame);
    statusLabel = new QLabel(coordsFrame);

    coordsLayout->addWidget(new QLabel("Lat:", coordsFrame));
    coordsLayout->addWidget(latitudeLabel);
    coordsLayout->addWidget(new QLabel("Lon:", coordsFrame));
    coordsLayout->addWidget(longitudeLabel);
    coordsLayout->addWidget(statusLabel);

    leftLayout->addWidget(coordsFrame);
    mainContent->addLayout(leftLayout, 2);

    // SETTINGS SIDE
    QVBoxLayout* rightLayout = new QVBoxLayout();
    QFrame* settingsFrame = new QFrame(contentWidget);
    settingsFrame->setStyleSheet("background-color: #232a3d; border-radius: 12px;");
    QVBoxLayout* settingsLayout = new QVBoxLayout(settingsFrame);

    radiusValueLabel = new QLabel("500 meters", settingsFrame);
    radiusSlider = new QSlider(Qt::Horizontal, settingsFrame);
    radiusSlider->setRange(100, 5000);
    radiusSlider->setValue(500);
    connect(radiusSlider, &QSlider::valueChanged, this, &TrackLocation::onRadiusChanged);

    homeCoords = new QLabel(settingsFrame);
    QPushButton* updateHomeBtn = new QPushButton("Set Map Center as Home", settingsFrame);
    updateHomeBtn->setStyleSheet("background-color: #4a6cf7; color: white; padding: 12px; font-weight: 600; border-radius: 8px;");
    connect(updateHomeBtn, &QPushButton::clicked, this, &TrackLocation::onUpdateHomeRequested);

    settingsLayout->addWidget(new QLabel("Geofence Radius", settingsFrame));
    settingsLayout->addWidget(radiusValueLabel);
    settingsLayout->addWidget(radiusSlider);
    settingsLayout->addWidget(new QLabel("Current Home:", settingsFrame));
    settingsLayout->addWidget(homeCoords);
    settingsLayout->addWidget(updateHomeBtn);

    rightLayout->addWidget(settingsFrame);
    rightLayout->addStretch();
    mainContent->addLayout(rightLayout, 1);

    contentLayout->addLayout(mainContent);
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void TrackLocation::onMapClicked(double lat, double lon)
{
    currentLatitude = lat;
    currentLongitude = lon;
    updateLocationDisplay();
    updateMapProperties();
}

void TrackLocation::onUpdateHomeRequested()
{
    homeLatitude = currentLatitude;
    homeLongitude = currentLongitude;

    if (m_mongoManager) {
        m_mongoManager->saveSafeZone(homeLatitude, homeLongitude, (double)safeZoneRadius, "Home");
        QMessageBox::information(this, "Safe Zone", "Home location updated successfully.");
    }
    updateLocationDisplay();
    updateMapProperties();
}

void TrackLocation::onRadiusChanged(int value)
{
    safeZoneRadius = value;
    radiusValueLabel->setText(QString("%1 meters").arg(value));
    updateLocationDisplay();
    updateMapProperties();
}

void TrackLocation::updateMapProperties()
{
    if (mapWidget && mapWidget->rootObject()) {
        mapWidget->rootObject()->setProperty("currentLat", currentLatitude);
        mapWidget->rootObject()->setProperty("currentLon", currentLongitude);
        mapWidget->rootObject()->setProperty("homeLat", homeLatitude);
        mapWidget->rootObject()->setProperty("homeLon", homeLongitude);
        mapWidget->rootObject()->setProperty("radius", safeZoneRadius);
    }
}

void TrackLocation::updateLocationDisplay()
{
    latitudeLabel->setText(QString::number(currentLatitude, 'f', 4));
    longitudeLabel->setText(QString::number(currentLongitude, 'f', 4));
    homeCoords->setText(QString("Lat: %1, Lon: %2").arg(homeLatitude, 'f', 4).arg(homeLongitude, 'f', 4));

    bool isInside = isWithinGeofence(currentLatitude, currentLongitude, homeLatitude, homeLongitude, safeZoneRadius);
    statusLabel->setText(isInside ? "SAFE" : "ALARM");
    statusLabel->setStyleSheet(isInside ? "color: #22c55e; font-weight: bold;" : "color: #dc2626; font-weight: bold;");
}