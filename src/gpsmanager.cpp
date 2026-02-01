#include "gpsmanager.h"
#include <QDebug>

GPSManager::GPSManager(QObject* parent)
    : QObject(parent)
    , m_source(nullptr)
    , m_timeoutTimer(new QTimer(this))
    , m_tracking(false)
    , m_safeZoneCenterLat(0.0)
    , m_safeZoneCenterLng(0.0)
    , m_safeZoneRadius(500.0)
    , m_safeZoneSet(false)
    , m_wasWithinSafeZone(true)
{
    // Create GPS source
    m_source = QGeoPositionInfoSource::createDefaultSource(this);

    if (m_source) {
        connect(m_source, &QGeoPositionInfoSource::positionUpdated,
            this, &GPSManager::onPositionUpdated);
        connect(m_source, &QGeoPositionInfoSource::errorOccurred,
            this, &GPSManager::onError);

        qDebug() << "GPS source created:" << m_source->sourceName();
    }
    else {
        qWarning() << "No GPS source available";
        emit errorOccurred("No GPS source available on this device");
    }

    // Timeout timer for when GPS takes too long
    m_timeoutTimer->setSingleShot(true);
    connect(m_timeoutTimer, &QTimer::timeout, this, &GPSManager::onTimeout);
}

GPSManager::~GPSManager()
{
    stopTracking();
}

void GPSManager::startTracking(int intervalMs)
{
    if (!m_source) {
        emit errorOccurred("GPS not available");
        return;
    }

    m_source->setUpdateInterval(intervalMs);
    m_source->startUpdates();
    m_tracking = true;
    m_timeoutTimer->start(30000); // 30 second timeout

    emit trackingStarted();
    qDebug() << "GPS tracking started, interval:" << intervalMs << "ms";
}

void GPSManager::stopTracking()
{
    if (m_source && m_tracking) {
        m_source->stopUpdates();
        m_tracking = false;
        m_timeoutTimer->stop();
        emit trackingStopped();
        qDebug() << "GPS tracking stopped";
    }
}

bool GPSManager::isTracking() const
{
    return m_tracking;
}

QGeoCoordinate GPSManager::currentPosition() const
{
    return m_currentPosition;
}

double GPSManager::latitude() const
{
    return m_currentPosition.latitude();
}

double GPSManager::longitude() const
{
    return m_currentPosition.longitude();
}

double GPSManager::altitude() const
{
    return m_currentPosition.altitude();
}

double GPSManager::speed() const
{
    // Speed would come from the position info, not stored here
    return 0.0;
}

double GPSManager::direction() const
{
    return 0.0;
}

void GPSManager::setSafeZone(double centerLat, double centerLng, double radiusMeters)
{
    m_safeZoneCenterLat = centerLat;
    m_safeZoneCenterLng = centerLng;
    m_safeZoneRadius = radiusMeters;
    m_safeZoneSet = true;

    qDebug() << "Safe zone set:" << centerLat << centerLng << "radius:" << radiusMeters << "m";

    // Check current position against new safe zone
    if (m_currentPosition.isValid()) {
        bool withinZone = isWithinSafeZone();
        if (withinZone != m_wasWithinSafeZone) {
            m_wasWithinSafeZone = withinZone;
            emit safeZoneStatusChanged(withinZone);
        }
    }
}

bool GPSManager::isWithinSafeZone() const
{
    if (!m_safeZoneSet || !m_currentPosition.isValid()) {
        return true; // Assume safe if no zone set or no position
    }

    double distance = distanceFromSafeZoneCenter();
    return distance <= m_safeZoneRadius;
}

double GPSManager::distanceFromSafeZoneCenter() const
{
    if (!m_safeZoneSet || !m_currentPosition.isValid()) {
        return 0.0;
    }

    return calculateDistance(
        m_currentPosition.latitude(),
        m_currentPosition.longitude(),
        m_safeZoneCenterLat,
        m_safeZoneCenterLng
    );
}

double GPSManager::calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    // Haversine formula
    double dLat = (lat2 - lat1) * DEG_TO_RAD;
    double dLon = (lon2 - lon1) * DEG_TO_RAD;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
        std::cos(lat1 * DEG_TO_RAD) * std::cos(lat2 * DEG_TO_RAD) *
        std::sin(dLon / 2) * std::sin(dLon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return EARTH_RADIUS_METERS * c;
}

void GPSManager::onPositionUpdated(const QGeoPositionInfo& info)
{
    m_timeoutTimer->stop();

    if (info.isValid()) {
        m_currentPosition = info.coordinate();

        qDebug() << "Position updated:" << m_currentPosition.latitude()
            << m_currentPosition.longitude();

        emit positionUpdated(m_currentPosition.latitude(), m_currentPosition.longitude());

        // Check safe zone status
        if (m_safeZoneSet) {
            bool withinZone = isWithinSafeZone();
            if (withinZone != m_wasWithinSafeZone) {
                m_wasWithinSafeZone = withinZone;
                emit safeZoneStatusChanged(withinZone);
                qDebug() << "Safe zone status changed:" << (withinZone ? "Inside" : "Outside");
            }
        }
    }

    // Restart timeout timer for next update
    if (m_tracking) {
        m_timeoutTimer->start(30000);
    }
}

void GPSManager::onError(QGeoPositionInfoSource::Error error)
{
    QString errorMsg;
    switch (error) {
    case QGeoPositionInfoSource::AccessError:
        errorMsg = "GPS access denied. Please enable location permissions.";
        break;
    case QGeoPositionInfoSource::ClosedError:
        errorMsg = "GPS connection closed unexpectedly.";
        break;
    case QGeoPositionInfoSource::NoError:
        return;
    default:
        errorMsg = "Unknown GPS error occurred.";
        break;
    }

    qWarning() << "GPS error:" << errorMsg;
    emit errorOccurred(errorMsg);
}

void GPSManager::onTimeout()
{
    qWarning() << "GPS timeout - no position received";
    emit errorOccurred("GPS timeout. Unable to get location.");
}
