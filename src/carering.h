#ifndef CARERING_H
#define CARERING_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QList>
#include "contact.h"

class CareRing : public QWidget
{
    Q_OBJECT

public:
    explicit CareRing(QWidget *parent = nullptr);

private slots:
    void onAddContact();
    void onDeleteContact(int id);
    void onSetPrimary(int id);
    void onCallContact(const Contact &contact);
    void onMessageContact(const Contact &contact);

private:
    void setupUi();
    void refreshContactList();
    QWidget* createContactCard(const Contact &contact);
    
    QScrollArea *scrollArea;
    QWidget *contentWidget;
    QVBoxLayout *contactsLayout;
    
    QList<Contact> contacts;
    int nextContactId;
};

#endif // CARERING_H
