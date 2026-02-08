#ifndef CARERING_H
#define CARERING_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QList>
#include "contact.h"

class MongoDBManager;

class CareRing : public QWidget
{
    Q_OBJECT

public:
    explicit CareRing(MongoDBManager* mongo, QWidget* parent = nullptr);

private slots:
    void onAddContact();
    void onDeleteContact(int index);
    void onSetPrimary(int index);
    void onCallContact(const Contact& contact);
    void onMessageContact(const Contact& contact);

private:
    void setupUi();
    void refreshContactList();
    void loadContactsFromDB();
    QWidget* createContactCard(const Contact& contact, int index);

    MongoDBManager* m_mongoManager;
    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QVBoxLayout* contactsLayout;
    QList<Contact> contacts;
    int nextContactId;
};

#endif // CARERING_H