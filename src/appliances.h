#ifndef APPLIANCES_H
#define APPLIANCES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QList>
#include "appliance.h"
#include <Qlabel.h>


class Appliances : public QWidget
{
    Q_OBJECT

public:
    explicit Appliances(QWidget *parent = nullptr);

private slots:
    void onApplianceToggled(int id, bool isOn);

private:
    void setupUi();
    void createApplianceCard(const Appliance &appliance, QLayout *layout);
    void updateOverviewCard();
    
    QScrollArea *scrollArea;
    QWidget *contentWidget;
    QWidget *overviewCard;
    QLabel *overviewStatusLabel;
    QLabel *overviewCountLabel;
    
    QList<Appliance> appliances;
};

#endif // APPLIANCES_H
