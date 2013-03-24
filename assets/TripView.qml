import bb.cascades 1.0

Page {
    property alias triptitle: edTitle.text
    property alias location: edLocation.text
    property alias description: edDescription.text
    property string tripID: ""
    function displayTrip(data) {
        if (data) {
            triptitle = data.title;
            location = data.location;
            description = data.description;
            tripID = data.tripID;
        }
    }
    function displayEditedTrip() {
        edTitle.resetText();
        edLocation.resetText();
        edDescription.resetText();
        edTitle.text = tripContent.triptitle;
        edLocation.text = tripContent.location;
        edDescription.text = tripContent.description;
        tripID = tripContent.tripID;
    }
    titleBar: TitleBar {
        title: "Trip Info"
    }
    attachedObjects: [
        // sheet is not visible still, prepare it and append to the attachedObjects
        // attachedObjects property is a storage for objects which are not supposed to be visible on scene like data models etc.
        Sheet {
            id: tripSheet
            content: TripSheet {
                id: tripContent
            }
        }
    ]
    actions: [
        ActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                tripContent.editTrip(triptitle, location, description, tripID);
                tripSheet.open();
            }
            title: "Edit Trip"
            imageSource: "asset:///images/ic_edit.png"
        }
    ]
    Container {
        Label {
            text: qsTr("Title:")
        }
        TextField {
            id: edTitle
            enabled: false
        }
        Label {
            text: qsTr("Location:")
        }
        TextField {
            id: edLocation
            enabled: false
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
            enabled: false
        }
    }
    function refreshDatamodel(ed) {
        if (ed) {
            _app.updateRecord(tripContent.tripID, tripContent.triptitle, tripContent.location, tripContent.description);
        }
        displayEditedTrip(tripContent);
        tripSheet.close();
        _app.readRecords();
        // navigationPane.pop();
    }
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.log("Page - onCreationCompleted()")
        
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        //  OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
        
        // connect the sheet done signal to the handler slot
        tripContent.edit.connect(refreshDatamodel)
    }
}
