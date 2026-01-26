#include "sosbutton.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>

SOSButton::SOSButton(QWidget *parent)
    : QPushButton(parent), isHolding(false)
{
    setupUi();
    
    holdTimer = new QTimer(this);
    holdTimer->setSingleShot(true);
    connect(holdTimer, &QTimer::timeout, this, &SOSButton::onHoldTimeout);
    
    connect(this, &QPushButton::clicked, this, &SOSButton::onClicked);
}

void SOSButton::setupUi()
{
    setText("SOS");
    setFixedSize(120, 120);
    setCursor(Qt::PointingHandCursor);
    
    setStyleSheet(R"(
        QPushButton {
            background-color: #dc2626;
            color: #ffffff;
            border: 4px solid #991b1b;
            border-radius: 60px;
            font-size: 24px;
            font-weight: 800;
        }
        QPushButton:hover {
            background-color: #ef4444;
            border-color: #dc2626;
        }
        QPushButton:pressed {
            background-color: #b91c1c;
            border-color: #7f1d1d;
        }
    )");
}

void SOSButton::onClicked()
{
    showConfirmationDialog();
}

void SOSButton::onHoldTimeout()
{
    isHolding = false;
    emit sosActivated();
}

void SOSButton::showConfirmationDialog()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Emergency SOS");
    dialog.setFixedSize(400, 250);
    dialog.setStyleSheet(R"(
        QDialog {
            background-color: #1a1f2e;
        }
        QLabel {
            color: #f0f0f5;
        }
        QPushButton {
            padding: 12px 24px;
            border-radius: 8px;
            font-weight: 600;
            font-size: 14px;
        }
    )");
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);
    
    QLabel *icon = new QLabel("⚠️", &dialog);
    icon->setStyleSheet("font-size: 48px;");
    icon->setAlignment(Qt::AlignCenter);
    layout->addWidget(icon);
    
    QLabel *title = new QLabel("Activate Emergency SOS?", &dialog);
    title->setStyleSheet("font-size: 20px; font-weight: 700; color: #f0f0f5;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);
    
    QLabel *description = new QLabel(
        "This will alert your emergency contacts and share your location. "
        "Only use in genuine emergencies.", &dialog);
    description->setStyleSheet("color: #a0a5b5; font-size: 14px;");
    description->setWordWrap(true);
    description->setAlignment(Qt::AlignCenter);
    layout->addWidget(description);
    
    layout->addSpacing(8);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    
    QPushButton *cancelBtn = new QPushButton("Cancel", &dialog);
    cancelBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2d3548;
            color: #f0f0f5;
            border: 1px solid #3d4560;
        }
        QPushButton:hover {
            background-color: #3d4560;
        }
    )");
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    QPushButton *confirmBtn = new QPushButton("Activate SOS", &dialog);
    confirmBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #dc2626;
            color: #ffffff;
            border: none;
        }
        QPushButton:hover {
            background-color: #ef4444;
        }
    )");
    connect(confirmBtn, &QPushButton::clicked, [this, &dialog]() {
        dialog.accept();
        emit sosActivated();
    });
    
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(confirmBtn);
    layout->addLayout(buttonLayout);
    
    dialog.exec();
}
