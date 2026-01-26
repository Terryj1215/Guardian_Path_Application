cmake_minimum_required(VERSION 3.16)
project(GuardianPath VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/dashboard.cpp
    src/dashboard.h
    src/tracklocation.cpp
    src/tracklocation.h
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
)

qt_add_executable(GuardianPath
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(GuardianPath PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(GuardianPath PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(GuardianPath)
