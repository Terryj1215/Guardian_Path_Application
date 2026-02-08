#ifndef CONTACT_H
#define CONTACT_H

#include <QString>

struct Contact {
    QString id; // Changed from int to QString to support MongoDB ObjectIDs
    QString name;
    QString phone;
    QString relationship;
    bool isPrimary;

    // Updated constructor to handle the QString id
    Contact(const QString& id = "",
        const QString& name = "",
        const QString& phone = "",
        const QString& relationship = "",
        bool isPrimary = false)
        : id(id),
        name(name),
        phone(phone),
        relationship(relationship),
        isPrimary(isPrimary) {
    }
};

#endif // CONTACT_H