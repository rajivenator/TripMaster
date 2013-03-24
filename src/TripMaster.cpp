// Default empty project template
#include "TripMaster.hpp"
#include "Trip.hpp"
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>
#include <bb/system/SystemDialog>
#include <QtSql/QtSql>
#include <QDebug>

using namespace bb::cascades;
using namespace bb::system;
using namespace bb::data;



TripMaster::TripMaster(bb::cascades::Application *app)
: QObject(app),m_dataModel(0)
{
	initDataModel();
		// create scene document from main.qml asset
	    // set parent to created document to ensure it exists for the whole application lifetime
	    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
	    qml->setContextProperty("_app", this);
	    // create root object for the UI
	    AbstractPane *root = qml->createRootObject<AbstractPane>();
	    // set created root object as a scene
	    app->setScene(root);
	    const bool dbInited = initDatabase();
	    // Inform the UI if the database was successfully initialized or not
	    root->setProperty("databaseOpen", dbInited);
}
//! [0]
void TripMaster::initDataModel()
{
    // Note: The Group Data Model is joining this objects tree as a child (for memory management)
    m_dataModel = new GroupDataModel(this);
    m_dataModel->setSortingKeys(QStringList() << "tripID");
    m_dataModel->setGrouping(ItemGrouping::None);
}
bool TripMaster::initDatabase()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("./data/tripmaster.db");
    if (database.open() == false) {
        const QSqlError error = database.lastError();
     //   alert(tr("Error opening connection to the database: %1").arg(error.text()));
        qDebug() << "\nDatabase NOT opened.";
        return false; // return as if we cannot open a connection to the db, then below calls
                      // will also fail
    }
    else{
    //	 alert("Database Created");
    }

    return true;
}
bool TripMaster::createRecord(const QString &title, const QString &location ,const QString &description)
{
    // 1. Verify the users input is valid.
    //    The SqlQuery's bind functionality will escape a users input ensuring that
    //    characters such as a quote will be properly accepted in the database and
    //    prevent Sql Injection attacks. However, this cannot be relied upon to validate
    //    all the data. In this case, we ensure that at least the firstname OR lastname
    //    contains some form of text.
    if (title.trimmed().isEmpty() && location.trimmed().isEmpty()) {
        alert(tr("You must provide a Title & location name."));
        return false;
    }

    // 2. Get the local DB connection. Note, called database()
    //    Will automatically open a connection to the database
    //    IMPORTANT NOTE: A QSqlQuery object can be created without a reference to
    //    the QSqlDatabase object (it will assume the default database connection
    //    if one is not provided), but, unlike a call to QSqlDatabase::database()
    //    it will not automatically open a connection to the database if it is
    //    currently closed.
    QSqlDatabase database = QSqlDatabase::database();
    // Drop the 'customers' table if it exists so that the application
       // always start with an empty table.
       // Note: A typical application would NOT do this.
       // open the default database.
     /*  QSqlQuery query(database);
       if (query.exec("DROP TABLE IF EXISTS trips")) {
           qDebug() << "Table dropped.";
       } else {
           const QSqlError error = query.lastError();
           alert(tr("Drop table error: %1").arg(error.text()));
       }*/

       // Create the 'customers' table that was just dropped (if it existed)
       // with no data
    	QSqlQuery query(database);
       const QString createSQL = "CREATE TABLE if not exists trips "
                                 "  (tripID INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "  title VARCHAR, "
                                 "  location VARCHAR,"
       						  	 "	description VARCHAR);";
       if (query.exec(createSQL)) {
           qDebug() << "Table created.";
       } else {
           const QSqlError error = query.lastError();
           alert(tr("Create table error: %1").arg(error.text()));
           return false;
       }

    // NOTE: Some applications might verify that the table being inserted to exists
    // at this point, however our application verifies all tables exist at application
    // startup.

    // 3. Create an QSqlQuery object with which you can execute queries
    //    In this example we bind parameters in the query. A large advantage to using
    //    bindings (aside from performance enhancements) is that input is automatically
    //    escaped avoiding potential issues with odd characters (quotes) and prevents
    //    SQL Injection attacks.
    //    Note that for databases that do not support bindings, Qt simulates the binding
    //    effects.
    //    IMPORTANT NOTE: If ever accepting user information without using bindings,
    //    be sure to 'escape' your queries.
   // QSqlQuery query(database);
    query.prepare("INSERT INTO trips"
                  "    (title, location ,description) "
                  "    VALUES (:title, :location, :description)");
    query.bindValue(":title", title);
    query.bindValue(":location", location);
    query.bindValue(":description",description);

    // 4. Execute the query and check the result
    bool success = false;
    if (query.exec()) {
        alert(tr("Create record succeeded."));
        success = true;
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        alert(tr("Create record error: %1").arg(error.text()));
    }

    // 5. Optionally close the database connection if we no longer plan to use it
    //    Note that calling QSqlDatabase::database() will automatically re-open
    //    the connection for us.
    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
    //    with this database connection.
    database.close();

    return success;
}

// Alert Dialog Box Functions
void TripMaster::alert(const QString &message)
{
    SystemDialog *dialog; // SystemDialog uses the BB10 native dialog.
    dialog = new SystemDialog(tr("OK"), 0); // New dialog with on 'Ok' button, no 'Cancel' button
    dialog->setTitle(tr("Alert")); // set a title for the message
    dialog->setBody(message); // set the message itself
    dialog->setDismissAutomatically(true); // Hides the dialog when a button is pressed.

    // Setup slot to mark the dialog for deletion in the next event loop after the dialog has been accepted.
    connect(dialog, SIGNAL(finished(bb::system::SystemUiResult::Type)), dialog, SLOT(deleteLater()));
    dialog->show();
}

//! [3]
GroupDataModel* TripMaster::dataModel() const
{
    return m_dataModel;
}

// Read all records from the database.
// Clear the data model and refill it.
void TripMaster::readRecords()
{
    // 1. Get the local DB connection. Note, called database()
    //    Will automatically open a connection to the database
    //    IMPORTANT NOTE: A QSqlQuery object can be created without a reference to
    //    the QSqlDatabase object (it will assume the default database connection
    //    if one is not provided), but, unlike a call to QSqlDatabase::database()
    //    it will not automatically open a connection to the database if it is
    //    currently closed.
    QSqlDatabase database = QSqlDatabase::database(); // opens the default database.

    // 2. Create a query to search for the records
    //    IMPORTANT NOTE: If accepting user input and not using bindings, be sure
    //    to escape it to allow quote characters, and prevent SQL Injection attacks.
    //    The below example is not a prepared statement and does not use bindings as
    //    there is no user input to accept.
    QSqlQuery query(database);
    const QString sqlQuery = "SELECT tripID,title, location, description FROM trips";

    // 3. Call 'exec' on the SQL Query. Note, that when using a SELECT action,
    //    the retrieved records are stored in the query and accessible by several
    //    different functions (see QSqlQuery documentation for more information).
    if (query.exec(sqlQuery)) {

        // Get the field indexes. We know the order of the fields, and could skip this step.
        // However this will still work if the fields change order in the query string.
        const int tripIDField = query.record().indexOf("tripID");
        const int titleField = query.record().indexOf("title");
        const int locationField = query.record().indexOf("location");
        const int descriptionField = query.record().indexOf("description");
        // The data will be displayed in a group data model
        // Clear any previous reads from the data model first
        m_dataModel->clear();

        // 4. Start navigating through the records by calling the 'next' function.
        //    The next function will position the 'query' at the next record result
        //    allowing you to access the record data via the 'value' function.
        //    The next record will return true as long as it continues to point to valid
        //    record. When there are no longer any records it will return false.
        int recordsRead = 0;
        while (query.next()) {
            // 5. Access the data (stored in the query) via the field indexes
            //    and add the data to the model.
            //    NOTE: When adding an object to a DataModel, the DataModel sets
            //    itself as the parent of the object if no parent has already been
            //    set. Therefore, when clearing or removing an item from the data model
            //    the data model will destory the object if it is the parent of the object.
            Trip *trip = new Trip(query.value(tripIDField).toString(),
                                        query.value(titleField).toString(),
                                        query.value(locationField).toString(),query.value(descriptionField).toString());
            m_dataModel->insert(trip);
            recordsRead++;
        }
        qDebug() << "Read " << recordsRead << " records succeeded";

        if (recordsRead == 0) {
            alert(tr("The Trip table is empty."));
        }
    } else {
        alert(tr("Read records failed: %1").arg(query.lastError().text()));
    }

    // 6. Optionally close the database connection if we no longer plan to use it
    //    Note that calling QSqlDatabase::database() will automatically re-open
    //    the connection for us.
    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
    //    with this database connection.
    database.close();
}

bool TripMaster::updateRecord(const QString &tripID,const QString &title, const QString &location ,const QString &description)
{

	QSqlDatabase database = QSqlDatabase::database();
	    QSqlQuery query(database);
	    const QString sqlCommand = "UPDATE trips "
	                               "    SET title = :title, location = :location , description = :description"
	                               "    WHERE tripID = :tripID";
	    query.prepare(sqlCommand);

	    // Note int he below bindings that firstName, lastName are strings, while customerIDKey
	    // is an integer. The bindValue function is operator overloaded to accept multiple datatypes.
	    query.bindValue(":title", title);
	    query.bindValue(":location", location);
	    query.bindValue(":description", description);
	    query.bindValue(":tripID", tripID);

	    // 4. Execute the query and check the result for errors
	    bool updated = false;
	    if (query.exec()) {
	        // 5. Verify that a row was modified, if not, there was no customer
	        //    with the specified ID
	        if (query.numRowsAffected() > 0) {
	            alert(tr("Trip with id=%1 was updated.").arg(tripID));
	            updated = true;
	        } else {
	            alert(tr("Trip with id=%1 was not found.").arg(tripID));
	        }
	    } else {
	        alert(tr("SQL error: %1").arg(query.lastError().text()));
	    }

	    // 6. Optionally close the database connection if we no longer plan to use it
	    //    Note that calling QSqlDatabase::database() will automatically re-open
	    //    the connection for us.
	    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
	    //    with this database connection.
	    database.close();

	    return updated;
}

bool TripMaster::deleteRecord(const QString &tripID)
{
	  QSqlDatabase database = QSqlDatabase::database();
	  QSqlQuery query(database);
	    query.prepare("DELETE FROM trips WHERE tripID=:tripID");
	    query.bindValue(":tripID", tripID);

	    // 4. Execute the query and check the result for errors
	    bool deleted = false;
	    if (query.exec()) {
	        // 5. Verify that a row was modified, if not, there was no customer
	        //    with the specified ID
	        if (query.numRowsAffected() > 0) {
	            alert(tr("Trip with id=%1 was deleted.").arg(tripID));
	            deleted = true;
	        } else {
	            alert(tr("Trip with id=%1 was not found.").arg(tripID));
	        }
	    } else {
	        alert(tr("SQL error: %1").arg(query.lastError().text()));
	    }

	    // 6. Optionally close the database connection if we no longer plan to use it
	    //    Note that calling QSqlDatabase::database() will automatically re-open
	    //    the connection for us.
	    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
	    //    with this database connection.
	    database.close();

	    return deleted;
}
