#ifndef __IDS__H__INCLUDED__
#define __IDS__H__INCLUDED__

namespace fmradio {
namespace ids {

enum {
    error_id            = (1 << 1),
    volume_id           = (1 << 2),
    frequency_id        = (1 << 3),
    pause_id            = (1 << 4),
    campus_on_id        = (1 << 5),
    campus_off_id       = (1 << 6),
    channel_id          = (1 << 7),
    scan_id             = (1 << 8),
    scan_ok_id          = (1 << 9),
    control_play_id     = (1 << 10),
    control_pause_id    = (1 << 11),
    earphones_in_id     = (1 << 12),
    earphones_out_id    = (1 << 13),
    audio_mode_id       = (1 << 14),
    version_id          = (1 << 15),
    ret_id              = (1 << 16),
};

}}

#endif // __IDS__H__INCLUDED__
