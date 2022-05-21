/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   myuseragent.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef MYUSERAGENT_H
#define MYUSERAGENT_H

#include "fmradio/useragent.h"

class MyUserAgent : public UserAgent
{
public:
    MyUserAgent();

    virtual int            getControlPanelCount()     const;
    virtual ControlPanel * getControlPanelWidget(int) const;
    virtual QString        getControlPanelName(int)   const;
};

#endif // MYUSERAGENT_H
