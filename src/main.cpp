#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "mongodbmanager.h"
#include "gpsmanager.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Set application metadata
    QApplication::setApplicationName("Guardian Path");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("GuardianPath");

    // 1. Initialize MongoDB connection
    // We create this on the heap so it persists throughout the application life
    MongoDBManager* mongoManager = new MongoDBManager(&app);

    // Attempt to connect to the local MongoDB instance
    // Note: 'guardian_path' is the database that will store your 'contacts' and 'locations'
    bool dbConnected = mongoManager->connect("mongodb://localhost:27017", "guardian_path");

    if (!dbConnected) {
        // Outcome 5 (Security/Reliability): Provide clear user feedback for system failure
        QMessageBox::warning(nullptr, "Database Connection",
            "Could not connect to MongoDB.\n"
            "Please ensure MongoDB is running on localhost:27017.\n\n"
            "The Care Ring will run in 'Session Only' mode (data will not be saved).");
    }

    // Identity Management: In a real app, this would be tied to a login system
    mongoManager->setUserId("demo-user");

    // 2. Initialize GPS manager
    GPSManager* gpsManager = new GPSManager(&app);

    // 3. Connect GPS updates to MongoDB 
    // This bridges the logic between real-time tracking and persistent storage
    QObject::connect(gpsManager, &GPSManager::positionUpdated,
        [mongoManager, gpsManager](double lat, double lng) {
            mongoManager->saveLocation(lat, lng, gpsManager->isWithinSafeZone());
        });

    // 4. Global Stylesheet - Maintains UI consistency across all modular components
    app.setStyleSheet(R"(
        /* Guardian Path - Security-First Dark Theme */
        QWidget {
            background-color: #1a1f2e;
            color: #f0f0f5;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 14px;
        }
        
        QMainWindow {
            background-color: #141824;
        }
        
        /* Nav and Buttons */
        QPushButton[class="nav-item"] {
            background-color: transparent;
            color: #a0a5b5;
            border: none;
            border-radius: 8px;
            padding: 12px 16px;
            text-align: left;
        }
        
        QPushButton[class="nav-item"]:hover {
            background-color: #232a3d;
            color: #f0f0f5;
        }
        
        QPushButton[class="nav-item"][active="true"] {
            background-color: #4a6cf7;
            color: #ffffff;
        }

        /* Input Fields */
        QLineEdit, QComboBox {
            background-color: #232a3d;
            color: #f0f0f5;
            border: 1px solid #3d4560;
            border-radius: 8px;
            padding: 10px 14px;
        }
        
        /* Status Badges */
        QLabel[class="safe"] {
            background-color: #22c55e;
            color: #141824;
            border-radius: 4px;
            padding: 4px 8px;
            font-weight: 600;
        }
    )");

    // 5. THE INJECTION: Hand the manager to the UI
    // This allows MainWindow to pass it down to CareRing and TrackLocation
    MainWindow window(mongoManager, gpsManager);
    window.show();

    // 6. Start GPS tracking (update every 5 seconds)
    gpsManager->startTracking(5000);

    return app.exec();
}