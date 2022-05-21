/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   defaulthorizontalpanel.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef DEFAULTDISPLAY_H
#define DEFAULTDISPLAY_H

#include "fmradio/controlpanel.h"

namespace Ui {
class DefaultHorizontalPanel;
}

class QToolButton;
class QButtonGroup;
class QAbstractItemModel;
class UiControl;
class DefaultHorizontalPanel : public ControlPanel
{
    Q_OBJECT

public:
    explicit DefaultHorizontalPanel(QWidget *parent = 0);
    ~DefaultHorizontalPanel();

    void setControl(UiControl *) override;


    void setChannels(const QVector<QPair<int, QString> > &);

    QSize sizeHint() const override;

signals:

private:
    // ignore mouse events
    void mousePressEvent(QMouseEvent * e)   override;
    void mouseReleaseEvent(QMouseEvent * e) override;
    void mouseMoveEvent(QMouseEvent * e)    override;

private:
    Ui::DefaultHorizontalPanel * ui;

    int mChannelButtonsWidth = 250;

    QButtonGroup * mCommandButtonGroup = nullptr;
    QButtonGroup * mActionButtonGroup = nullptr;

    void onControlButtonClick(int);

    // widget event handlers
    void volumeUp();
    void volumeDown();
    void frequencyUp();
    void frequencyDown();
    void scanUp();
    void scanDown();
    void toggleMute();

    // model actions
    void setVolume(int);
    void setFrequency(int);
    void setMute(bool);
    void setStereo(bool);
    void setOutputPlugged(bool);
    void setCurrentChannel(int);
    void setChannelCount(int);
    void setScanning(bool);
    void setBusy(bool);

    void syncControlModelData(int) override;
    void syncPlayListModelData() override;


    QToolButton * createChannelButton(const QString &, int);
    void createChannelButtons();
    void updateChannelButtons();
    QString channelButtonText(int);
    void setStatusChannelName(int);
};

#endif // DEFAULTDISPLAY_H
