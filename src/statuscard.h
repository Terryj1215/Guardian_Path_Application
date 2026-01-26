#ifndef STATUSCARD_H
#define STATUSCARD_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class StatusCard : public QFrame
{
    Q_OBJECT

public:
    enum Status { Safe, Warning, Danger };
    
    explicit StatusCard(const QString &title, const QString &value, 
                        Status status = Safe, QWidget *parent = nullptr);
    
    void setTitle(const QString &title);
    void setValue(const QString &value);
    void setStatus(Status status);
    void setDescription(const QString &description);

private:
    void setupUi();
    void updateStatusStyle();
    
    QLabel *titleLabel;
    QLabel *valueLabel;
    QLabel *statusLabel;
    QLabel *descriptionLabel;
    Status currentStatus;
};

#endif // STATUSCARD_H
