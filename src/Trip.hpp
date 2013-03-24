/*
 * Trip.hpp
 *
 *  Created on: 22-Feb-2013
 *      Author: rajivs
 */

#ifndef TRIP_HPP_
#define TRIP_HPP_
#include <QObject>
class Trip:public QObject {


	 Q_OBJECT

	    // These are the properties that will be accessible by the datamodel in the view.
	    Q_PROPERTY(QString tripID READ tripID WRITE setTripID NOTIFY tripIDChanged FINAL)
	    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
	    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged FINAL)
	    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)

	public:
	    Trip(QObject *parent = 0);
	    Trip(const QString &id, const QString &title, const QString &location, const QString &description, QObject *parent = 0);

	    QString tripID() const;
	    QString title() const;
	    QString location() const;
	    QString description() const;

	    void setTripID(const QString &newId);
	    void setTitle(const QString &newTitle);
	    void setLocation(const QString &newLocation);
	    void setDescription(const QString &newDescription);

	Q_SIGNALS:

	    void tripIDChanged(const QString &newId);
	    void titleChanged(const QString &title);
	    void locationChanged(const QString &location);
	    void descriptionChanged(const QString &description);

	private:
	    QString m_tripID;
	    QString m_title;
	    QString m_location;
	    QString m_description;
};

#endif /* TRIP_HPP_ */
