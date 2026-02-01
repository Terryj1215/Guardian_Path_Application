import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning

Item {
    id: root
    width: 600; height: 500

    property double currentLat: 40.7128
    property double currentLon: -74.0060
    property double homeLat: 40.7128
    property double homeLon: -74.0060
    property double radius: 500

    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter { name: "osm.useragent"; value: "GuardianPath" }
    }

    // This is the engine that finds your home address
    GeocodeModel {
        id: geocodeModel
        plugin: mapPlugin
        onStatusChanged: {
            if (status === GeocodeModel.Ready && count > 0) {
                var res = get(0).coordinate;
                map.center = res; // Move map to your home
                trackLocationWidget.onMapClicked(res.latitude, res.longitude); // Update C++
            }
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(currentLat, currentLon)
        zoomLevel: 14

        MapCircle {
            center: QtPositioning.coordinate(homeLat, homeLon)
            radius: root.radius
            color: "#444a6cf7"; border.width: 2; border.color: "#4a6cf7"
        }

        MapQuickItem {
            coordinate: QtPositioning.coordinate(currentLat, currentLon)
            anchorPoint.x: 16; anchorPoint.y: 32
            sourceItem: Text { text: "📍"; font.pixelSize: 32 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: (mouse) => {
                var coord = map.toCoordinate(Qt.point(mouse.x, mouse.y));
                trackLocationWidget.onMapClicked(coord.latitude, coord.longitude);
            }
        }
    }

    // THE ADDRESS SEARCH BAR
    Rectangle {
        anchors { top: parent.top; left: parent.left; right: parent.right; margins: 15 }
        height: 45; color: "#232a3d"; radius: 8; border.color: "#4a6cf7"; border.width: 2

        TextInput {
            id: searchInput
            anchors.fill: parent; anchors.margins: 12
            color: "white"; verticalAlignment: TextInput.AlignVCenter
            font.pixelSize: 14; focus: true
            text: "Type address here and press Enter..."
            
            // When you hit Enter, it starts the search
            onAccepted: {
                geocodeModel.query = text;
                geocodeModel.update();
            }
            
            onActiveFocusChanged: if(activeFocus) text = ""
        }
    }
}