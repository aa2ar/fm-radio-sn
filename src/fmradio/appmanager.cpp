/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   appmanager.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "appmanager.h"
#include "core/commands.h"
#include "core/events.h"
#include "core/controller.h"
#include "core/io/protocol.h"

#include "ui/settings/settingsdialog.h"

#include <QFileInfo>
#include <QDebug>

using namespace fmradio;

AppManager::AppManager(SettingsDialog * sd)
    : mSettingsDialog(sd)
{
    qDebug() << __PRETTY_FUNCTION__;

    mEventHandlerQueue.reserve(10);

    mController = new fmradio::controller_t(new fmradio::protocol_t(), this, this, this);

    connect(this, SIGNAL(sendData(QByteArray)), this, SLOT(onSendData(QByteArray)), Qt::QueuedConnection);
    connect(this, SIGNAL(eventEnqueued()), this, SLOT(onEventEnqueued()), Qt::QueuedConnection);

    // serial port setup
    connect(&mPort, SIGNAL(readyRead()), this, SLOT(readData()), Qt::DirectConnection);
    connect(&mPort, SIGNAL(error(QSerialPort::SerialPortError)),
             this,  SLOT(onError(QSerialPort::SerialPortError)));

    connect(&mStatusRequestTimer, &QTimer::timeout, this, [this]{
        mController->send(status_command_t::get());
    });
}

AppManager::~AppManager()
{
    mStatusRequestTimer.stop();
    mController->stop();
    closeSerialPort();
    delete mController;
}

bool AppManager::openSerialPort(const QString & portName)
{
    closeSerialPort();

    //const SettingsDialog::Settings p = m_settings->settings();
    mPort.setPortName(portName);
    mPort.setBaudRate(38400);
//    mPort.setDataBits(QSerialPort::Data8);
//    mPort.setParity(QSerialPort::NoParity);
//    mPort.setStopBits(QSerialPort::OneStop);
//    mPort.setFlowControl(QSerialPort::NoFlowControl);
    if (mPort.open(QIODevice::ReadWrite)) {
//        m_console->setEnabled(true);
//        m_console->setLocalEchoEnabled(p.localEchoEnabled);
//        m_ui->actionConnect->setEnabled(false);
//        m_ui->actionDisconnect->setEnabled(true);
//        m_ui->actionConfigure->setEnabled(false);
//        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
//                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
//                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

        mController->start();

        mController->send(status_command_t::get());

        mStatusRequestTimer.start(1000);

        return true;
    } else {
        //QMessageBox::critical(this, tr("Error"), mPort.errorString());
        //showStatusMessage(tr("Open error"));
    }

    return false;
}

void AppManager::closeSerialPort()
{
    mStatusRequestTimer.stop();
    mController->stop();
    mPort.close();
}

void AppManager::enableLog(bool value)
{
    mController->enable_log(value);
}

void AppManager::logOnlyUnhandled(bool value)
{
    mController->log_only_unhandled(value);
}

int AppManager::rowCount(const QModelIndex & /*parent*/) const
{
    return 8;
}

QVariant AppManager::data(const QModelIndex & index, int /*role*/) const
{
    switch (index.row()) {
    case BusyIndex:           return mBusy;
    case VolumeIndex:         return mVolume;
    case FrequencyIndex:      return mFrequency;
    case CurrentChannelIndex: return mCurrentChannel;
    case ChannelCountIndex:   return mChannelCount;
    case MutedIndex:          return mMuted;
    case StereoIndex:         return mStereo;
    case EarphonesPluggedIndex:  return mEarphonesPlugged;
    }
    return QVariant();
}

int AppManager::volume() const
{
    return mVolume;
}

int AppManager::frequency() const
{
    return mFrequency;
}

bool AppManager::muted() const
{
    return mMuted;
}

void AppManager::onVolumeUp()
{
    if (mController)
        mController->send(volume_up_command_t::get());
}

void AppManager::onVolumeDown()
{
    if (mController)
        mController->send(volume_down_command_t::get());
}

void AppManager::onFrequencyUp()
{
    if (mController && !mBusy) {
        int newValue = mFrequency+1;
        if (mFrequencyRange.frequency_valid(newValue)) {
            set_frequency_command_t * cmd = set_frequency_command_t::get();
            cmd->init(newValue);
            mController->send(cmd);
        }
    }
}

void AppManager::onFrequencyDown()
{
    if (mController && !mBusy) {
        int newValue = mFrequency-1;
        if (mFrequencyRange.frequency_valid(newValue)) {
            set_frequency_command_t * cmd = set_frequency_command_t::get();
            cmd->init(newValue);
            mController->send(cmd);
        }
    }
}

void AppManager::onSetFrequency(int value)
{
    if (mController && !mBusy) {
        set_frequency_command_t * cmd = set_frequency_command_t::get();
        cmd->init(unsigned(value));
        mController->send(cmd);
    }
}

void AppManager::onScanUp()
{
    if (mController && !mBusy)
        mController->send(scan_up_command_t::get());
}

void AppManager::onScanDown()
{
    if (mController && !mBusy)
        mController->send(scan_down_command_t::get());
}

void AppManager::onSetMuted(bool)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void AppManager::onToggleMute()
{
    if (mController)
        mController->send(pause_command_t::get());
}

// event_input_t
void AppManager::enqueue(event_t * e)
{
    std::lock_guard<std::mutex> lock(mEventQueueMutex);
    mEventInputQueue.push_back(e);
    emit eventEnqueued();
}

// event_handler_t
void AppManager::handle_event(event_t *)
{
    qDebug() << "handle_event(event_t)";
}

void AppManager::handle_event(volume_event_t * e)
{
    if (mVolume != e->value()) {
        mVolume = e->value();
        notifyDataChanged(VolumeIndex);
//        if (mUiFace)
//            mUiFace->setVolume(mVolume);
    }
}

void AppManager::handle_event(frequency_event_t * e)
{
    if (mFrequency != e->value()) {
        mFrequency = e->value();
        notifyDataChanged(FrequencyIndex);
//        if (mUiFace)
//            mUiFace->setFrequency(mFrequency);
    }
}

void AppManager::handle_event(campus_on_event_t *)
{
    if (mFrequencyRange.set_campus_mode(true)) {
        // TODO: check frequency
    }
}

void AppManager::handle_event(campus_off_event_t *)
{
    if (mFrequencyRange.set_campus_mode(false)) {
        // TODO: check frequency
    }
}

void AppManager::handle_event(channel_event_t * e)
{
    if (mCurrentChannel != e->value()) {
        mCurrentChannel = e->value();
        notifyDataChanged(CurrentChannelIndex);
    }
}

void AppManager::handle_event(scan_event_t * e)
{
    if (e->value() == scan_event_t::START)
        mBusy = true;
    else if (e->value() == scan_event_t::END)
        mBusy = false;

    notifyDataChanged(BusyIndex);
}

void AppManager::handle_event(control_play_event_t *)
{
    if (mMuted) {
        mMuted = false;
        notifyDataChanged(MutedIndex);
    }
}

void AppManager::handle_event(control_pause_event_t *)
{
    if (!mMuted) {
        mMuted = true;
        notifyDataChanged(MutedIndex);
    }
}

void AppManager::handle_event(earphones_in_event_t *)
{
    if (!mEarphonesPlugged) {
        mEarphonesPlugged = true;
        notifyDataChanged(EarphonesPluggedIndex);
    }
}

void AppManager::handle_event(earphones_out_event_t *)
{
    if (mEarphonesPlugged) {
        mEarphonesPlugged = false;
        notifyDataChanged(EarphonesPluggedIndex);
    }
}

void AppManager::handle_event(audio_mode_event_t * e)
{
    if (mStereo != e->value()) {
        mStereo = e->value();
        notifyDataChanged(StereoIndex);
    }
}

void AppManager::handle_event(version_event_t * /*e*/)
{
    //    qDebug() << e->value();
}

// output_t
int AppManager::write(const char *data, size_t size)
{
    emit sendData(QByteArray(data, size));
    return 0;
}

// io_logger_t
void AppManager::log_input(const char * data)
{
    qDebug() << data;
    if (mSettingsDialog)
        mSettingsDialog->writeLogInput(data);
}

void AppManager::log_output(const char * data)
{
    if (mSettingsDialog)
        mSettingsDialog->writeLogOutput(data);
}

void AppManager::log_info(const char * data)
{
    if (mSettingsDialog)
        mSettingsDialog->writeLogInfo(data);
}

void AppManager::setSerialPort(QString value)
{
    QFileInfo fi(value);
    if (fi.exists() && !fi.isDir())
        openSerialPort(value);
}

// private slots
void AppManager::onSendData(QByteArray ary)
{
    mPort.write(ary);
}

void AppManager::readData()
{
    const QByteArray data = mPort.readAll();
    mController->on_input(data.data(), data.size());
}

void AppManager::onError(QSerialPort::SerialPortError e)
{
    qDebug() << e;
}

void AppManager::onEventEnqueued()
{
    {
        std::lock_guard<std::mutex> lock(mEventQueueMutex);
        mEventHandlerQueue = std::move(mEventInputQueue);
    }

    for (event_t * e: mEventHandlerQueue) {
        e->handle(this);
        e->release();
    }
    mEventHandlerQueue.clear();
}

void AppManager::notifyDataChanged(int row)
{
    QModelIndex i = index(row, 0);
    emit dataChanged(i, i);
}
