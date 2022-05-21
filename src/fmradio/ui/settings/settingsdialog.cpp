/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   settingsdialog.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "logmodel.h"
#include "logmodeldelegate.h"
#include "../playlist/playlistmodel.h"
#include "../playlist/playlistmodeleditordelegate.h"
#include <QApplication>
#include <QPushButton>
#include <QCloseEvent>
#include <QStandardItemModel>
#include <QDebug>

QPoint SettingsDialog::position(-1, -1);

SettingsDialog::SettingsDialog(PlayListModel * playlistModel, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , mPlaylistModel(playlistModel)
{
    setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);

    ui->setupUi(this);

    ui->caption->setAttribute(Qt::WA_TransparentForMouseEvents);

    QLabel * logo = new QLabel(
                QStringLiteral("FM Radio SN v") + qApp->applicationVersion(),
                ui->tabWidget);
    QFont font = logo->font();
    font.setBold(true);
    logo->setFont(font);
    //logo->setFrameShape(QFrame::Box);
    ui->tabWidget->setCornerWidget(logo);

    connect(ui->edSerialPort, &QLineEdit::textEdited, this, &SettingsDialog::serialPortChanged);

    ui->playlistTable->setModel(mPlaylistModel);
    ui->playlistTable->setItemDelegate(new PlayListModelEditorDelegate(ui->playlistTable->font(), this));
    ui->playlistTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->playlistTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->playlistTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->playlistTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    connect(ui->playlistTable, &QTableView::clicked, mPlaylistModel, &PlayListModel::clicked);
    connect(ui->edAddPlaylistItemFreq, &QLineEdit::returnPressed, this, &SettingsDialog::addPlaylistItem);
    connect(ui->btnAddPlaylistItem, &QToolButton::clicked, this, &SettingsDialog::addPlaylistItem);
    connect(ui->btnRemovePlaylistItem, &QToolButton::clicked, this, &SettingsDialog::removePlaylistItem);

    mLogModel = new LogModel(this);
    ui->listView->setModel(mLogModel);
    ui->listView->setItemDelegate(new LogModelDelegate(mLogModel,
                                                       ui->listView->font(),
                                                       ui->listView->palette(), this));

    connect(ui->cbEnableLog, &QCheckBox::clicked, this, &SettingsDialog::enableLog);
    connect(ui->cbLogUnhandledOnly, &QCheckBox::clicked, this, &SettingsDialog::logOnlyUnhandled);
    connect(ui->btnClearLog, &QToolButton::clicked, mLogModel, &LogModel::clear);

    connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::close);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::open()
{
    if (isVisible())
        activateWindow();
    else
        show();
}

void SettingsDialog::showEvent(QShowEvent *)
{
    if (position.x() > -1)
        move(position);

    emit enableLog(ui->cbEnableLog->isChecked());

    emit aboutToShow();
}

void SettingsDialog::closeEvent(QCloseEvent * event)
{
    event->ignore();
    position = pos();
    emit enableLog(false);
    emit aboutToHide();
    hide();
}

void SettingsDialog::writeLogInput(const char * data)
{
    if (isVisible()) {
        mLogModel->addInputData(QString(data));
        if (ui->cbAutoScroll->isChecked())
            ui->listView->scrollToBottom();
    }
}

void SettingsDialog::writeLogOutput(const char * data)
{
    if (isVisible()) {
        mLogModel->addOutputData(QString(data));
        if (ui->cbAutoScroll->isChecked())
            ui->listView->scrollToBottom();
    }
}

void SettingsDialog::writeLogInfo(const char * data)
{
    if (isVisible()) {
        mLogModel->addInfoData(QString(data));
        if (ui->cbAutoScroll->isChecked())
            ui->listView->scrollToBottom();
    }
}

void SettingsDialog::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "SettingsDialog::mousePressEvent";
    mDragStartingPos = event->pos();
}

void SettingsDialog::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "SettingsDialog::mouseReleaseEvent";
}

void SettingsDialog::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "SettingsDialog::mouseMoveEvent: " << event->pos() - mDragStartingPos;
    move(pos() + (event->pos() - mDragStartingPos));
}

void SettingsDialog::addPlaylistItem()
{
    bool ok = false;
    float ffreq = ui->edAddPlaylistItemFreq->text().toFloat(&ok);
    if (ok) {
        int freq = int(ffreq * 10.0f + 0.5f);
        mPlaylistModel->insertItem(freq, "");
    }
}

void SettingsDialog::removePlaylistItem()
{
    mPlaylistModel->removeItem(ui->playlistTable->currentIndex());
}
