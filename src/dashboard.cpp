#include "dashboard.h"
#include <QFrame>
#include <QPushButton>
#include <QDateTime>
#include <QMessageBox>

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    // Sample contacts
    contacts.append(Contact(1, "Sarah Johnson", "+1 (555) 123-4567", "Daughter", true));
    contacts.append(Contact(2, "Dr. Michael Chen", "+1 (555) 234-5678", "Doctor", false));
    contacts.append(Contact(3, "Emergency Services", "911", "Emergency", false));
    
    setupUi();
}

void Dashboard::setupUi()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);
    
    createHeader();
    createSOSSection();
    createStatusCards();
    createQuickContacts();
    
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void Dashboard::createHeader()
{
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(contentWidget->layout());
    
    // Greeting
    QString timeOfDay;
    int hour = QDateTime::currentDateTime().time().hour();
    if (hour < 12) timeOfDay = "Good morning";
    else if (hour < 18) timeOfDay = "Good afternoon";
    else timeOfDay = "Good evening";
    
    QLabel *greeting = new QLabel(timeOfDay, contentWidget);
    greeting->setStyleSheet("color: #a0a5b5; font-size: 16px;");
    contentLayout->addWidget(greeting);
    
    QLabel *title = new QLabel("Your Safety Dashboard", contentWidget);
    title->setStyleSheet("color: #f0f0f5; font-size: 32px; font-weight: 700;");
    contentLayout->addWidget(title);
    
    QLabel *subtitle = new QLabel("Everything is monitored and secure", contentWidget);
    subtitle->setStyleSheet("color: #6b7280; font-size: 14px; margin-bottom: 16px;");
    contentLayout->addWidget(subtitle);
}

void Dashboard::createSOSSection()
{
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(contentWidget->layout());
    
    QFrame *sosFrame = new QFrame(contentWidget);
    sosFrame->setStyleSheet(R"(
        QFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #2d1f1f, stop:1 #1a1f2e);
            border: 1px solid #3d2828;
            border-radius: 16px;
            padding: 24px;
        }
    )");
    
    QHBoxLayout *sosLayout = new QHBoxLayout(sosFrame);
    sosLayout->setContentsMargins(32, 32, 32, 32);
    sosLayout->setSpacing(32);
    
    // Left side - text
    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->setSpacing(8);
    
    QLabel *sosTitle = new QLabel("Emergency SOS", sosFrame);
    sosTitle->setStyleSheet("color: #f0f0f5; font-size: 24px; font-weight: 700;");
    textLayout->addWidget(sosTitle);
    
    QLabel *sosDesc = new QLabel(
        "Press the SOS button to immediately alert your emergency contacts "
        "and share your current location.", sosFrame);
    sosDesc->setStyleSheet("color: #a0a5b5; font-size: 14px;");
    sosDesc->setWordWrap(true);
    sosDesc->setMaximumWidth(400);
    textLayout->addWidget(sosDesc);
    
    textLayout->addStretch();
    sosLayout->addLayout(textLayout, 1);
    
    // Right side - SOS button
    sosButton = new SOSButton(sosFrame);
    connect(sosButton, &SOSButton::sosActivated, this, &Dashboard::onSOSActivated);
    sosLayout->addWidget(sosButton, 0, Qt::AlignCenter);
    
    contentLayout->addWidget(sosFrame);
}

void Dashboard::createStatusCards()
{
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(contentWidget->layout());
    
    QLabel *sectionTitle = new QLabel("Status Overview", contentWidget);
    sectionTitle->setStyleSheet("color: #f0f0f5; font-size: 18px; font-weight: 600; margin-top: 16px;");
    contentLayout->addWidget(sectionTitle);
    
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(16);
    
    // Location card
    locationCard = new StatusCard("Location Status", "Inside Safe Zone", StatusCard::Safe, contentWidget);
    locationCard->setDescription("Home - 500m radius");
    cardsLayout->addWidget(locationCard);
    
    // Appliances card
    appliancesCard = new StatusCard("Appliances", "All Secure", StatusCard::Safe, contentWidget);
    appliancesCard->setDescription("6 devices monitored");
    cardsLayout->addWidget(appliancesCard);
    
    // Care Ring card
    careRingCard = new StatusCard("Care Ring", "3 Contacts", StatusCard::Safe, contentWidget);
    careRingCard->setDescription("1 primary contact set");
    cardsLayout->addWidget(careRingCard);
    
    contentLayout->addLayout(cardsLayout);
}

void Dashboard::createQuickContacts()
{
    QVBoxLayout *contentLayout = qobject_cast<QVBoxLayout*>(contentWidget->layout());
    
    QLabel *sectionTitle = new QLabel("Quick Contacts", contentWidget);
    sectionTitle->setStyleSheet("color: #f0f0f5; font-size: 18px; font-weight: 600; margin-top: 16px;");
    contentLayout->addWidget(sectionTitle);
    
    QHBoxLayout *contactsLayout = new QHBoxLayout();
    contactsLayout->setSpacing(16);
    
    for (const Contact &contact : contacts) {
        QFrame *contactCard = new QFrame(contentWidget);
        contactCard->setStyleSheet(R"(
            QFrame {
                background-color: #232a3d;
                border: 1px solid #2d3548;
                border-radius: 12px;
            }
        )");
        
        QVBoxLayout *cardLayout = new QVBoxLayout(contactCard);
        cardLayout->setContentsMargins(20, 20, 20, 20);
        cardLayout->setSpacing(8);
        
        // Name with primary badge
        QHBoxLayout *nameLayout = new QHBoxLayout();
        QLabel *nameLabel = new QLabel(contact.name, contactCard);
        nameLabel->setStyleSheet("color: #f0f0f5; font-size: 16px; font-weight: 600;");
        nameLayout->addWidget(nameLabel);
        
        if (contact.isPrimary) {
            QLabel *badge = new QLabel("Primary", contactCard);
            badge->setStyleSheet(R"(
                background-color: #4a6cf7;
                color: #ffffff;
                border-radius: 4px;
                padding: 2px 8px;
                font-size: 11px;
                font-weight: 600;
            )");
            nameLayout->addWidget(badge);
        }
        nameLayout->addStretch();
        cardLayout->addLayout(nameLayout);
        
        QLabel *relLabel = new QLabel(contact.relationship, contactCard);
        relLabel->setStyleSheet("color: #6b7280; font-size: 13px;");
        cardLayout->addWidget(relLabel);
        
        QLabel *phoneLabel = new QLabel(contact.phone, contactCard);
        phoneLabel->setStyleSheet("color: #a0a5b5; font-size: 14px;");
        cardLayout->addWidget(phoneLabel);
        
        cardLayout->addSpacing(8);
        
        // Call button
        QPushButton *callBtn = new QPushButton("Call", contactCard);
        callBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #22c55e;
                color: #141824;
                border: none;
                border-radius: 8px;
                padding: 10px 16px;
                font-weight: 600;
            }
            QPushButton:hover {
                background-color: #16a34a;
            }
        )");
        cardLayout->addWidget(callBtn);
        
        contactsLayout->addWidget(contactCard);
    }
    
    contactsLayout->addStretch();
    contentLayout->addLayout(contactsLayout);
}

void Dashboard::onSOSActivated()
{
    QMessageBox::information(this, "SOS Activated", 
        "Emergency SOS has been activated!\n\n"
        "Your emergency contacts have been notified and your location has been shared.\n\n"
        "In a real application, this would:\n"
        "• Send SMS/calls to all emergency contacts\n"
        "• Share GPS coordinates\n"
        "• Alert monitoring services");
}
