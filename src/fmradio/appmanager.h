/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   appmanager.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include "core/eventinput.h"
#include "core/eventhandler.h"
#include "core/frequencyrange.h"
#include "core/io/output.h"
#include "core/inputlogger.h"

#include "ui/uicontrol.h"

#include <QAbstractListModel>
#include <QSerialPort>
#include <QTimer>
#include <mutex>

// events:
// display
// - set frequency, mute, mono/stereo, earphones on/off, channel number,
// - set channel name
// volume knob
// - set value
// frequency knob
// - no event

// commands:


namespace fmradio
{
    class controller_t;
}

class SettingsDialog;
class AppManager
        : public QAbstractListModel
        , public UiControl
        , public fmradio::event_input_t
        , public fmradio::event_handler_t
        , public fmradio::output_t
        , public fmradio::io_logger_t
{
    Q_OBJECT
public:
    AppManager(SettingsDialog *);
    ~AppManager();

    bool openSerialPort(const QString &);
    void closeSerialPort();

    // QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex())             const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    // UiControl
    int  volume()     const override;
    int  frequency()  const override;
    bool muted()      const override;
    void onVolumeUp()       override;
    void onVolumeDown()     override;
    void onFrequencyUp()    override;
    void onFrequencyDown()  override;
    void onSetFrequency(int)override;
    void onScanUp()         override;
    void onScanDown()       override;
    void onSetMuted(bool)   override;
    void onToggleMute()     override;

    // event_input_t
    void enqueue(fmradio::event_t *) override;

    // event_handler_t
    void handle_event(fmradio::event_t *) override;
    void handle_event(fmradio::volume_event_t *) override;
    void handle_event(fmradio::frequency_event_t *) override;
    void handle_event(fmradio::campus_on_event_t *)  override;
    void handle_event(fmradio::campus_off_event_t *) override;
    void handle_event(fmradio::channel_event_t *) override;
    void handle_event(fmradio::scan_event_t *) override;
    void handle_event(fmradio::control_play_event_t *) override;
    void handle_event(fmradio::control_pause_event_t *) override;
    void handle_event(fmradio::earphones_in_event_t *) override;
    void handle_event(fmradio::earphones_out_event_t *) override;
    void handle_event(fmradio::audio_mode_event_t *) override;
    void handle_event(fmradio::version_event_t *) override;

    // output_t
    int write(const char * data, size_t size) override;

    // io_logger_t
    void log_input(const char *)  override;
    void log_output(const char *) override;
    void log_info(const char *)   override;

signals:
    void sendData(QByteArray);
    void eventEnqueued();

public slots:
    void setSerialPort(QString);
    void enableLog(bool);
    void logOnlyUnhandled(bool);

private slots:
    void onSendData(QByteArray);
    void readData();
    void onError(QSerialPort::SerialPortError);
    void onEventEnqueued();

private:
    enum {
        BusyIndex           = 0,
        VolumeIndex         = 1,
        FrequencyIndex      = 2,
        CurrentChannelIndex = 3,
        ChannelCountIndex   = 4,
        MutedIndex          = 5,
        StereoIndex         = 6,
        EarphonesPluggedIndex = 7,
    };

    QSerialPort mPort;
    QTimer mStatusRequestTimer;

    SettingsDialog * mSettingsDialog = nullptr;

    fmradio::controller_t * mController = nullptr;

    std::vector<fmradio::event_t *> mEventInputQueue;
    std::vector<fmradio::event_t *> mEventHandlerQueue;
    std::mutex mEventQueueMutex;

    int  mVolume         = 0;
    int  mFrequency      = 0;
    int  mCurrentChannel = 0;
    int  mChannelCount   = 0;
    bool mMuted          = false;
    bool mStereo         = false;
    bool mEarphonesPlugged  = false;

    fmradio::frequency_range_t mFrequencyRange;

    bool mBusy = false;

    void notifyDataChanged(int);
};

#endif // UICONTROLLER_H
