/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
 * @file   defaulthorizontalpanel.cpp
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#include "defaulthorizontalpanel.h"
#include "ui_defaulthorizontalpanel.h"
#include "fmradio/ui/widgets/tunerknob.h"
#include "fmradio/ui/uicontrol.h"
#include <QToolButton>
#include <QButtonGroup>
#include <QMouseEvent>
#include <QMenu>
#include <QFontDatabase>
#include <QDebug>

DefaultHorizontalPanel::DefaultHorizontalPanel(QWidget *parent) :
    ControlPanel(parent),
    ui(new Ui::DefaultHorizontalPanel)
{
    ui->setupUi(this);

    createChannelButtons();

    mActionButtonGroup = new QButtonGroup(this);
    mActionButtonGroup->addButton(ui->btnPlaylist, ControlPanel::Playlist);
    mActionButtonGroup->addButton(ui->btnSettings, ControlPanel::Settings);

    ui->volumeKnob->initLabel(
                QPixmap(":/rc/img/sound_mute-16x16.png"),
                QPixmap(":/rc/img/sound_norm-16x16.png"));
    ui->radioKnob->initLabel(
                QPixmap(":/rc/img/scan_up_down_n-20x16.png"),
                QPixmap(":/rc/img/scan_up_down_h-20x16.png"));

    //int id = QFontDatabase::addApplicationFont(":/7seg");
    //QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    //QFont font7seg(family, 22);
    //ui->freqValue->setFont(font7seg);

    // volume knob setup
    connect(ui->volumeKnob, &TunerKnob::rotatedClockwise, this, &DefaultHorizontalPanel::volumeUp);
    connect(ui->volumeKnob, &TunerKnob::rotatedCounterClockwise, this, &DefaultHorizontalPanel::volumeDown);
    connect(ui->volumeKnob, &TunerKnob::clicked, this, &DefaultHorizontalPanel::toggleMute);

    // frequency knob setup
    connect(ui->radioKnob, &TunerKnob::rotatedClockwise, this, &DefaultHorizontalPanel::frequencyUp);
    connect(ui->radioKnob, &TunerKnob::rotatedCounterClockwise, this, &DefaultHorizontalPanel::frequencyDown);
    connect(ui->radioKnob, &RadioKnob::leftClicked, this, &DefaultHorizontalPanel::scanDown);
    connect(ui->radioKnob, &RadioKnob::rightClicked, this, &DefaultHorizontalPanel::scanUp);

    // action buttons setup
    connect(mActionButtonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &ControlPanel::action);
}

DefaultHorizontalPanel::~DefaultHorizontalPanel()
{
    delete ui;
}

void DefaultHorizontalPanel::setControl(UiControl * c)
{
    mControl = c;
    if (mControl) {
        setVolume(mControl->volume());
        setFrequency(mControl->frequency());
    }
}

void DefaultHorizontalPanel::setVolume(int value)
{
    ui->volumeKnob->setValue(value);
}

void DefaultHorizontalPanel::setFrequency(int value)
{
    ui->freqValue->setText(QString().sprintf("%.1f", value / 10.0f));
    setStatusChannelName(value);
}

void DefaultHorizontalPanel::setMute(bool value)
{
    ui->volumeKnob->setMuted(!value);
}

void DefaultHorizontalPanel::setStereo(bool value)
{
    //ui->audioModeValue->setText(value ? QStringLiteral("STEREO") : QStringLiteral("MONO"));
    ui->audioModeValue->setEnabled(value);
}

void DefaultHorizontalPanel::setOutputPlugged(bool value)
{
    ui->earphonesStatus->setEnabled(value);
}

void DefaultHorizontalPanel::setCurrentChannel(int value)
{
    ui->channelValue->setNum(value);
}

void DefaultHorizontalPanel::setChannelCount(int /*value*/)
{

}

void DefaultHorizontalPanel::setScanning(bool value)
{
    ui->radioKnob->enableLabel(!value);
}

void DefaultHorizontalPanel::mousePressEvent(QMouseEvent * event)
{
    event->ignore();
}

void DefaultHorizontalPanel::mouseReleaseEvent(QMouseEvent * event)
{
    event->ignore();
}

void DefaultHorizontalPanel::mouseMoveEvent(QMouseEvent * event)
{
    event->ignore();
}

void DefaultHorizontalPanel::onControlButtonClick(int id)
{
    if (mControl)
        mControl->onSetFrequency(id);
}

QSize DefaultHorizontalPanel::sizeHint() const
{
    int width = 80 + mChannelButtonsWidth + 80;
    return {width, 80};
}

void DefaultHorizontalPanel::volumeUp()
{
    if (mControl) mControl->onVolumeUp();
}

void DefaultHorizontalPanel::volumeDown()
{
    if (mControl) mControl->onVolumeDown();
}

void DefaultHorizontalPanel::frequencyUp()
{
    if (mControl) mControl->onFrequencyUp();
}

void DefaultHorizontalPanel::frequencyDown()
{
    if (mControl) mControl->onFrequencyDown();
}

void DefaultHorizontalPanel::scanUp()
{
    if (mControl) mControl->onScanUp();
}

void DefaultHorizontalPanel::scanDown()
{
    if (mControl) mControl->onScanDown();
}

void DefaultHorizontalPanel::toggleMute()
{
    if (mControl) mControl->onToggleMute();
}

void DefaultHorizontalPanel::syncControlModelData(int row)
{
    switch (row) {
    case 0: // busy
        setBusy(mControlModel->data(mControlModel->index(row)).toBool());
        break;
    case 1: // volume
        setVolume(mControlModel->data(mControlModel->index(row)).toInt());
        break;
    case 2: // frequency
        setFrequency(mControlModel->data(mControlModel->index(row)).toInt());
        break;
    case 3: // cur ch
        setCurrentChannel(mControlModel->data(mControlModel->index(row)).toInt());
        break;
    case 5: // mute
        setMute(mControlModel->data(mControlModel->index(row)).toBool());
        break;
    case 6: // mono/stereo
        setStereo(mControlModel->data(mControlModel->index(row)).toBool());
        break;
    case 7: // earphones
        setOutputPlugged(mControlModel->data(mControlModel->index(row)).toBool());
        break;
    // ...
    }
}

void DefaultHorizontalPanel::syncPlayListModelData()
{
    updateChannelButtons();
}

void DefaultHorizontalPanel::setBusy(bool value)
{
    ui->radioKnob->setEnabled(!value);
}

void DefaultHorizontalPanel::setChannels(const QVector<QPair<int, QString> > & data)
{return;
    if (mCommandButtonGroup)
        delete mCommandButtonGroup;

    while (!ui->buttons->layout()->isEmpty())
        delete ui->buttons->layout()->takeAt(0);

    mCommandButtonGroup = new QButtonGroup(this);
    connect(mCommandButtonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &DefaultHorizontalPanel::onControlButtonClick);

    for (auto d: data) {
        QToolButton * b = createChannelButton(d.second, d.first);
        ui->buttons->layout()->addWidget(b);
        mCommandButtonGroup->addButton(b, d.first);
    }

//    QSpacerItem * spacer = new QSpacerItem(10, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    ui->buttons->layout()->addItem(spacer);

//    QToolButton * b = createChannelButton(QString(), 0);
//    b->setIcon(QIcon(QStringLiteral(":/rc/img/menu2gray-16x16.png")));
//    b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
//    ui->buttons->layout()->addWidget(b);

    mChannelButtonsWidth = ui->buttons->sizeHint().width();
    if (mChannelButtonsWidth < 250)
        mChannelButtonsWidth = 250;
}


QToolButton * DefaultHorizontalPanel::createChannelButton(const QString & text, int freq)
{
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QToolButton * btn1 = new QToolButton(this);
    btn1->setObjectName(QStringLiteral("btn1"));
    sizePolicy.setHeightForWidth(btn1->sizePolicy().hasHeightForWidth());
    btn1->setSizePolicy(sizePolicy);
    btn1->setFocusPolicy(Qt::NoFocus);
    btn1->setIconSize({16, 16});
    btn1->setText(text);
    btn1->setProperty("freq", QVariant(freq));///???
    btn1->setContextMenuPolicy(Qt::PreventContextMenu);

//    QFont font;
//    font.setBold(true);
//    font.setWeight(75);
//    btn1->setFont(font);

    return btn1;
}

void DefaultHorizontalPanel::createChannelButtons()
{
    if (mCommandButtonGroup)
        return;

    mCommandButtonGroup = new QButtonGroup(this);
    connect(mCommandButtonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &DefaultHorizontalPanel::onControlButtonClick);

    for (int i = 0; i < 5; ++i) {
        QToolButton * b = createChannelButton("", 0);
        b->setVisible(false);
        ui->buttons->layout()->addWidget(b);
        mCommandButtonGroup->addButton(b, i);
    }

    mChannelButtonsWidth = ui->buttons->sizeHint().width();
    if (mChannelButtonsWidth < 250)
        mChannelButtonsWidth = 250;

    ui->buttons->setFixedWidth(250);
}

void DefaultHorizontalPanel::updateChannelButtons()
{
    if (mPlayListModel && mCommandButtonGroup) {

        QList<QAbstractButton *> buttons = mCommandButtonGroup->buttons();
        foreach (QAbstractButton * btn, buttons)
            btn->setVisible(false);

        int n = qMin(mPlayListModel->rowCount(), buttons.size()); // max button count

        int i = 0;
        for (; i < n; ++i) {
            bool b = mPlayListModel->data(mPlayListModel->index(i, 0)).toBool(); // favourite?
            if (!b) break;
            QAbstractButton * btn = buttons[i];
            btn->setVisible(true);

            QString text = mPlayListModel->data(mPlayListModel->index(i, 2)).toString(); // channel name
            int freq = mPlayListModel->data(mPlayListModel->index(i, 1)).toInt();
            if (text.isEmpty())
                text = QString().sprintf("%.1f", freq/10.0f); // channel frequency
            btn->setText(text);

            mCommandButtonGroup->setId(btn, freq);
        }
    }
}

QString DefaultHorizontalPanel::channelButtonText(int id)
{
    QAbstractButton * btn = mCommandButtonGroup->button(id);
    return btn ? btn->text() : QString();
}

void DefaultHorizontalPanel::setStatusChannelName(int freq)
{
    int rows = mPlayListModel->rowCount();
    for (int i = 0; i < rows; ++i) {
        int ifreq = mPlayListModel->data(mPlayListModel->index(i, 1)).toInt();
        if (ifreq == freq) {
            QString text = mPlayListModel->data(mPlayListModel->index(i, 2)).toString(); // channel name
            ui->statusText->setText(text);
            return;
        }
    }
    QString text = QString().sprintf("%.1f MHz", freq/10.0f);
    ui->statusText->setText(text);
}












