#ifndef CONTACT_H
#define CONTACT_H

#include <QString>

struct Contact {
    int id;
    QString name;
    QString phone;
    QString relationship;
    bool isPrimary;
    
    Contact(int id = 0, const QString &name = "", const QString &phone = "", 
            const QString &relationship = "", bool isPrimary = false)
        : id(id), name(name), phone(phone), relationship(relationship), isPrimary(isPrimary) {}
};

#endif // CONTACT_H
