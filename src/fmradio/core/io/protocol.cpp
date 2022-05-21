#include "protocol.h"
#include <cstring>

using namespace fmradio;

const char * EOP = "\r\n"; // end of packet
const size_t EOPLEN = strlen(EOP);

protocol_t::protocol_t()
{
    _bop = _buffer;
}

bool protocol_t::parse(const char * data, size_t size)
{
    if ( _offset + size < buffer_size) {
        strcpy(_buffer + _offset, data);
        _offset += size;
    }

    if (_eop == nullptr)
        _eop = strstr(_bop, EOP);

    return _eop;
}

const char * protocol_t::get()
{
    const char * p = nullptr;
    if (_eop) { // end of packet was found
        *_eop = '\0';
        p = _bop;
        _bop = _eop + EOPLEN;
        _eop = strstr(_bop, EOP);
    }
    else {
        size_t len = (_buffer + _offset) - _bop;
        memcpy(_buffer, _bop, len);
        _bop = _buffer;
        _offset = len;
        *(_buffer + _offset) = '\0';
    }
    return p;
}

