#ifndef SOSBUTTON_H
#define SOSBUTTON_H

#include <QPushButton>
#include <QTimer>
#include <QMessageBox>

class SOSButton : public QPushButton
{
    Q_OBJECT

public:
    explicit SOSButton(QWidget *parent = nullptr);
    
signals:
    void sosActivated();
    void sosCancelled();

private slots:
    void onClicked();
    void onHoldTimeout();

private:
    void setupUi();
    void showConfirmationDialog();
    
    QTimer *holdTimer;
    bool isHolding;
};

#endif // SOSBUTTON_H
