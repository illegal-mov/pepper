#ifndef CONVERSION_H
#define CONVERSION_H

#include <cstdint>
#include <cstdlib>

#include "Types.h"

/* Non-member non-friend functions to addresses from one type to another */

// forward declarations
namespace Pepper {
    class PeFile;
}

namespace Pepper { namespace Convert {
    enum class AddrType {
        AVA, // Absolute Virtual Address
        RVA, // Relative Virtual Address
        RAW, // Raw Address (File Offset)
    };

    /* Use an RVA address to get the difference between RVAs and RAWs */
    uint32_t getRvaToRawDiff(const PeFile& pe, const addr_t rva);

    /* Use a RAW address to get the difference between RVAs and RAWs */
    uint32_t getRawToRvaDiff(const PeFile& pe, const addr_t raw);

    /* Convert an {AVA,RVA,RAW} to an {AVA,RVA,RAW} */
    addr_t convertAddr(const PeFile& pe, const addr_t addr, AddrType src, AddrType dst);
}}

#endif
