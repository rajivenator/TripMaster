/*
 * Trip.cpp
 *
 *  Created on: 22-Feb-2013
 *      Author: rajivs
 */

#include "Trip.hpp"


Trip::Trip(QObject *parent)
    : QObject(parent)
{
}

Trip::Trip(const QString &id, const QString &title, const QString &location,const QString &description ,QObject *parent)
    : QObject(parent)
    , m_tripID(id)
    , m_title(title)
    , m_location(location)
	, m_description(description)
{
}

QString Trip::tripID() const
{
    return m_tripID;
}

QString Trip::title() const
{
    return m_title;
}

QString Trip::location() const
{
    return m_location;
}

QString Trip::description() const
{
    return m_description;
}
void Trip::setTripID(const QString &newId)
{
    if (newId != m_tripID) {
    	m_tripID = newId;
        emit tripIDChanged(newId);
    }
}

void Trip::setTitle(const QString &newTitle)
{
    if (newTitle != m_title) {
        m_title = newTitle;
        emit titleChanged(newTitle);
    }
}

void Trip::setLocation(const QString &newLocation)
{
    if (newLocation != m_location) {
    	m_location = newLocation;
        emit locationChanged(newLocation);
    }
}

void Trip::setDescription(const QString &newDescription)
{
	if (newDescription != m_description) {
		m_description = newDescription;
	        emit descriptionChanged(newDescription);
	    }
}
