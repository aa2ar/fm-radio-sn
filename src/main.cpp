/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   main.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "fmradio/appwindow.h"
#include "myuseragent.h"
#include <QMainWindow>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QStringLiteral("fm-radio-sn"));
    a.setApplicationDisplayName(QStringLiteral("FM Radio SN"));
    a.setApplicationVersion(QStringLiteral("1.0"));

    AppWindow w(new MyUserAgent);
    w.show();

    return a.exec();
}
