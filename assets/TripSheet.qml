import bb.cascades 1.0

Page {
    property bool databaseOpen: false //Not required right now cleanup
    property alias triptitle: edTitle.text
    property alias location: edLocation.text
    property alias description: edDescription.text
    property string tripID: ""
    property bool isEdit: false
    signal add(bool ok)
    signal edit(bool ed)
    function addTrip() {
        edTitle.resetText();
        edLocation.resetText();
        edDescription.resetText();
        isEdit = false;
    }
    function editTrip(vTitle, vLocation, vDescription, vTripID) {
        edTitle.resetText();
        edLocation.resetText();
        edDescription.resetText();
        edTitle.text = vTitle;
        edLocation.text = vLocation;
        edDescription.text = vDescription;
        tripID = vTripID;
        isEdit = true;
    }
    titleBar: TitleBar {
        dismissAction: ActionItem {
            onTriggered: {
                if (isEdit) {
                    edit(false);
                } else {
                    add(false)
                }
            }
            title: "Cancel"
        }
        acceptAction: ActionItem {
            title: "Save"
            onTriggered: {
                if (isEdit) {
                    edit(true);
                } else {
                    add(true)
                }
            }
        }
        title: "Trip Info"
    }
    Container {
        Label {
            text: qsTr("Title:")
        }
        TextField {
            id: edTitle
        }
        Label {
            text: qsTr("Location:")
        }
        TextField {
            id: edLocation
        }
        Label {
            text: qsTr("Trip Description:")
        }
        TextArea {
            id: edDescription
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }
    }
}
