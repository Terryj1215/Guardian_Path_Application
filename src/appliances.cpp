#include "appliances.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>

Appliances::Appliances(QWidget *parent)
    : QWidget(parent)
{
    // Sample appliances
    appliances.append(Appliance(1, "Kitchen Stove", "stove", false, true, "2 min ago"));
    appliances.append(Appliance(2, "Front Door", "door", false, true, "Just now"));
    appliances.append(Appliance(3, "Living Room Lights", "light", true, true, "5 min ago"));
    appliances.append(Appliance(4, "Water Heater", "heater", true, true, "10 min ago"));
    appliances.append(Appliance(5, "Garage Door", "door", false, true, "15 min ago"));
    appliances.append(Appliance(6, "HVAC System", "hvac", true, true, "1 min ago"));
    
    setupUi();
}

void Appliances::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);
    
    // Header
    QLabel *title = new QLabel("Monitor Appliances", contentWidget);
    title->setStyleSheet("color: #f0f0f5; font-size: 32px; font-weight: 700;");
    contentLayout->addWidget(title);
    
    QLabel *subtitle = new QLabel("Control and monitor your home appliances for safety", contentWidget);
    subtitle->setStyleSheet("color: #6b7280; font-size: 14px; margin-bottom: 16px;");
    contentLayout->addWidget(subtitle);
    
    // Overview card
    overviewCard = new QWidget(contentWidget);
    overviewCard->setStyleSheet(R"(
        background-color: #232a3d;
        border: 1px solid #2d3548;
        border-radius: 12px;
    )");
    
    QHBoxLayout *overviewLayout = new QHBoxLayout(overviewCard);
    overviewLayout->setContentsMargins(24, 24, 24, 24);
    overviewLayout->setSpacing(24);
    
    QVBoxLayout *overviewText = new QVBoxLayout();
    QLabel *overviewTitle = new QLabel("Overall Status", overviewCard);
    overviewTitle->setStyleSheet("color: #a0a5b5; font-size: 13px;");
    overviewText->addWidget(overviewTitle);
    
    overviewStatusLabel = new QLabel("All Appliances Safe", overviewCard);
    overviewStatusLabel->setStyleSheet("color: #f0f0f5; font-size: 24px; font-weight: 700;");
    overviewText->addWidget(overviewStatusLabel);
    
    overviewCountLabel = new QLabel("6 devices monitored", overviewCard);
    overviewCountLabel->setStyleSheet("color: #6b7280; font-size: 14px;");
    overviewText->addWidget(overviewCountLabel);
    
    overviewLayout->addLayout(overviewText);
    overviewLayout->addStretch();
    
    QLabel *statusBadge = new QLabel("Safe", overviewCard);
    statusBadge->setStyleSheet(R"(
        background-color: #22c55e;
        color: #141824;
        border-radius: 4px;
        padding: 8px 16px;
        font-weight: 600;
        font-size: 14px;
    )");
    overviewLayout->addWidget(statusBadge, 0, Qt::AlignCenter);
    
    contentLayout->addWidget(overviewCard);
    
    // Appliances grid
    QLabel *gridTitle = new QLabel("Your Appliances", contentWidget);
    gridTitle->setStyleSheet("color: #f0f0f5; font-size: 18px; font-weight: 600; margin-top: 8px;");
    contentLayout->addWidget(gridTitle);
    
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(16);
    
    int row = 0, col = 0;
    for (const Appliance &appliance : appliances) {
        createApplianceCard(appliance, gridLayout);
        
        QWidget *card = gridLayout->itemAt(gridLayout->count() - 1)->widget();
        gridLayout->addWidget(card, row, col);
        
        col++;
        if (col >= 3) {
            col = 0;
            row++;
        }
    }
    
    contentLayout->addLayout(gridLayout);
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void Appliances::createApplianceCard(const Appliance &appliance, QLayout *layout)
{
    QFrame *card = new QFrame(contentWidget);
    card->setStyleSheet(R"(
        QFrame {
            background-color: #232a3d;
            border: 1px solid #2d3548;
            border-radius: 12px;
        }
    )");
    card->setMinimumSize(280, 180);
    
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(12);
    
    // Header with icon and status
    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    // Icon based on type
    QString icon = "🔌";
    if (appliance.type == "stove") icon = "🍳";
    else if (appliance.type == "door") icon = "🚪";
    else if (appliance.type == "light") icon = "💡";
    else if (appliance.type == "heater") icon = "🔥";
    else if (appliance.type == "hvac") icon = "❄️";
    
    QLabel *iconLabel = new QLabel(icon, card);
    iconLabel->setStyleSheet("font-size: 32px;");
    headerLayout->addWidget(iconLabel);
    
    headerLayout->addStretch();
    
    // Status badge
    QLabel *statusBadge = new QLabel(appliance.isSafe ? "Safe" : "Alert", card);
    if (appliance.isSafe) {
        statusBadge->setStyleSheet(R"(
            background-color: #22c55e;
            color: #141824;
            border-radius: 4px;
            padding: 4px 8px;
            font-weight: 600;
            font-size: 11px;
        )");
    } else {
        statusBadge->setStyleSheet(R"(
            background-color: #dc2626;
            color: #ffffff;
            border-radius: 4px;
            padding: 4px 8px;
            font-weight: 600;
            font-size: 11px;
        )");
    }
    headerLayout->addWidget(statusBadge);
    
    cardLayout->addLayout(headerLayout);
    
    // Name
    QLabel *nameLabel = new QLabel(appliance.name, card);
    nameLabel->setStyleSheet("color: #f0f0f5; font-size: 16px; font-weight: 600;");
    cardLayout->addWidget(nameLabel);
    
    // Last checked
    QLabel *checkedLabel = new QLabel("Last checked: " + appliance.lastChecked, card);
    checkedLabel->setStyleSheet("color: #6b7280; font-size: 12px;");
    cardLayout->addWidget(checkedLabel);
    
    cardLayout->addStretch();
    
    // Toggle row
    QHBoxLayout *toggleLayout = new QHBoxLayout();
    
    QLabel *stateLabel = new QLabel(appliance.isOn ? "On" : "Off", card);
    stateLabel->setStyleSheet("color: #a0a5b5; font-size: 14px;");
    toggleLayout->addWidget(stateLabel);
    
    toggleLayout->addStretch();
    
    // Toggle switch (using styled checkbox)
    QCheckBox *toggle = new QCheckBox(card);
    toggle->setChecked(appliance.isOn);
    toggle->setStyleSheet(R"(
        QCheckBox::indicator {
            width: 44px;
            height: 24px;
            border-radius: 12px;
            background-color: #3d4560;
        }
        QCheckBox::indicator:checked {
            background-color: #22c55e;
        }
    )");
    
    int applianceId = appliance.id;
    connect(toggle, &QCheckBox::toggled, [this, applianceId, stateLabel](bool checked) {
        stateLabel->setText(checked ? "On" : "Off");
        onApplianceToggled(applianceId, checked);
    });
    
    toggleLayout->addWidget(toggle);
    cardLayout->addLayout(toggleLayout);
    
    static_cast<QGridLayout*>(layout)->addWidget(card);
}

void Appliances::onApplianceToggled(int id, bool isOn)
{
    for (int i = 0; i < appliances.size(); ++i) {
        if (appliances[i].id == id) {
            appliances[i].isOn = isOn;
            break;
        }
    }
    updateOverviewCard();
}

void Appliances::updateOverviewCard()
{
    int onCount = 0;
    bool allSafe = true;
    
    for (const Appliance &app : appliances) {
        if (app.isOn) onCount++;
        if (!app.isSafe) allSafe = false;
    }
    
    overviewCountLabel->setText(QString("%1 devices monitored, %2 active").arg(appliances.size()).arg(onCount));
    
    if (allSafe) {
        overviewStatusLabel->setText("All Appliances Safe");
    } else {
        overviewStatusLabel->setText("Attention Required");
    }
}
