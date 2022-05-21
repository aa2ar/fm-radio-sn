#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstddef>

namespace fmradio {

class protocol_t
{
public:
    protocol_t();

    bool parse(const char *, size_t);
    const char * get();

private:
    enum { buffer_size = 1024 };
    char _buffer[buffer_size];
    size_t _offset = 0;
    char * _bop = nullptr; // beginning of packet
    char * _eop = nullptr; // end of packet
};

} // namespace fmtuner

#endif // PROTOCOL_H
