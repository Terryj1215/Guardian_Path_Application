#ifndef MONGODBMANAGER_H
#define MONGODBMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <memory>

class MongoDBManager : public QObject
{
    Q_OBJECT

public:
    explicit MongoDBManager(QObject* parent = nullptr);
    ~MongoDBManager();

    bool connect(const QString& uri = "mongodb://localhost:27017",
        const QString& database = "guardian_path");
    bool isConnected() const;
    bool updatePrimaryContact(const QString& contactName);

    // User ID for this device
    void setUserId(const QString& userId);
    QString userId() const;

    // Location operations
    bool saveLocation(double latitude, double longitude, bool isWithinSafeZone);
    QJsonObject getLatestLocation();

    // Safe zone operations
    bool saveSafeZone(double centerLat, double centerLng, double radiusMeters, const QString& name);
    QJsonObject getSafeZone();

    // SOS operations
    QString triggerSOS(double latitude, double longitude);
    bool resolveSOS(const QString& alertId);
    QJsonArray getActiveSOS();

    // Appliance operations
    bool saveAppliance(const QString& name, const QString& type, bool isOn);
    bool toggleAppliance(const QString& name, bool isOn);
    QJsonArray getAppliances();

    // Contact operations
    bool saveContact(const QString& name, const QString& phone,
        const QString& relationship, bool isPrimary);
    bool deleteContact(const QString& contactId);
    bool setPrimaryContact(const QString& contactId);
    QJsonArray getContacts();

signals:
    void connectionStatusChanged(bool connected);
    void locationSaved();
    void sosTriggered(const QString& alertId);
    void sosResolved(const QString& alertId);
    void applianceUpdated(const QString& name, bool isOn);
    void contactsChanged();
    void errorOccurred(const QString& error);

private:
    static mongocxx::instance s_instance;
    std::unique_ptr<mongocxx::pool> m_pool;
    QString m_uri;
    QString m_databaseName;
    QString m_userId;
    bool m_connected;

    mongocxx::database getDatabase();
};

#endif // MONGODBMANAGER_H
