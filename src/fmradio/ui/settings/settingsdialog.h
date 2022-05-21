/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   settingsdialog.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class PlayListModel;
class LogModel;
class LogModelDelegate;
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(PlayListModel *, QWidget *parent = 0);
    ~SettingsDialog();

    void open() override;

    void showEvent(QShowEvent *) override;
    void closeEvent(QCloseEvent *) override;

    void writeLogInput(const char *);
    void writeLogOutput(const char *);
    void writeLogInfo(const char *);

signals:
    void aboutToShow();
    void aboutToHide();

    void serialPortChanged(QString);

    void enableLog(bool);
    void logOnlyUnhandled(bool);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::SettingsDialog *ui;
    LogModel * mLogModel = nullptr;

    static QPoint position;
    QPoint mDragStartingPos;

    PlayListModel * mPlaylistModel = nullptr;

    void addPlaylistItem();
    void removePlaylistItem();
};

#endif // SETTINGSDIALOG_H
