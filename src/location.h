#ifndef LOCATION_H
#define LOCATION_H

#include <QString>
#include <cmath>

struct SafetyLocation {
    int id;
    QString name;
    QString type; // hospital, police, fire, shelter, pharmacy
    QString address;
    QString phone;
    double latitude;
    double longitude;
    double distanceKm;
    
    SafetyLocation(int id = 0, const QString &name = "", const QString &type = "",
                   const QString &address = "", const QString &phone = "",
                   double lat = 0.0, double lon = 0.0, double dist = 0.0)
        : id(id), name(name), type(type), address(address), phone(phone),
          latitude(lat), longitude(lon), distanceKm(dist) {}
};

// Haversine formula for calculating distance between two coordinates
inline double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0; // Earth's radius in kilometers
    const double PI = 3.14159265358979323846;
    
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;
    
    lat1 = lat1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;
    
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    
    return R * c;
}

// Check if coordinate is within geofence
inline bool isWithinGeofence(double userLat, double userLon, 
                              double centerLat, double centerLon, 
                              double radiusMeters) {
    double distanceKm = calculateDistance(userLat, userLon, centerLat, centerLon);
    return (distanceKm * 1000) <= radiusMeters;
}

#endif // LOCATION_H
