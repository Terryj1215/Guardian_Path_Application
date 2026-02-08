Markdown
Guardian Path - C++ Qt Application
===================================

Prerequisites:
--------------
1. Visual Studio 2022 (recommended) or later
2. Qt 6.x installed (https://www.qt.io/download)
   - Required Modules: Qt Positioning, Qt Location
3. Qt VS Tools extension for Visual Studio
4. CMake 3.16 or later
5. MongoDB C++ Driver (mongocxx) via vcpkg

Setup Instructions:
-------------------

Option A: Using CMake with Visual Studio
1. Open Visual Studio.
2. Select "Open a local folder" and navigate to the guardian-path-cpp folder.
3. Visual Studio will detect the CMakeLists.txt automatically.
4. Configure Qt path in CMakeSettings.json if needed.
5. Build > Build All.

Option B: Using Qt Creator
1. Open Qt Creator.
2. File > Open File or Project.
3. Select CMakeLists.txt.
4. Configure the project with your Qt kit.
5. Build > Build Project.

Project Structure:
------------------
guardian-path-cpp/
├── CMakeLists.txt          - CMake build configuration
├── src/
│   ├── main.cpp            - Application entry point & global styles
│   ├── mainwindow.h/cpp    - Main window with sidebar navigation
│   ├── mongodbmanager.h/cpp - MongoDB controller & CRUD logic
│   ├── dashboard.h/cpp      - Dashboard page with SOS button
│   ├── tracklocation.h/cpp - Location tracking page
│   ├── appliances.h/cpp    - Appliance monitoring page
│   ├── carering.h/cpp      - Contact management page
│   ├── contact.h/cpp       - Contact data model with ObjectID support
│   ├── location.h/cpp      - Location data model with Haversine formula
│   └── sosbutton.h/cpp     - SOS button widget
└── resources/
    └── resources.qrc       - Qt resource file for icons

Features:
---------
- Dashboard with SOS emergency button and Quick Contacts.
- Location tracking with geofence visualization and Haversine distance.
- Appliance monitoring with toggle controls.
- Care Ring contact management with MongoDB persistence.

## 🚀 Enhancement Two: Algorithms & Geospatial Integration
[cite_start]In this milestone, the application was upgraded from a static UI mockup to a dynamic system utilizing complex geospatial algorithms and third-party API integration[cite: 43, 44].

### Key Technical Features:
* [cite_start]**Interactive Mapping Engine:** Integrated OpenStreetMap (OSM) via QML to provide real-time visual feedback of user locations[cite: 43, 45].
* [cite_start]**Geocoding Search Algorithm:** Implemented an asynchronous search feature that translates human-readable addresses into precise geographic coordinates[cite: 52].
* [cite_start]**Haversine Distance Logic:** Developed a backend algorithm using the Haversine formula to calculate the great-circle distance between the user and the "Safe Zone" center[cite: 48, 49].
* [cite_start]**Dynamic Geofencing:** Created a real-time boundary system where user-defined radii trigger safety alerts and visual UI changes (Green to Red)[cite: 45, 56].

## 🚀 Enhancement Three: Database Integration & Persistence
The final enhancement transitioned the application from volatile local memory to a professional NoSQL persistence layer using MongoDB.

### Key Technical Features:
* [cite_start]**MongoDB Backend Integration:** Replaced hardcoded sample arrays with a live MongoDB connection for persistent storage[cite: 8].
* [cite_start]**Full CRUD Implementation:** Developed a robust system for Creating, Reading, Updating (Set Primary), and Deleting (CRUD) contact data via the C++ interface[cite: 14].
* **Type-Safe ObjectID Refactoring:** Refactored the Contact data model to utilize QString identifiers to map UI elements directly to MongoDB ObjectIDs (OIDs).
* **State-Persistent UX:** Implemented synchronization logic ensuring the Dashboard and Care Ring views are always in parity with the database state.

### Technical Stack:
* [cite_start]**Language:** C++17 [cite: 6]
* [cite_start]**Framework:** Qt 6.9 (Widgets, QML, Location, Positioning) [cite: 6, 43, 61]
* [cite_start]**Database:** MongoDB (NoSQL) [cite: 47, 52]
* [cite_start]**Build System:** CMake [cite: 30]
