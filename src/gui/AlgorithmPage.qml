import QtQuick 2.9
import QtQuick.Controls 2.2
import carsevolution 1.0

Component {
    ScrollView {
        id: scrollView
        width: parent.width

        TextArea {
            Accessible.name: "Documentation"
            id: textArea
            padding: 20
            readOnly: true
            baseUrl: "qrc:/"
            text: fileReader.readFile("qrc:/docs/algorithm_overview.html")
            textFormat: Qt.RichText
             wrapMode: TextEdit.WordWrap

            FileIO {
                id: fileReader
            }
        }
    }
}
