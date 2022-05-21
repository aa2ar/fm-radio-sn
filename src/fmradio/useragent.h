/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   useragent.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef USERAGENT_H
#define USERAGENT_H

#include <QVector>
#include <QString>

class ControlPanel;
class UserAgent
{
public:
    UserAgent() = default;
    virtual ~UserAgent() = default;
    virtual int            getControlPanelCount() const = 0;
    virtual ControlPanel * getControlPanelWidget(int) const = 0;
    virtual QString        getControlPanelName(int)   const = 0;
};

#endif // USERAGENT_H
