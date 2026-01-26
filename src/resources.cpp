#include "resources.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>

Resources::Resources(QWidget *parent)
    : QWidget(parent), currentFilter("all")
{
    // Sample safety locations
    allLocations.append(SafetyLocation(1, "City General Hospital", "hospital", 
        "123 Medical Center Dr", "+1 (555) 100-1000", 40.7150, -74.0080, 0.8));
    allLocations.append(SafetyLocation(2, "Central Police Station", "police",
        "456 Justice Ave", "+1 (555) 100-2000", 40.7110, -74.0050, 1.2));
    allLocations.append(SafetyLocation(3, "Fire Station #7", "fire",
        "789 Emergency Blvd", "+1 (555) 100-3000", 40.7140, -74.0100, 0.5));
    allLocations.append(SafetyLocation(4, "Community Shelter", "shelter",
        "321 Safe Haven Rd", "+1 (555) 100-4000", 40.7100, -74.0040, 1.5));
    allLocations.append(SafetyLocation(5, "24hr Pharmacy Plus", "pharmacy",
        "555 Health St", "+1 (555) 100-5000", 40.7135, -74.0070, 0.3));
    allLocations.append(SafetyLocation(6, "Urgent Care Clinic", "hospital",
        "777 Quick Care Ln", "+1 (555) 100-6000", 40.7160, -74.0090, 1.0));
    
    setupUi();
}

void Resources::setupUi()
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
    QLabel *title = new QLabel("Safety Resources", contentWidget);
    title->setStyleSheet("color: #f0f0f5; font-size: 32px; font-weight: 700;");
    contentLayout->addWidget(title);
    
    QLabel *subtitle = new QLabel("Find nearby emergency services and safety locations", contentWidget);
    subtitle->setStyleSheet("color: #6b7280; font-size: 14px; margin-bottom: 8px;");
    contentLayout->addWidget(subtitle);
    
    // Search bar
    searchEdit = new QLineEdit(contentWidget);
    searchEdit->setPlaceholderText("Search locations...");
    searchEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: #232a3d;
            color: #f0f0f5;
            border: 1px solid #3d4560;
            border-radius: 8px;
            padding: 12px 16px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border-color: #4a6cf7;
        }
    )");
    connect(searchEdit, &QLineEdit::textChanged, this, &Resources::onSearchChanged);
    contentLayout->addWidget(searchEdit);
    
    // Filter buttons
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(8);
    
    QStringList filters = {"all", "hospital", "police", "fire", "shelter", "pharmacy"};
    QStringList filterLabels = {"All", "Hospitals", "Police", "Fire", "Shelters", "Pharmacies"};
    
    for (int i = 0; i < filters.size(); ++i) {
        QPushButton *filterBtn = new QPushButton(filterLabels[i], contentWidget);
        filterBtn->setProperty("filter", filters[i]);
        filterBtn->setCursor(Qt::PointingHandCursor);
        
        QString baseStyle = R"(
            QPushButton {
                border-radius: 16px;
                padding: 8px 16px;
                font-weight: 500;
                font-size: 13px;
            }
        )";
        
        if (filters[i] == currentFilter) {
            filterBtn->setStyleSheet(baseStyle + R"(
                QPushButton {
                    background-color: #4a6cf7;
                    color: #ffffff;
                    border: none;
                }
            )");
        } else {
            filterBtn->setStyleSheet(baseStyle + R"(
                QPushButton {
                    background-color: #2d3548;
                    color: #a0a5b5;
                    border: 1px solid #3d4560;
                }
                QPushButton:hover {
                    background-color: #3d4560;
                    color: #f0f0f5;
                }
            )");
        }
        
        connect(filterBtn, &QPushButton::clicked, [this, filters, i]() {
            onFilterChanged(filters[i]);
        });
        
        filterLayout->addWidget(filterBtn);
    }
    filterLayout->addStretch();
    contentLayout->addLayout(filterLayout);
    
    // Locations list
    locationsLayout = new QVBoxLayout();
    locationsLayout->setSpacing(12);
    refreshLocationList();
    
    contentLayout->addLayout(locationsLayout);
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void Resources::onSearchChanged(const QString &text)
{
    currentSearch = text.toLower();
    refreshLocationList();
}

void Resources::onFilterChanged(const QString &filter)
{
    currentFilter = filter;
    
    // Update button styles - would need to store buttons or recreate them
    // For simplicity, just refresh the list
    refreshLocationList();
}

void Resources::refreshLocationList()
{
    // Clear existing widgets
    QLayoutItem *item;
    while ((item = locationsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add filtered location cards
    for (const SafetyLocation &location : allLocations) {
        // Apply filters
        if (currentFilter != "all" && location.type != currentFilter) {
            continue;
        }
        
        if (!currentSearch.isEmpty()) {
            if (!location.name.toLower().contains(currentSearch) &&
                !location.address.toLower().contains(currentSearch) &&
                !location.type.toLower().contains(currentSearch)) {
                continue;
            }
        }
        
        QWidget *card = createLocationCard(location);
        locationsLayout->addWidget(card);
    }
    
    // Show "no results" if empty
    if (locationsLayout->count() == 0) {
        QLabel *noResults = new QLabel("No locations found matching your criteria.", contentWidget);
        noResults->setStyleSheet("color: #6b7280; font-size: 14px; padding: 20px;");
        noResults->setAlignment(Qt::AlignCenter);
        locationsLayout->addWidget(noResults);
    }
}

QWidget* Resources::createLocationCard(const SafetyLocation &location)
{
    QFrame *card = new QFrame(contentWidget);
    card->setStyleSheet(R"(
        QFrame {
            background-color: #232a3d;
            border: 1px solid #2d3548;
            border-radius: 12px;
        }
    )");
    
    QHBoxLayout *cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(16);
    
    // Icon
    QLabel *iconLabel = new QLabel(getIconForType(location.type), card);
    iconLabel->setStyleSheet("font-size: 32px;");
    iconLabel->setFixedSize(48, 48);
    iconLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(iconLabel);
    
    // Info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(4);
    
    QHBoxLayout *nameRow = new QHBoxLayout();
    QLabel *nameLabel = new QLabel(location.name, card);
    nameLabel->setStyleSheet("color: #f0f0f5; font-size: 16px; font-weight: 600;");
    nameRow->addWidget(nameLabel);
    
    // Type badge
    QString badgeColor;
    if (location.type == "hospital") badgeColor = "#ef4444";
    else if (location.type == "police") badgeColor = "#3b82f6";
    else if (location.type == "fire") badgeColor = "#f97316";
    else if (location.type == "shelter") badgeColor = "#22c55e";
    else if (location.type == "pharmacy") badgeColor = "#8b5cf6";
    else badgeColor = "#6b7280";
    
    QLabel *typeBadge = new QLabel(location.type.at(0).toUpper() + location.type.mid(1), card);
    typeBadge->setStyleSheet(QString(R"(
        background-color: %1;
        color: #ffffff;
        border-radius: 4px;
        padding: 2px 8px;
        font-size: 11px;
        font-weight: 600;
    )").arg(badgeColor));
    nameRow->addWidget(typeBadge);
    nameRow->addStretch();
    infoLayout->addLayout(nameRow);
    
    QLabel *addressLabel = new QLabel(location.address, card);
    addressLabel->setStyleSheet("color: #a0a5b5; font-size: 14px;");
    infoLayout->addWidget(addressLabel);
    
    QLabel *phoneLabel = new QLabel(location.phone, card);
    phoneLabel->setStyleSheet("color: #6b7280; font-size: 13px;");
    infoLayout->addWidget(phoneLabel);
    
    cardLayout->addLayout(infoLayout, 1);
    
    // Distance
    QVBoxLayout *distanceLayout = new QVBoxLayout();
    distanceLayout->setAlignment(Qt::AlignRight);
    
    QLabel *distLabel = new QLabel(QString("%1 km").arg(location.distanceKm, 0, 'f', 1), card);
    distLabel->setStyleSheet("color: #4a6cf7; font-size: 18px; font-weight: 700;");
    distLabel->setAlignment(Qt::AlignRight);
    distanceLayout->addWidget(distLabel);
    
    QLabel *awayLabel = new QLabel("away", card);
    awayLabel->setStyleSheet("color: #6b7280; font-size: 12px;");
    awayLabel->setAlignment(Qt::AlignRight);
    distanceLayout->addWidget(awayLabel);
    
    cardLayout->addLayout(distanceLayout);
    
    // Action buttons
    QVBoxLayout *actionsLayout = new QVBoxLayout();
    actionsLayout->setSpacing(8);
    
    QPushButton *callBtn = new QPushButton("Call", card);
    callBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #22c55e;
            color: #141824;
            border: none;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #16a34a;
        }
    )");
    connect(callBtn, &QPushButton::clicked, [this, location]() {
        onCallLocation(location);
    });
    actionsLayout->addWidget(callBtn);
    
    QPushButton *dirBtn = new QPushButton("Directions", card);
    dirBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2d3548;
            color: #f0f0f5;
            border: 1px solid #3d4560;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #3d4560;
        }
    )");
    connect(dirBtn, &QPushButton::clicked, [this, location]() {
        onGetDirections(location);
    });
    actionsLayout->addWidget(dirBtn);
    
    cardLayout->addLayout(actionsLayout);
    
    return card;
}

QString Resources::getIconForType(const QString &type)
{
    if (type == "hospital") return "🏥";
    if (type == "police") return "🚔";
    if (type == "fire") return "🚒";
    if (type == "shelter") return "🏠";
    if (type == "pharmacy") return "💊";
    return "📍";
}

void Resources::onCallLocation(const SafetyLocation &location)
{
    QMessageBox::information(this, "Calling",
        QString("Initiating call to %1\n\nPhone: %2\n\n"
                "In a real application, this would dial the number.")
        .arg(location.name).arg(location.phone));
}

void Resources::onGetDirections(const SafetyLocation &location)
{
    QMessageBox::information(this, "Directions",
        QString("Getting directions to %1\n\nAddress: %2\n"
                "Coordinates: %3, %4\n\n"
                "In a real application, this would open a maps application.")
        .arg(location.name).arg(location.address)
        .arg(location.latitude, 0, 'f', 4).arg(location.longitude, 0, 'f', 4));
}
