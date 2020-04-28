#ifndef CONVERSION_H
#define CONVERSION_H

#include <cstdint>
#include <cstdlib>

#include "Types.h"

/* Non-member non-friend functions to addresses from one type to another */

namespace Pepper {
    class PeFile;
}

namespace Pepper { namespace Convert {
    enum class AddrType {
        AVA, // Absolute Virtual Address
        RVA, // Relative Virtual Address
        RAW, // Raw Address (File Offset)
    };

    uint32_t getRvaToRawDiff(const PeFile& pe, const addr_t rva);

    uint32_t getRawToRvaDiff(const PeFile& pe, const addr_t raw);

    addr_t convertAddr(const PeFile& pe, const addr_t addr, AddrType src, AddrType dst);
}}

#endif
