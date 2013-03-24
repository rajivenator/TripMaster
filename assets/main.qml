import bb.cascades 1.0

TabbedPane {
    id: tabPaneTrippings
    activeTab: tabExpenses
    objectName: "tabPaneTrippings"
    showTabsOnActionBar: false
    sidebarState: SidebarState.Hidden
    Tab {
        id: tabTrip
        description: "Add your trips here"
        title: "Trips"
        objectName: "tabTrip"
        onTriggered: {
            _app.readRecords();
        }
        NavigationPane {
            id: navigationPane
            Page {
                attachedObjects: [
                    // sheet is not visible still, prepare it and append to the attachedObjects
                    // attachedObjects property is a storage for objects which are not supposed to be visible on scene like data models etc.
                    Sheet {
                        id: tripSheet
                        content: TripSheet {
                            id: tripContent
                        }
                    },
                    ComponentDefinition {
                        id: tripView
                        source: "TripView.qml"
                    }
                ]
                actions: [
                    ActionItem {
                        ActionBar.placement: ActionBarPlacement.OnBar
                        onTriggered: {
                            tripContent.addTrip();
                            tripSheet.open();
                        }
                        title: "New Trip"
                        imageSource: "asset:///images/ic_add.png"
                    }
                ]
                content: Container {
                    ListView {
                        id: listView
                        dataModel: _app.dataModel
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1.0
                        }
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                        listItemComponents: [
                            ListItemComponent {
                                type: "item"
                                StandardListItem {
                                    title: qsTr("%1 %2").arg(ListItemData.title).arg(ListItemData.location)
                                    description: qsTr("Description: %1").arg(ListItemData.description)
                                }
                            }
                        ]
                        
                        onTriggered: {
                            var page = tripView.createObject();
                            var data = dataModel.data(indexPath);
                            page.displayTrip(data);
                            navigationPane.push(page);
                            //tripSheet.open();
                        }
                    }
                }
                function refreshDatamodel(ok) {
                    if (ok) {
                        _app.createRecord(tripContent.triptitle, tripContent.location, tripContent.description);
                    }
                    _app.readRecords();
                    tripSheet.close();
                }
                onCreationCompleted: {
                    // this slot is called when declarative scene is created
                    // write post creation initialization here
                    console.log("Page - onCreationCompleted()")
                    
                    // enable layout to adapt to the device rotation
                    // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
                    //  OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
                    
                    // connect the sheet done signal to the handler slot
                    tripContent.add.connect(refreshDatamodel)
                }
            }
        }
    
    }
    Tab {
        
        id: tabExpenses
        title: "Expenses"        
        NavigationPane {
            Page {
                actions: ActionItem {
                    title: "Add Expense"
                    imageSource: "asset:///images/ic_add.png"
                    ActionBar.placement : ActionBarPlacement.OnBar
                }
            }
        }
    }
    Tab {
        id: tabTrippers
        title: "Trippers"
        NavigationPane {
            Page {
                actions: ActionItem {
                    title:"Add Trippers"
                    imageSource: "asset:///images/ic_add.png"
                    ActionBar.placement : ActionBarPlacement.OnBar
                }
            }
        }
    }
    
    onCreationCompleted: {
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }

}
