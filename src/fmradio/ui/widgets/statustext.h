/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

#ifndef STATUSTEXT_H
#define STATUSTEXT_H

#include "slidingtext.h"

class StatusText : public SlidingText
{
public:
    explicit StatusText(QWidget * parent = 0);

};

#endif // STATUSTEXT_H
