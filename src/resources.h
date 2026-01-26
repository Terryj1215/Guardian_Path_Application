#ifndef RESOURCES_H
#define RESOURCES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QList>
#include "location.h"

class Resources : public QWidget
{
    Q_OBJECT

public:
    explicit Resources(QWidget *parent = nullptr);

private slots:
    void onSearchChanged(const QString &text);
    void onFilterChanged(const QString &filter);
    void onCallLocation(const SafetyLocation &location);
    void onGetDirections(const SafetyLocation &location);

private:
    void setupUi();
    void refreshLocationList();
    QWidget* createLocationCard(const SafetyLocation &location);
    QString getIconForType(const QString &type);
    
    QScrollArea *scrollArea;
    QWidget *contentWidget;
    QVBoxLayout *locationsLayout;
    QLineEdit *searchEdit;
    
    QList<SafetyLocation> allLocations;
    QString currentFilter;
    QString currentSearch;
};

#endif // RESOURCES_H
