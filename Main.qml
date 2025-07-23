import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import DeviceNS 1.0
// 1. Выберите и импортируйте нужный стиль (один из вариантов)
import QtQuick.Controls.Material
import QtQuick.Controls.Fusion

// Material Design
// Или: import QtQuick.Controls.Fusion
// Или: import QtQuick.Controls.Universal
ApplicationWindow {
    id: mw
    width: 640
    height: 480
    visible: true
    title: qsTr("CarIntegrator")

    // 2. Установите стиль для всего приложения
    Component.onCompleted: {
        // Для Material:
        // Material.theme = Material.Light
        // Material.accent = Material.Blue

        // Или для Fusion:
        Application.style = "Fusion"
    }

    GridLayout {
        columns: 6
        rows: 4
        anchors.fill: parent

        ColumnLayout {
            Layout.columnSpan: 3
            Layout.rowSpan: 4
            Layout.margins: 20
            Layout.alignment: Qt.AlignTop

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true

                color: "white"

                Column {
                    Text {
                        text: "Об устройстве"
                    }

                    Row {
                        Text {
                            text: "IP Adress:"
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            text: "127.0.0.1"
                        }
                    }

                    Row {
                        Text {
                            text: "Название устройства: "
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            text: deviceState.deviceInfo.name
                        }
                    }

                    Row {
                        Text {
                            text: "Статус устройства: "
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        Text {
                            text: deviceState.deviceStatus.description
                        }
                    }
                }
            }

            Rectangle {
                color: "white"
                Layout.fillHeight: true
                Layout.fillWidth: true

                Column {
                    Text {
                        text: "Подключенные устройства"
                    }
                    ListView {
                        id: connectorsListView
                        model: serviceBus.connectors

                        delegate: Text {
                            text: modelData
                        }

                        width: 100
                        height: 150
                    }
                }
            }
        }

        ColumnLayout {
            Layout.columnSpan: 3
            Layout.rowSpan: 4
            Layout.margins: 20
            Layout.alignment: Qt.AlignTop

            Layout.fillWidth: true
            Layout.fillHeight: true
            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true

                color: "white"

                Text {
                    text: "Виджеты"
                }
            }
        }
    }
}
