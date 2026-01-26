#include "statuscard.h"

StatusCard::StatusCard(const QString &title, const QString &value, 
                       Status status, QWidget *parent)
    : QFrame(parent), currentStatus(status)
{
    setupUi();
    setTitle(title);
    setValue(value);
    setStatus(status);
}

void StatusCard::setupUi()
{
    setProperty("class", "card");
    setStyleSheet(R"(
        QFrame {
            background-color: #232a3d;
            border: 1px solid #2d3548;
            border-radius: 12px;
            padding: 20px;
        }
    )");
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(12);
    
    // Title row with status
    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("color: #a0a5b5; font-size: 13px; font-weight: 500;");
    headerLayout->addWidget(titleLabel);
    
    statusLabel = new QLabel(this);
    statusLabel->setFixedHeight(24);
    statusLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(statusLabel);
    headerLayout->addStretch();
    
    layout->addLayout(headerLayout);
    
    // Value
    valueLabel = new QLabel(this);
    valueLabel->setStyleSheet("color: #f0f0f5; font-size: 28px; font-weight: 700;");
    layout->addWidget(valueLabel);
    
    // Description
    descriptionLabel = new QLabel(this);
    descriptionLabel->setStyleSheet("color: #6b7280; font-size: 13px;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->hide();
    layout->addWidget(descriptionLabel);
}

void StatusCard::setTitle(const QString &title)
{
    titleLabel->setText(title);
}

void StatusCard::setValue(const QString &value)
{
    valueLabel->setText(value);
}

void StatusCard::setStatus(Status status)
{
    currentStatus = status;
    updateStatusStyle();
}

void StatusCard::setDescription(const QString &description)
{
    descriptionLabel->setText(description);
    descriptionLabel->setVisible(!description.isEmpty());
}

void StatusCard::updateStatusStyle()
{
    QString text;
    QString style;
    
    switch (currentStatus) {
        case Safe:
            text = "Safe";
            style = "background-color: #22c55e; color: #141824; border-radius: 4px; padding: 4px 12px; font-weight: 600; font-size: 12px;";
            break;
        case Warning:
            text = "Warning";
            style = "background-color: #eab308; color: #141824; border-radius: 4px; padding: 4px 12px; font-weight: 600; font-size: 12px;";
            break;
        case Danger:
            text = "Alert";
            style = "background-color: #dc2626; color: #ffffff; border-radius: 4px; padding: 4px 12px; font-weight: 600; font-size: 12px;";
            break;
    }
    
    statusLabel->setText(text);
    statusLabel->setStyleSheet(style);
}
