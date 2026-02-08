#include "carering.h"
#include "mongodbmanager.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>

CareRing::CareRing(MongoDBManager* mongo, QWidget* parent)
    : QWidget(parent), m_mongoManager(mongo), nextContactId(1)
{
    setupUi();

    if (m_mongoManager && m_mongoManager->isConnected()) {
        loadContactsFromDB();
    }
    else {
        contacts.append(Contact("offline_0", "Offline Mode", "No DB Connection", "System", false));
        refreshContactList();
    }
}

void CareRing::loadContactsFromDB()
{
    if (!m_mongoManager) return;

    contacts.clear();
    QJsonArray dbContacts = m_mongoManager->getContacts();

    for (const QJsonValue& value : dbContacts) {
        QJsonObject obj = value.toObject();
        Contact c;
        c.id = obj["id"].toString();
        c.name = obj["name"].toString();
        c.phone = obj["phone"].toString();
        c.relationship = obj["relationship"].toString();
        c.isPrimary = obj["isPrimary"].toBool();
        contacts.append(c);
    }

    refreshContactList();
}

void CareRing::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(24);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* title = new QLabel("Care Ring", contentWidget);
    title->setStyleSheet("color: #f0f0f5; font-size: 32px; font-weight: 700;");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    QPushButton* addBtn = new QPushButton("+ Add Contact", contentWidget);
    addBtn->setCursor(Qt::PointingHandCursor);
    addBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #4a6cf7; color: white; border-radius: 8px; 
            padding: 12px 24px; font-weight: 600;
        }
        QPushButton:hover { background-color: #5b7af8; }
    )");
    connect(addBtn, &QPushButton::clicked, this, &CareRing::onAddContact);
    headerLayout->addWidget(addBtn);

    contentLayout->addLayout(headerLayout);

    contactsLayout = new QVBoxLayout();
    contactsLayout->setSpacing(12);
    contentLayout->addLayout(contactsLayout);
    contentLayout->addStretch();

    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);
}

void CareRing::refreshContactList()
{
    QLayoutItem* item;
    while ((item = contactsLayout->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    for (int i = 0; i < contacts.size(); ++i) {
        contactsLayout->addWidget(createContactCard(contacts[i], i));
    }
}

QWidget* CareRing::createContactCard(const Contact& contact, int index)
{
    QFrame* card = new QFrame(contentWidget);
    card->setObjectName("contactCard");
    card->setStyleSheet("QFrame#contactCard { background-color: #232a3d; border: 1px solid #2d3548; border-radius: 12px; }");

    QHBoxLayout* cardLayout = new QHBoxLayout(card);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(20);

    QLabel* avatar = new QLabel(contact.name.left(1).toUpper(), card);
    avatar->setFixedSize(50, 50);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setStyleSheet("background-color: #4a6cf7; color: white; border-radius: 25px; font-weight: 700; font-size: 20px;");
    cardLayout->addWidget(avatar);

    QVBoxLayout* info = new QVBoxLayout();
    QHBoxLayout* nameRow = new QHBoxLayout();
    QLabel* nameLabel = new QLabel(contact.name, card);
    nameLabel->setStyleSheet("color: #f0f0f5; font-size: 18px; font-weight: 600;");
    nameRow->addWidget(nameLabel);

    if (contact.isPrimary) {
        QLabel* badge = new QLabel("PRIMARY", card);
        badge->setStyleSheet("background-color: #22c55e; color: #141824; border-radius: 4px; padding: 2px 8px; font-size: 10px; font-weight: 800;");
        nameRow->addWidget(badge);
    }
    nameRow->addStretch();
    info->addLayout(nameRow);

    QLabel* detailLabel = new QLabel(contact.relationship + " • " + contact.phone, card);
    detailLabel->setStyleSheet("color: #a0a5b5; font-size: 14px;");
    info->addWidget(detailLabel);
    cardLayout->addLayout(info, 1);

    // Actions Column - NOW WITH ALL FOUR BUTTONS
    QHBoxLayout* btns = new QHBoxLayout();
    btns->setSpacing(8);
    auto style = [](QPushButton* b, QString c) {
        b->setFixedSize(85, 30);
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet(QString("QPushButton { color: %1; border: 1px solid %1; border-radius: 6px; font-size: 11px; font-weight: 600; }"
            "QPushButton:hover { background: %1; color: white; }").arg(c));
        };

    // 1. Call
    QPushButton* callBtn = new QPushButton("Call", card);
    style(callBtn, "#22c55e");
    connect(callBtn, &QPushButton::clicked, [this, contact]() { onCallContact(contact); });

    // 2. Message
    QPushButton* msgBtn = new QPushButton("Message", card);
    style(msgBtn, "#4a6cf7");
    connect(msgBtn, &QPushButton::clicked, [this, contact]() { onMessageContact(contact); });

    // 3. Set Primary
    QPushButton* priBtn = new QPushButton("Primary", card);
    style(priBtn, "#eab308"); // Yellow/Gold for primary
    connect(priBtn, &QPushButton::clicked, [this, index]() { onSetPrimary(index); });

    // 4. Delete
    QPushButton* delBtn = new QPushButton("Delete", card);
    style(delBtn, "#ef4444");
    connect(delBtn, &QPushButton::clicked, [this, index]() { onDeleteContact(index); });

    btns->addWidget(callBtn);
    btns->addWidget(msgBtn);
    btns->addWidget(priBtn);
    btns->addWidget(delBtn);
    cardLayout->addLayout(btns);

    return card;
}

void CareRing::onAddContact()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Member");
    dialog.setFixedWidth(350);
    dialog.setStyleSheet("background-color: #1a1f2e; color: white;");

    QFormLayout* form = new QFormLayout(&dialog);
    QLineEdit* nameEdit = new QLineEdit(&dialog);
    QLineEdit* phoneEdit = new QLineEdit(&dialog);
    QComboBox* relCombo = new QComboBox(&dialog);
    relCombo->addItems({ "Daughter", "Son", "Doctor", "Friend", "Uncle", "Aunt", "Caregiver" });

    form->addRow("Name:", nameEdit);
    form->addRow("Phone:", phoneEdit);
    form->addRow("Role:", relCombo);

    QPushButton* save = new QPushButton("Save Contact", &dialog);
    save->setStyleSheet("background-color: #4a6cf7; color: white; padding: 10px; font-weight: bold; border-radius: 4px;");
    form->addRow(save);

    connect(save, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        if (m_mongoManager && m_mongoManager->isConnected()) {
            m_mongoManager->saveContact(nameEdit->text(), phoneEdit->text(), relCombo->currentText(), false);
            loadContactsFromDB();
        }
    }
}

void CareRing::onSetPrimary(int index)
{
    if (index < 0 || index >= contacts.size()) return;
    if (m_mongoManager && m_mongoManager->isConnected()) {
        m_mongoManager->updatePrimaryContact(contacts[index].name);
        loadContactsFromDB(); // Refresh to show the green badge on the new primary
    }
}

void CareRing::onDeleteContact(int index)
{
    if (index < 0 || index >= contacts.size()) return;
    QString contactId = contacts[index].id;

    if (m_mongoManager && m_mongoManager->isConnected()) {
        if (m_mongoManager->deleteContact(contactId)) {
            loadContactsFromDB();
        }
        else {
            QMessageBox::warning(this, "Error", "Could not delete contact.");
        }
    }
    else {
        contacts.removeAt(index);
        refreshContactList();
    }
}

void CareRing::onCallContact(const Contact& c) {
    QMessageBox::information(this, "Call", "Initiating call to " + c.name);
}

void CareRing::onMessageContact(const Contact& c) {
    QMessageBox::information(this, "Message", "Opening chat with " + c.name);
}