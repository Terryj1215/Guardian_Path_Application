#include "carering.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QMessageBox>

CareRing::CareRing(QWidget *parent)
    : QWidget(parent), nextContactId(4)
{
    // Sample contacts
    contacts.append(Contact(1, "Sarah Johnson", "+1 (555) 123-4567", "Daughter", true));
    contacts.append(Contact(2, "Dr. Michael Chen", "+1 (555) 234-5678", "Doctor", false));
    contacts.append(Contact(3, "Robert Johnson", "+1 (555) 345-6789", "Son", false));
    
    setupUi();
}

void CareRing::setupUi()
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
    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    QVBoxLayout *titleLayout = new QVBoxLayout();
    QLabel *title = new QLabel("Care Ring", contentWidget);
    title->setStyleSheet("color: #f0f0f5; font-size: 32px; font-weight: 700;");
    titleLayout->addWidget(title);
    
    QLabel *subtitle = new QLabel("Manage your trusted emergency contacts", contentWidget);
    subtitle->setStyleSheet("color: #6b7280; font-size: 14px;");
    titleLayout->addWidget(subtitle);
    
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    
    // Add contact button
    QPushButton *addBtn = new QPushButton("+ Add Contact", contentWidget);
    addBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a6cf7;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 12px 24px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #5b7af8;
        }
    )");
    connect(addBtn, &QPushButton::clicked, this, &CareRing::onAddContact);
    headerLayout->addWidget(addBtn);
    
    contentLayout->addLayout(headerLayout);
    
    // Info banner
    QFrame *infoBanner = new QFrame(contentWidget);
    infoBanner->setStyleSheet(R"(
        background-color: rgba(74, 108, 247, 0.1);
        border: 1px solid rgba(74, 108, 247, 0.3);
        border-radius: 8px;
    )");
    
    QHBoxLayout *bannerLayout = new QHBoxLayout(infoBanner);
    bannerLayout->setContentsMargins(16, 12, 16, 12);
    
    QLabel *infoIcon = new QLabel("ℹ️", infoBanner);
    infoIcon->setStyleSheet("font-size: 16px;");
    bannerLayout->addWidget(infoIcon);
    
    QLabel *infoText = new QLabel(
        "Your primary contact will be notified first in emergencies. "
        "All contacts will receive your location when SOS is activated.", infoBanner);
    infoText->setStyleSheet("color: #a0a5b5; font-size: 13px;");
    infoText->setWordWrap(true);
    bannerLayout->addWidget(infoText, 1);
    
    contentLayout->addWidget(infoBanner);
    
    // Contacts list
    QLabel *listTitle = new QLabel("Your Contacts", contentWidget);
    listTitle->setStyleSheet("color: #f0f0f5; font-size: 18px; font-weight: 600;");
    contentLayout->addWidget(listTitle);
    
    contactsLayout = new QVBoxLayout();
    contactsLayout->setSpacing(12);
    refreshContactList();
    
    contentLayout->addLayout(contactsLayout);
    contentLayout->addStretch();
    
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void CareRing::refreshContactList()
{
    // Clear existing widgets
    QLayoutItem *item;
    while ((item = contactsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add contact cards
    for (const Contact &contact : contacts) {
        QWidget *card = createContactCard(contact);
        contactsLayout->addWidget(card);
    }
}

QWidget* CareRing::createContactCard(const Contact &contact)
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
    
    // Avatar
    QLabel *avatar = new QLabel(contact.name.left(1).toUpper(), card);
    avatar->setFixedSize(48, 48);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setStyleSheet(R"(
        background-color: #4a6cf7;
        color: #ffffff;
        border-radius: 24px;
        font-size: 20px;
        font-weight: 600;
    )");
    cardLayout->addWidget(avatar);
    
    // Info
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(4);
    
    QHBoxLayout *nameRow = new QHBoxLayout();
    QLabel *nameLabel = new QLabel(contact.name, card);
    nameLabel->setStyleSheet("color: #f0f0f5; font-size: 16px; font-weight: 600;");
    nameRow->addWidget(nameLabel);
    
    if (contact.isPrimary) {
        QLabel *primaryBadge = new QLabel("Primary", card);
        primaryBadge->setStyleSheet(R"(
            background-color: #22c55e;
            color: #141824;
            border-radius: 4px;
            padding: 2px 8px;
            font-size: 11px;
            font-weight: 600;
        )");
        nameRow->addWidget(primaryBadge);
    }
    nameRow->addStretch();
    infoLayout->addLayout(nameRow);
    
    QLabel *relLabel = new QLabel(contact.relationship, card);
    relLabel->setStyleSheet("color: #6b7280; font-size: 13px;");
    infoLayout->addWidget(relLabel);
    
    QLabel *phoneLabel = new QLabel(contact.phone, card);
    phoneLabel->setStyleSheet("color: #a0a5b5; font-size: 14px;");
    infoLayout->addWidget(phoneLabel);
    
    cardLayout->addLayout(infoLayout, 1);
    
    // Action buttons
    QHBoxLayout *actionsLayout = new QHBoxLayout();
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
    connect(callBtn, &QPushButton::clicked, [this, contact]() {
        onCallContact(contact);
    });
    actionsLayout->addWidget(callBtn);
    
    QPushButton *msgBtn = new QPushButton("Message", card);
    msgBtn->setStyleSheet(R"(
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
    connect(msgBtn, &QPushButton::clicked, [this, contact]() {
        onMessageContact(contact);
    });
    actionsLayout->addWidget(msgBtn);
    
    if (!contact.isPrimary) {
        QPushButton *primaryBtn = new QPushButton("Set Primary", card);
        primaryBtn->setStyleSheet(R"(
            QPushButton {
                background-color: transparent;
                color: #4a6cf7;
                border: 1px solid #4a6cf7;
                border-radius: 8px;
                padding: 10px 16px;
                font-weight: 500;
            }
            QPushButton:hover {
                background-color: rgba(74, 108, 247, 0.1);
            }
        )");
        int contactId = contact.id;
        connect(primaryBtn, &QPushButton::clicked, [this, contactId]() {
            onSetPrimary(contactId);
        });
        actionsLayout->addWidget(primaryBtn);
    }
    
    QPushButton *deleteBtn = new QPushButton("Delete", card);
    deleteBtn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            color: #dc2626;
            border: 1px solid #dc2626;
            border-radius: 8px;
            padding: 10px 16px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: rgba(220, 38, 38, 0.1);
        }
    )");
    int contactId = contact.id;
    connect(deleteBtn, &QPushButton::clicked, [this, contactId]() {
        onDeleteContact(contactId);
    });
    actionsLayout->addWidget(deleteBtn);
    
    cardLayout->addLayout(actionsLayout);
    
    return card;
}

void CareRing::onAddContact()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Contact");
    dialog.setFixedSize(400, 320);
    dialog.setStyleSheet(R"(
        QDialog {
            background-color: #1a1f2e;
        }
        QLabel {
            color: #f0f0f5;
        }
        QLineEdit, QComboBox {
            background-color: #232a3d;
            color: #f0f0f5;
            border: 1px solid #3d4560;
            border-radius: 8px;
            padding: 10px 14px;
        }
        QLineEdit:focus, QComboBox:focus {
            border-color: #4a6cf7;
        }
    )");
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);
    
    QLabel *titleLabel = new QLabel("Add New Contact", &dialog);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: 700;");
    layout->addWidget(titleLabel);
    
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    
    QLineEdit *nameEdit = new QLineEdit(&dialog);
    nameEdit->setPlaceholderText("Enter name");
    formLayout->addRow("Name:", nameEdit);
    
    QLineEdit *phoneEdit = new QLineEdit(&dialog);
    phoneEdit->setPlaceholderText("+1 (555) 000-0000");
    formLayout->addRow("Phone:", phoneEdit);
    
    QComboBox *relCombo = new QComboBox(&dialog);
    relCombo->addItems({"Family", "Friend", "Doctor", "Caregiver", "Neighbor", "Other"});
    formLayout->addRow("Relationship:", relCombo);
    
    layout->addLayout(formLayout);
    layout->addSpacing(16);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    QPushButton *cancelBtn = new QPushButton("Cancel", &dialog);
    cancelBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2d3548;
            color: #f0f0f5;
            border: none;
            border-radius: 8px;
            padding: 12px 24px;
            font-weight: 500;
        }
    )");
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    QPushButton *saveBtn = new QPushButton("Add Contact", &dialog);
    saveBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a6cf7;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 12px 24px;
            font-weight: 600;
        }
    )");
    connect(saveBtn, &QPushButton::clicked, [&]() {
        if (!nameEdit->text().isEmpty() && !phoneEdit->text().isEmpty()) {
            dialog.accept();
        }
    });
    
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(saveBtn);
    layout->addLayout(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        Contact newContact(nextContactId++, nameEdit->text(), phoneEdit->text(), 
                          relCombo->currentText(), false);
        contacts.append(newContact);
        refreshContactList();
    }
}

void CareRing::onDeleteContact(int id)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete Contact",
        "Are you sure you want to remove this contact from your Care Ring?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        for (int i = 0; i < contacts.size(); ++i) {
            if (contacts[i].id == id) {
                contacts.removeAt(i);
                break;
            }
        }
        refreshContactList();
    }
}

void CareRing::onSetPrimary(int id)
{
    for (int i = 0; i < contacts.size(); ++i) {
        contacts[i].isPrimary = (contacts[i].id == id);
    }
    refreshContactList();
}

void CareRing::onCallContact(const Contact &contact)
{
    QMessageBox::information(this, "Calling", 
        QString("Initiating call to %1\n\nPhone: %2\n\n"
                "In a real application, this would dial the number.")
        .arg(contact.name).arg(contact.phone));
}

void CareRing::onMessageContact(const Contact &contact)
{
    QMessageBox::information(this, "Message", 
        QString("Opening message composer for %1\n\n"
                "In a real application, this would open SMS/messaging.")
        .arg(contact.name));
}
