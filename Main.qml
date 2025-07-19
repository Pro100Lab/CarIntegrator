import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// 1. Выберите и импортируйте нужный стиль (один из вариантов)
import QtQuick.Controls.Material  // Material Design
// Или: import QtQuick.Controls.Fusion
// Или: import QtQuick.Controls.Universal

ApplicationWindow {
    id: mw
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    // 2. Установите стиль для всего приложения
    Component.onCompleted: {
        // Для Material:
        Material.theme = Material.Light
        Material.accent = Material.Blue

        // Или для Fusion:
        // Application.style = "Fusion"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40

        Repeater
        {
            model: [
                {
                    text: "1"
                },
                {
                    text: "2"
                },
                {
                    text: "3"
                },
                {
                    text: "4"
                },
                {
                    text: "5"
                }
            ]


            Button {
                id: fancyButton
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.preferredWidth: 90
                onClicked: debugHelper.pingDebug(modelData.text)
                text: modelData.text

                // 3. Явно отключаем нативный стиль
                property bool __customBackground: true  // Ключевая строка!

                background: Rectangle {
                    id: bg
                    radius: 10
                    color: "#4facfe"

                    // Имитация тени
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -2
                        radius: 10
                        color: "#60000000"
                        z: -1
                    }
                }

                // Эффект при наведении
                states: State {
                    name: "hovered"
                    when: fancyButton.hovered
                    PropertyChanges {
                        target: bg
                        color: "#6a85b6"
                    }
                }

                Behavior on scale { NumberAnimation { duration: 100 } }
                onPressedChanged: scale = pressed ? 0.95 : 1.0
            }
        }
    }
}
