Guardian Path - C++ Qt Application
===================================

Prerequisites:
--------------
1. Visual Studio 2019 or later
2. Qt 6.x installed (https://www.qt.io/download)
3. Qt VS Tools extension for Visual Studio
   - Install from Visual Studio Marketplace: "Qt Visual Studio Tools"
4. CMake 3.16 or later

Setup Instructions:
-------------------

Option A: Using CMake with Visual Studio
1. Open Visual Studio
2. Select "Open a local folder" and navigate to the guardian-path-cpp folder
3. Visual Studio will detect the CMakeLists.txt automatically
4. Configure Qt path in CMakeSettings.json if needed:
   - Add: "CMAKE_PREFIX_PATH": "C:/Qt/6.x.x/msvc2019_64"
5. Build > Build All

Option B: Using Qt Creator
1. Open Qt Creator
2. File > Open File or Project
3. Select CMakeLists.txt
4. Configure the project with your Qt kit
5. Build > Build Project

Option C: Manual Qt Project File (.pro)
Create a GuardianPath.pro file with:

    QT += core gui widgets
    CONFIG += c++17
    
    SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/dashboard.cpp \
        src/tracklocation.cpp \
        src/appliances.cpp \
        src/carering.cpp \
        src/resources.cpp \
        src/sosbutton.cpp \
        src/statuscard.cpp \
        src/contact.cpp \
        src/appliance.cpp \
        src/location.cpp
    
    HEADERS += \
        src/mainwindow.h \
        src/dashboard.h \
        src/tracklocation.h \
        src/appliances.h \
        src/carering.h \
        src/resources.h \
        src/sosbutton.h \
        src/statuscard.h \
        src/contact.h \
        src/appliance.h \
        src/location.h
    
    RESOURCES += resources/resources.qrc

Project Structure:
------------------
guardian-path-cpp/
├── CMakeLists.txt          - CMake build configuration
├── README.txt              - This file
├── src/
│   ├── main.cpp            - Application entry point & global styles
│   ├── mainwindow.h/cpp    - Main window with sidebar navigation
│   ├── dashboard.h/cpp     - Dashboard page with SOS button
│   ├── tracklocation.h/cpp - Location tracking page
│   ├── appliances.h/cpp    - Appliance monitoring page
│   ├── carering.h/cpp      - Contact management page
│   ├── resources.h/cpp     - Safety resources directory
│   ├── sosbutton.h/cpp     - SOS button widget
│   ├── statuscard.h/cpp    - Status card widget
│   ├── contact.h/cpp       - Contact data model
│   ├── appliance.h/cpp     - Appliance data model
│   └── location.h/cpp      - Location data model with Haversine formula
└── resources/
    └── resources.qrc       - Qt resource file for icons

Features:
---------
- Dashboard with SOS emergency button
- Location tracking with geofence visualization
- Appliance monitoring with toggle controls
- Care Ring contact management
- Safety resources directory with search/filter

Styling:
--------
The application uses Qt Style Sheets (QSS) defined in main.cpp.
The dark theme uses the following color palette:
- Background: #141824, #1a1f2e
- Cards: #232a3d
- Primary: #4a6cf7 (blue)
- Safe/Success: #22c55e (green)
- Warning: #eab308 (yellow)
- Danger/SOS: #dc2626 (red)
- Text: #f0f0f5 (light), #a0a5b5 (muted), #6b7280 (subtle)
