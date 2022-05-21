/**
 * @file   uicontrol.h
 * @author Alexander Arefyev
 * @date   21.07.2018
 *
 *
 */

#ifndef UICONTROL
#define UICONTROL

class UiControl
{
public:
    virtual ~UiControl() = default;

    virtual int  volume()    const = 0;
    virtual int  frequency() const = 0;
    virtual bool muted()     const = 0;

    virtual void onVolumeUp()       = 0;
    virtual void onVolumeDown()     = 0;
    virtual void onFrequencyUp()    = 0;
    virtual void onFrequencyDown()  = 0;
    virtual void onSetFrequency(int)= 0;
    virtual void onScanUp()         = 0;
    virtual void onScanDown()       = 0;
    virtual void onSetMuted(bool)   = 0; // depricated
    virtual void onToggleMute()     = 0;
};

#endif // UICONTROL

