#ifndef TRACKLOCATION_H
#define TRACKLOCATION_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QScrollArea>
#include <QQuickWidget>
#include "location.h"
#include "mongodbmanager.h"

class TrackLocation : public QWidget
{
    Q_OBJECT

public:
    explicit TrackLocation(MongoDBManager* mongo, QWidget* parent = nullptr);

public slots:
    void onMapClicked(double lat, double lon);

private slots:
    void onRadiusChanged(int value);
    void onUpdateHomeRequested();

private:
    void setupUi();
    void updateLocationDisplay();
    void updateMapProperties();

    // Missing function from your previous build error
    bool isWithinGeofence(double lat1, double lon1, double lat2, double lon2, double radiusMeters) {
        return (calculateDistance(lat1, lon1, lat2, lon2) * 1000.0) <= radiusMeters;
    }

    MongoDBManager* m_mongoManager;
    QQuickWidget* mapWidget;

    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QLabel* latitudeLabel;
    QLabel* longitudeLabel;
    QLabel* statusLabel;
    QLabel* radiusValueLabel;
    QLabel* homeCoords;
    QSlider* radiusSlider;

    double currentLatitude;
    double currentLongitude;
    double homeLatitude;
    double homeLongitude;
    int safeZoneRadius;
};

#endif