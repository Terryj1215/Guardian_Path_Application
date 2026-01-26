#include "tracklocation.h"
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>

TrackLocation::TrackLocation(QWidget *parent)
    : QWidget(parent),
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
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);
    
    // Header
    QLabel *title = new QLabel("Track Location", contentWidget);
    title->setStyleSheet("color: #f0f0f5; font-size: 32px; font-weight: 700;");
    contentLayout->addWidget(title);
    
    QLabel *subtitle = new QLabel("Monitor your location and set safe zone boundaries", contentWidget);
    subtitle->setStyleSheet("color: #6b7280; font-size: 14px; margin-bottom: 16px;");
    contentLayout->addWidget(subtitle);
    
    // Main content grid
    QHBoxLayout *mainContent = new QHBoxLayout();
    mainContent->setSpacing(24);
    
    // Left side - Map and coordinates
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(16);
    
    // Map placeholder
    mapPlaceholder = new QWidget(contentWidget);
    mapPlaceholder->setMinimumSize(500, 400);
    mapPlaceholder->setStyleSheet(R"(
        background-color: #1a2035;
        border: 2px solid #2d3548;
        border-radius: 16px;
    )");
    
    QVBoxLayout *mapLayout = new QVBoxLayout(mapPlaceholder);
    mapLayout->setAlignment(Qt::AlignCenter);
    
    // Geofence visualization
    QFrame *geofenceCircle = new QFrame(mapPlaceholder);
    geofenceCircle->setFixedSize(200, 200);
    geofenceCircle->setStyleSheet(R"(
        background-color: rgba(74, 108, 247, 0.15);
        border: 3px solid #4a6cf7;
        border-radius: 100px;
    )");
    
    QVBoxLayout *circleLayout = new QVBoxLayout(geofenceCircle);
    circleLayout->setAlignment(Qt::AlignCenter);
    
    QLabel *pinLabel = new QLabel("📍", geofenceCircle);
    pinLabel->setStyleSheet("font-size: 32px;");
    pinLabel->setAlignment(Qt::AlignCenter);
    circleLayout->addWidget(pinLabel);
    
    QLabel *homeLabel = new QLabel("Home", geofenceCircle);
    homeLabel->setStyleSheet("color: #4a6cf7; font-weight: 600;");
    homeLabel->setAlignment(Qt::AlignCenter);
    circleLayout->addWidget(homeLabel);
    
    mapLayout->addWidget(geofenceCircle, 0, Qt::AlignCenter);
    
    QLabel *mapNote = new QLabel("Geofence Visualization", mapPlaceholder);
    mapNote->setStyleSheet("color: #6b7280; font-size: 12px; margin-top: 16px;");
    mapNote->setAlignment(Qt::AlignCenter);
    mapLayout->addWidget(mapNote);
    
    leftLayout->addWidget(mapPlaceholder);
    
    // Coordinates display
    QFrame *coordsFrame = new QFrame(contentWidget);
    coordsFrame->setStyleSheet(R"(
        QFrame {
            background-color: #232a3d;
            border: 1px solid #2d3548;
            border-radius: 12px;
        }
    )");
    
    QHBoxLayout *coordsLayout = new QHBoxLayout(coordsFrame);
    coordsLayout->setContentsMargins(20, 20, 20, 20);
    coordsLayout->setSpacing(32);
    
    // Latitude
    QVBoxLayout *latLayout = new QVBoxLayout();
    QLabel *latTitle = new QLabel("Latitude", coordsFrame);
    latTitle->setStyleSheet("color: #a0a5b5; font-size: 13px;");
    latitudeLabel = new QLabel("0.0000", coordsFrame);
    latitudeLabel->setStyleSheet("color: #f0f0f5; font-size: 20px; font-weight: 600;");
    latLayout->addWidget(latTitle);
    latLayout->addWidget(latitudeLabel);
    coordsLayout->addLayout(latLayout);
    
    // Longitude
    QVBoxLayout *lonLayout = new QVBoxLayout();
    QLabel *lonTitle = new QLabel("Longitude", coordsFrame);
    lonTitle->setStyleSheet("color: #a0a5b5; font-size: 13px;");
    longitudeLabel = new QLabel("0.0000", coordsFrame);
    longitudeLabel->setStyleSheet("color: #f0f0f5; font-size: 20px; font-weight: 600;");
    lonLayout->addWidget(lonTitle);
    lonLayout->addWidget(longitudeLabel);
    coordsLayout->addLayout(lonLayout);
    
    // Status
    QVBoxLayout *statusLayout = new QVBoxLayout();
    QLabel *statusTitle = new QLabel("Status", coordsFrame);
    statusTitle->setStyleSheet("color: #a0a5b5; font-size: 13px;");
    statusLabel = new QLabel("Inside Safe Zone", coordsFrame);
    statusLabel->setStyleSheet(R"(
        background-color: #22c55e;
        color: #141824;
        border-radius: 4px;
        padding: 4px 12px;
        font-weight: 600;
    )");
    statusLayout->addWidget(statusTitle);
    statusLayout->addWidget(statusLabel);
    coordsLayout->addLayout(statusLayout);
    
    coordsLayout->addStretch();
    leftLayout->addWidget(coordsFrame);
    
    mainContent->addLayout(leftLayout, 2);
    
    // Right side - Settings
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(16);
    
    QFrame *settingsFrame = new QFrame(contentWidget);
    settingsFrame->setStyleSheet(R"(
        QFrame {
            background-color: #232a3d;
            border: 1px solid #2d3548;
            border-radius: 12px;
        }
    )");
    
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsFrame);
    settingsLayout->setContentsMargins(24, 24, 24, 24);
    settingsLayout->setSpacing(20);
    
    QLabel *settingsTitle = new QLabel("Safe Zone Settings", settingsFrame);
    settingsTitle->setStyleSheet("color: #f0f0f5; font-size: 18px; font-weight: 600;");
    settingsLayout->addWidget(settingsTitle);
    
    // Radius slider
    QLabel *radiusTitle = new QLabel("Safe Zone Radius", settingsFrame);
    radiusTitle->setStyleSheet("color: #a0a5b5; font-size: 13px;");
    settingsLayout->addWidget(radiusTitle);
    
    radiusValueLabel = new QLabel("500 meters", settingsFrame);
    radiusValueLabel->setStyleSheet("color: #f0f0f5; font-size: 24px; font-weight: 700;");
    settingsLayout->addWidget(radiusValueLabel);
    
    radiusSlider = new QSlider(Qt::Horizontal, settingsFrame);
    radiusSlider->setRange(100, 2000);
    radiusSlider->setValue(500);
    radiusSlider->setTickInterval(100);
    connect(radiusSlider, &QSlider::valueChanged, this, &TrackLocation::onRadiusChanged);
    settingsLayout->addWidget(radiusSlider);
    
    QHBoxLayout *rangeLabels = new QHBoxLayout();
    QLabel *minLabel = new QLabel("100m", settingsFrame);
    minLabel->setStyleSheet("color: #6b7280; font-size: 12px;");
    QLabel *maxLabel = new QLabel("2000m", settingsFrame);
    maxLabel->setStyleSheet("color: #6b7280; font-size: 12px;");
    rangeLabels->addWidget(minLabel);
    rangeLabels->addStretch();
    rangeLabels->addWidget(maxLabel);
    settingsLayout->addLayout(rangeLabels);
    
    settingsLayout->addSpacing(16);
    
    // Home location
    QLabel *homeTitle = new QLabel("Home Location", settingsFrame);
    homeTitle->setStyleSheet("color: #a0a5b5; font-size: 13px; margin-top: 16px;");
    settingsLayout->addWidget(homeTitle);
    
    QLabel *homeCoords = new QLabel(
        QString("Lat: %1\nLon: %2").arg(homeLatitude, 0, 'f', 4).arg(homeLongitude, 0, 'f', 4),
        settingsFrame);
    homeCoords->setStyleSheet("color: #f0f0f5; font-size: 14px;");
    settingsLayout->addWidget(homeCoords);
    
    QPushButton *updateHomeBtn = new QPushButton("Update Home Location", settingsFrame);
    updateHomeBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a6cf7;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 12px 20px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #5b7af8;
        }
    )");
    settingsLayout->addWidget(updateHomeBtn);
    
    settingsLayout->addStretch();
    
    // Haversine formula reference
    QFrame *formulaFrame = new QFrame(settingsFrame);
    formulaFrame->setStyleSheet("background-color: #1a1f2e; border-radius: 8px; padding: 12px;");
    QVBoxLayout *formulaLayout = new QVBoxLayout(formulaFrame);
    formulaLayout->setContentsMargins(16, 16, 16, 16);
    
    QLabel *formulaTitle = new QLabel("Distance Calculation", formulaFrame);
    formulaTitle->setStyleSheet("color: #a0a5b5; font-size: 12px; font-weight: 600;");
    formulaLayout->addWidget(formulaTitle);
    
    QLabel *formulaText = new QLabel("Using Haversine formula:\na = sin²(Δφ/2) + cos(φ1)·cos(φ2)·sin²(Δλ/2)\nc = 2·atan2(√a, √(1-a))\nd = R·c", formulaFrame);
    formulaText->setStyleSheet("color: #6b7280; font-size: 11px; font-family: monospace;");
    formulaLayout->addWidget(formulaText);
    
    settingsLayout->addWidget(formulaFrame);
    
    rightLayout->addWidget(settingsFrame);
    rightLayout->addStretch();
    
    mainContent->addLayout(rightLayout, 1);
    
    contentLayout->addLayout(mainContent);
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void TrackLocation::onRadiusChanged(int value)
{
    safeZoneRadius = value;
    radiusValueLabel->setText(QString("%1 meters").arg(value));
    updateLocationDisplay();
}

void TrackLocation::updateLocationDisplay()
{
    latitudeLabel->setText(QString::number(currentLatitude, 'f', 4));
    longitudeLabel->setText(QString::number(currentLongitude, 'f', 4));
    
    bool isInside = isWithinGeofence(currentLatitude, currentLongitude, 
                                      homeLatitude, homeLongitude, safeZoneRadius);
    
    if (isInside) {
        statusLabel->setText("Inside Safe Zone");
        statusLabel->setStyleSheet(R"(
            background-color: #22c55e;
            color: #141824;
            border-radius: 4px;
            padding: 4px 12px;
            font-weight: 600;
        )");
    } else {
        statusLabel->setText("Outside Safe Zone");
        statusLabel->setStyleSheet(R"(
            background-color: #dc2626;
            color: #ffffff;
            border-radius: 4px;
            padding: 4px 12px;
            font-weight: 600;
        )");
    }
}
