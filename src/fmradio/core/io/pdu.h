#ifndef PDU_H
#define PDU_H

#include  <cstddef>

namespace fmradio {

class pdu_t
{
public:
    virtual ~pdu_t() = default;
    const char * data() const { return _data; }
    size_t       size() const { return _size; }

protected:
    const char * _data = nullptr;
    size_t       _size = 0;
};

} // namespace fmtuner

#endif // PDU_H
