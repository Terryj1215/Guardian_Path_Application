#ifndef APPLIANCE_H
#define APPLIANCE_H

#include <QString>

struct Appliance {
    int id;
    QString name;
    QString type;
    bool isOn;
    bool isSafe;
    QString lastChecked;
    
    Appliance(int id = 0, const QString &name = "", const QString &type = "",
              bool isOn = false, bool isSafe = true, const QString &lastChecked = "")
        : id(id), name(name), type(type), isOn(isOn), isSafe(isSafe), lastChecked(lastChecked) {}
};

#endif // APPLIANCE_H
