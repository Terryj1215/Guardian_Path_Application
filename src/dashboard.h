#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "statuscard.h"
#include "sosbutton.h"
#include "contact.h"

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

private slots:
    void onSOSActivated();

private:
    void setupUi();
    void createHeader();
    void createSOSSection();
    void createStatusCards();
    void createQuickContacts();
    
    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QWidget *contentWidget;
    
    SOSButton *sosButton;
    StatusCard *locationCard;
    StatusCard *appliancesCard;
    StatusCard *careRingCard;
    
    QList<Contact> contacts;
};

#endif // DASHBOARD_H
