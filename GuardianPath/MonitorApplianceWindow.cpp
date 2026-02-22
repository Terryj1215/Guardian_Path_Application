#include "monitorappliancewindow.h"
#include <QVBoxLayout>
#include <QLabel>

MonitorApplianceWindow::MonitorApplianceWindow(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* label = new QLabel("Monitor Appliance Screen", this);
    QFont font = label->font();
    font.setPointSize(18);
    label->setFont(font);
    layout->addWidget(label);

    QPushButton* backBtn = new QPushButton("Back to Main", this);
    layout->addWidget(backBtn);

    connect(backBtn, &QPushButton::clicked, this, &MonitorApplianceWindow::backToMainRequested);
}