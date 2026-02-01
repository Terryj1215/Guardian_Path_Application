#ifndef GPSMANAGER_H
#define GPSMANAGER_H

#include <QObject>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QGeoCoordinate>
#include <QTimer>
#include <cmath>

class GPSManager : public QObject
{
    Q_OBJECT

public:
    explicit GPSManager(QObject* parent = nullptr);
    ~GPSManager();

    // Start/stop GPS tracking
    void startTracking(int intervalMs = 5000);
    void stopTracking();
    bool isTracking() const;

    // Current position
    QGeoCoordinate currentPosition() const;
    double latitude() const;
    double longitude() const;
    double altitude() const;
    double speed() const;
    double direction() const;

    // Safe zone
    void setSafeZone(double centerLat, double centerLng, double radiusMeters);
    bool isWithinSafeZone() const;
    double distanceFromSafeZoneCenter() const;

    // Haversine distance calculation (meters)
    static double calculateDistance(double lat1, double lon1, double lat2, double lon2);

signals:
    void positionUpdated(double latitude, double longitude);
    void safeZoneStatusChanged(bool isWithinSafeZone);
    void errorOccurred(const QString& error);
    void trackingStarted();
    void trackingStopped();

private slots:
    void onPositionUpdated(const QGeoPositionInfo& info);
    void onError(QGeoPositionInfoSource::Error error);
    void onTimeout();

private:
    QGeoPositionInfoSource* m_source;
    QGeoCoordinate m_currentPosition;
    QTimer* m_timeoutTimer;
    bool m_tracking;

    // Safe zone parameters
    double m_safeZoneCenterLat;
    double m_safeZoneCenterLng;
    double m_safeZoneRadius;
    bool m_safeZoneSet;
    bool m_wasWithinSafeZone;

    static constexpr double EARTH_RADIUS_METERS = 6371000.0;
    static constexpr double DEG_TO_RAD = M_PI / 180.0;
};

#endif // GPSMANAGER_H
