cmake_minimum_required(VERSION 3.16)
# Add vcpkg toolchain 
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
project(GuardianPath VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ADDED QuickWidgets and Location here
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Positioning QuickWidgets Location)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/dashboard.cpp
    src/dashboard.h
    src/tracklocation.cpp
    src/tracklocation.h
    src/mapview.qml  # ADDED the QML file to sources
    src/appliances.cpp
    src/appliances.h
    src/carering.cpp
    src/carering.h
    src/resources.cpp
    src/resources.h
    src/sosbutton.cpp
    src/sosbutton.h
    src/statuscard.cpp
    src/statuscard.h
    src/contact.cpp
    src/contact.h
    src/appliance.cpp
    src/appliance.h
    src/location.cpp
    src/location.h
    resources/resources.qrc
    src/gpsmanager.cpp
    src/gpsmanager.h
    src/mongodbmanager.cpp
    src/mongodbmanager.h
)

qt_add_executable(GuardianPath WIN32 ${PROJECT_SOURCES})

# ADDED the new Qt modules to the link libraries
target_link_libraries(GuardianPath PRIVATE 
    Qt6::Widgets 
    Qt6::Positioning 
    Qt6::QuickWidgets 
    Qt6::Location
    mongo::mongocxx_shared 
    mongo::bsoncxx_shared
)