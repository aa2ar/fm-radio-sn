/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   myuseragent.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "myuseragent.h"
#include "ui/panels/defaulthorizontalpanel.h"

MyUserAgent::MyUserAgent()
{

}

int MyUserAgent::getControlPanelCount() const
{
    return 1;
}

ControlPanel * MyUserAgent::getControlPanelWidget(int) const
{
    DefaultHorizontalPanel * dd = new DefaultHorizontalPanel;
    return dd;
}

QString MyUserAgent::getControlPanelName(int) const
{
    return QStringLiteral("Default");
}

