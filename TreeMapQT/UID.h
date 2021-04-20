#pragma once

#include <QUuid>

class cUID
{
public:
	cUID() 
            : m_uid(QUuid::createUuid().toString())
	{
	}

        cUID(const QString& uid)
            : m_uid(uid)
        {
        }

	virtual ~cUID() {}

        QString UID()
        {
            return m_uid;
        }
private:
        QString m_uid;
};

