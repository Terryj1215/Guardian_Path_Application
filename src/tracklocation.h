#ifndef TRACKLOCATION_H
#define TRACKLOCATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QScrollArea>
#include "location.h"

class TrackLocation : public QWidget
{
    Q_OBJECT

public:
    explicit TrackLocation(QWidget *parent = nullptr);

private slots:
    void onRadiusChanged(int value);

private:
    void setupUi();
    void updateLocationDisplay();
    
    QScrollArea *scrollArea;
    QWidget *contentWidget;
    
    QLabel *latitudeLabel;
    QLabel *longitudeLabel;
    QLabel *statusLabel;
    QLabel *radiusValueLabel;
    QSlider *radiusSlider;
    QWidget *mapPlaceholder;
    
    double currentLatitude;
    double currentLongitude;
    double homeLatitude;
    double homeLongitude;
    int safeZoneRadius; // in meters
};

#endif // TRACKLOCATION_H
