import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15


ApplicationWindow {
    id: root
	title: "uCAN Monitor"
    visible: true
    
    minimumWidth: 300
    minimumHeight: layout.height
    maximumWidth: 300
    maximumHeight: layout.height
    
    ButtonGroup {
        id: server_buttongroup
    }

    ColumnLayout {
        id: layout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        ListView {
            id: server_listview
            Layout.margins: 10
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            width: 280
            height: contentHeight
            model: server_selector.server_list
            delegate: RadioDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                text: modelData
                ButtonGroup.group: server_buttongroup
                checked: index == 0
                background: Rectangle {
                    opacity: 0
                }
            }
        }

        RowLayout {
            Layout.margins: 10
            Layout.alignment: Qt.AlignCenter
            spacing: 50

            Button {
                id: exit_button
                Layout.alignment: Qt.AlignCenter
                text: "Exit"
                width: 100
                onClicked: {
                    Qt.quit();
                }
            }

            Button {
                id: ok_button
                Layout.alignment: Qt.AlignCenter
                text: "Ok"
                width: 100
                onClicked: {
                    var component = Qt.createComponent("../main.qml")
                    var window = component.createObject(null)
                    window.show()
                    root.close();
                    server_selector.create(server_buttongroup.checkedButton.text);
                }
            }
        }
    }

}