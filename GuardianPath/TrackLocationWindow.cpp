#include "tracklocationwindow.h"
#include <QVBoxLayout>
#include <QLabel>

TrackLocationWindow::TrackLocationWindow(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // Add a label to indicate the current screen
    QLabel* label = new QLabel("Track Location Screen", this);
    QFont font = label->font();
    font.setPointSize(18);
    label->setFont(font);
    layout->addWidget(label);

    // Add a "Back to Main" button
    QPushButton* backBtn = new QPushButton("Back to Main", this);
    layout->addWidget(backBtn);

    // Connect the back button's clicked signal to emit our custom signal
    connect(backBtn, &QPushButton::clicked, this, &TrackLocationWindow::backToMainRequested);
}