#include "careringwindow.h"
#include <QVBoxLayout>
#include <QLabel>

CareRingWindow::CareRingWindow(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* label = new QLabel("Care Ring Screen", this);
    QFont font = label->font();
    font.setPointSize(18);
    label->setFont(font);
    layout->addWidget(label);

    QPushButton* backBtn = new QPushButton("Back to Main", this);
    layout->addWidget(backBtn);

    connect(backBtn, &QPushButton::clicked, this, &CareRingWindow::backToMainRequested);
}
