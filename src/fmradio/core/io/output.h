#ifndef OUTPUT_H
#define OUTPUT_H

#include <cstddef>

namespace fmradio {

class output_t
{
public:
    virtual ~output_t() = default;
    virtual int write(const char *, size_t) = 0;
};

} // namespace fmtuner

#endif // OUTPUT_H
