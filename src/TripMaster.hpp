// Default empty project template
#ifndef TripMaster_HPP_
#define TripMaster_HPP_

#include <QObject>
#include <bb/cascades/GroupDataModel>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class TripMaster : public QObject
{
    Q_OBJECT
    // A property that is used by the list view in QML
    Q_PROPERTY(bb::cascades::DataModel* dataModel READ dataModel CONSTANT)

public:
    TripMaster(bb::cascades::Application *app);
    virtual ~TripMaster() {}
    Q_INVOKABLE bool createRecord(const QString &title, const QString &location,const QString &description);
    Q_INVOKABLE void readRecords();
    Q_INVOKABLE bool updateRecord(const QString &tripID,const QString &title, const QString &location,const QString &description);
    Q_INVOKABLE bool deleteRecord(const QString &tripID);
private:
    // Functions to call upon initialization to setup the model and database
    void initDataModel();
    bool initDatabase();
    void alert(const QString &message);
    // The getter method for the property
       bb::cascades::GroupDataModel* dataModel() const;

       // The data shown by the list view.
       bb::cascades::GroupDataModel* m_dataModel;
};


#endif /* TripMaster_HPP_ */
