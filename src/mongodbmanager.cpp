#include "mongodbmanager.h"
#include <QDateTime>
#include <QDebug>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <mongocxx/exception/exception.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

// Initialize MongoDB driver instance (must be done once)
mongocxx::instance MongoDBManager::s_instance{};

MongoDBManager::MongoDBManager(QObject* parent)
    : QObject(parent)
    , m_connected(false)
    , m_userId("default-user")
{
}

MongoDBManager::~MongoDBManager()
{
}

bool MongoDBManager::connect(const QString& uri, const QString& database)
{
    try {
        m_uri = uri;
        m_databaseName = database;

        mongocxx::uri mongoUri(uri.toStdString());
        m_pool = std::make_unique<mongocxx::pool>(mongoUri);

        // Test connection
        auto client = m_pool->acquire();
        auto db = (*client)[database.toStdString()];
        db.run_command(make_document(kvp("ping", 1)));

        m_connected = true;
        emit connectionStatusChanged(true);
        qDebug() << "Connected to MongoDB:" << uri;
        return true;

    }
    catch (const mongocxx::exception& e) {
        m_connected = false;
        emit connectionStatusChanged(false);
        emit errorOccurred(QString("MongoDB connection failed: %1").arg(e.what()));
        qWarning() << "MongoDB connection failed:" << e.what();
        return false;
    }
}

bool MongoDBManager::isConnected() const
{
    return m_connected;
}

void MongoDBManager::setUserId(const QString& userId)
{
    m_userId = userId;
}

QString MongoDBManager::userId() const
{
    return m_userId;
}

mongocxx::database MongoDBManager::getDatabase()
{
    auto client = m_pool->acquire();
    return (*client)[m_databaseName.toStdString()];
}

bool MongoDBManager::saveLocation(double latitude, double longitude, bool isWithinSafeZone)
{
    if (!m_connected) return false;

    try {
        auto db = getDatabase();
        auto collection = db["locations"];

        auto doc = make_document(
            kvp("userId", m_userId.toStdString()),
            kvp("latitude", latitude),
            kvp("longitude", longitude),
            kvp("isWithinSafeZone", isWithinSafeZone),
            kvp("timestamp", bsoncxx::types::b_date{ std::chrono::system_clock::now() })
        );

        collection.insert_one(doc.view());
        emit locationSaved();
        return true;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to save location: %1").arg(e.what()));
        return false;
    }
}

QJsonObject MongoDBManager::getLatestLocation()
{
    QJsonObject result;
    if (!m_connected) return result;

    try {
        auto db = getDatabase();
        auto collection = db["locations"];

        mongocxx::options::find opts;
        opts.sort(make_document(kvp("timestamp", -1)));
        opts.limit(1);

        auto cursor = collection.find(
            make_document(kvp("userId", m_userId.toStdString())),
            opts
        );

        for (auto&& doc : cursor) {
            result["latitude"] = doc["latitude"].get_double().value;
            result["longitude"] = doc["longitude"].get_double().value;
            result["isWithinSafeZone"] = doc["isWithinSafeZone"].get_bool().value;
            break;
        }

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to get location: %1").arg(e.what()));
    }

    return result;
}

bool MongoDBManager::saveSafeZone(double centerLat, double centerLng, double radiusMeters, const QString& name)
{
    if (!m_connected) return false;

    try {
        auto db = getDatabase();
        auto collection = db["safe_zones"];

        auto filter = make_document(kvp("userId", m_userId.toStdString()));
        auto update = make_document(
            kvp("$set", make_document(
                kvp("userId", m_userId.toStdString()),
                kvp("centerLat", centerLat),
                kvp("centerLng", centerLng),
                kvp("radiusMeters", radiusMeters),
                kvp("name", name.toStdString())
            ))
        );

        mongocxx::options::update opts;
        opts.upsert(true);

        collection.update_one(filter.view(), update.view(), opts);
        return true;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to save safe zone: %1").arg(e.what()));
        return false;
    }
}

QJsonObject MongoDBManager::getSafeZone()
{
    QJsonObject result;
    if (!m_connected) return result;

    try {
        auto db = getDatabase();
        auto collection = db["safe_zones"];

        auto doc = collection.find_one(
            make_document(kvp("userId", m_userId.toStdString()))
        );

        if (doc) {
            result["centerLat"] = doc->view()["centerLat"].get_double().value;
            result["centerLng"] = doc->view()["centerLng"].get_double().value;
            result["radiusMeters"] = doc->view()["radiusMeters"].get_double().value;
            result["name"] = QString::fromStdString(std::string(doc->view()["name"].get_string().value));
        }

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to get safe zone: %1").arg(e.what()));
    }

    return result;
}

QString MongoDBManager::triggerSOS(double latitude, double longitude)
{
    if (!m_connected) return QString();

    try {
        auto db = getDatabase();
        auto collection = db["sos_alerts"];

        auto doc = make_document(
            kvp("userId", m_userId.toStdString()),
            kvp("latitude", latitude),
            kvp("longitude", longitude),
            kvp("timestamp", bsoncxx::types::b_date{ std::chrono::system_clock::now() }),
            kvp("isActive", true)
        );

        auto result = collection.insert_one(doc.view());
        QString alertId = QString::fromStdString(result->inserted_id().get_oid().value.to_string());

        emit sosTriggered(alertId);
        return alertId;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to trigger SOS: %1").arg(e.what()));
        return QString();
    }
}

bool MongoDBManager::resolveSOS(const QString& alertId)
{
    if (!m_connected) return false;

    try {
        auto db = getDatabase();
        auto collection = db["sos_alerts"];

        bsoncxx::oid oid(alertId.toStdString());
        auto filter = make_document(kvp("_id", oid));
        auto update = make_document(
            kvp("$set", make_document(
                kvp("isActive", false),
                kvp("resolvedAt", bsoncxx::types::b_date{ std::chrono::system_clock::now() })
            ))
        );

        collection.update_one(filter.view(), update.view());
        emit sosResolved(alertId);
        return true;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to resolve SOS: %1").arg(e.what()));
        return false;
    }
}

QJsonArray MongoDBManager::getActiveSOS()
{
    QJsonArray result;
    if (!m_connected) return result;

    try {
        auto db = getDatabase();
        auto collection = db["sos_alerts"];

        auto cursor = collection.find(
            make_document(
                kvp("userId", m_userId.toStdString()),
                kvp("isActive", true)
            )
        );

        for (auto&& doc : cursor) {
            QJsonObject alert;
            alert["id"] = QString::fromStdString(doc["_id"].get_oid().value.to_string());
            alert["latitude"] = doc["latitude"].get_double().value;
            alert["longitude"] = doc["longitude"].get_double().value;
            result.append(alert);
        }

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to get SOS alerts: %1").arg(e.what()));
    }

    return result;
}

bool MongoDBManager::saveAppliance(const QString& name, const QString& type, bool isOn)
{
    if (!m_connected) return false;

    try {
        auto db = getDatabase();
        auto collection = db["appliances"];

        auto filter = make_document(
            kvp("userId", m_userId.toStdString()),
            kvp("name", name.toStdString())
        );

        auto update = make_document(
            kvp("$set", make_document(
                kvp("userId", m_userId.toStdString()),
                kvp("name", name.toStdString()),
                kvp("type", type.toStdString()),
                kvp("isOn", isOn),
                kvp("lastUpdated", bsoncxx::types::b_date{ std::chrono::system_clock::now() })
            ))
        );

        mongocxx::options::update opts;
        opts.upsert(true);

        collection.update_one(filter.view(), update.view(), opts);
        emit applianceUpdated(name, isOn);
        return true;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to save appliance: %1").arg(e.what()));
        return false;
    }
}

bool MongoDBManager::toggleAppliance(const QString& name, bool isOn)
{
    return saveAppliance(name, "", isOn);
}

QJsonArray MongoDBManager::getAppliances()
{
    QJsonArray result;
    if (!m_connected) return result;

    try {
        auto db = getDatabase();
        auto collection = db["appliances"];

        auto cursor = collection.find(
            make_document(kvp("userId", m_userId.toStdString()))
        );

        for (auto&& doc : cursor) {
            QJsonObject appliance;
            appliance["name"] = QString::fromStdString(std::string(doc["name"].get_string().value));
            appliance["type"] = QString::fromStdString(std::string(doc["type"].get_string().value));
            appliance["isOn"] = doc["isOn"].get_bool().value;
            result.append(appliance);
        }

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to get appliances: %1").arg(e.what()));
    }

    return result;
}

bool MongoDBManager::saveContact(const QString& name, const QString& phone,
    const QString& relationship, bool isPrimary)
{
    if (!m_connected) return false;

    try {
        auto db = getDatabase();
        auto collection = db["contacts"];

        // If setting as primary, unset others first
        if (isPrimary) {
            collection.update_many(
                make_document(kvp("userId", m_userId.toStdString())),
                make_document(kvp("$set", make_document(kvp("isPrimary", false))))
            );
        }

        auto doc = make_document(
            kvp("userId", m_userId.toStdString()),
            kvp("name", name.toStdString()),
            kvp("phone", phone.toStdString()),
            kvp("relationship", relationship.toStdString()),
            kvp("isPrimary", isPrimary)
        );

        collection.insert_one(doc.view());
        emit contactsChanged();
        return true;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to save contact: %1").arg(e.what()));
        return false;
    }
}

bool MongoDBManager::deleteContact(const QString& contactId)
{
    if (!m_connected) return false;

    try {
        auto db = getDatabase();
        auto collection = db["contacts"];

        bsoncxx::oid oid(contactId.toStdString());
        collection.delete_one(make_document(kvp("_id", oid)));

        emit contactsChanged();
        return true;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to delete contact: %1").arg(e.what()));
        return false;
    }
}

bool MongoDBManager::setPrimaryContact(const QString& contactId)
{
    if (!m_connected) return false;

    try {
        auto db = getDatabase();
        auto collection = db["contacts"];

        // Unset all primaries first
        collection.update_many(
            make_document(kvp("userId", m_userId.toStdString())),
            make_document(kvp("$set", make_document(kvp("isPrimary", false))))
        );

        // Set new primary
        bsoncxx::oid oid(contactId.toStdString());
        collection.update_one(
            make_document(kvp("_id", oid)),
            make_document(kvp("$set", make_document(kvp("isPrimary", true))))
        );

        emit contactsChanged();
        return true;

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to set primary contact: %1").arg(e.what()));
        return false;
    }
}

QJsonArray MongoDBManager::getContacts()
{
    QJsonArray result;
    if (!m_connected) return result;

    try {
        auto db = getDatabase();
        auto collection = db["contacts"];

        mongocxx::options::find opts;
        opts.sort(make_document(kvp("isPrimary", -1), kvp("name", 1)));

        auto cursor = collection.find(
            make_document(kvp("userId", m_userId.toStdString())),
            opts
        );

        for (auto&& doc : cursor) {
            QJsonObject contact;
            contact["id"] = QString::fromStdString(doc["_id"].get_oid().value.to_string());
            contact["name"] = QString::fromStdString(std::string(doc["name"].get_string().value));
            contact["phone"] = QString::fromStdString(std::string(doc["phone"].get_string().value));
            contact["relationship"] = QString::fromStdString(std::string(doc["relationship"].get_string().value));
            contact["isPrimary"] = doc["isPrimary"].get_bool().value;
            result.append(contact);
        }

    }
    catch (const mongocxx::exception& e) {
        emit errorOccurred(QString("Failed to get contacts: %1").arg(e.what()));
    }

    return result;
}
