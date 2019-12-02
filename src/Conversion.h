#ifndef CONVERSION_H
#define CONVERSION_H

#include <cstdint>
#include <cstdlib>

/* Non-member non-friend functions to addresses from one type to another */

// forward declarations
namespace Pepper {
    class PeFile;
}

namespace Pepper { namespace Convert {
    enum AddrType {
        AVA, // Absolute Virtual Address
        RVA, // Relative Virtual Address
        RAW, // Raw Address (File Offset)
    };

    /* Use an RVA address to get the difference between RVAs and RAWs */
    int32_t getRvaToRawDiff(const PeFile &pe, int64_t rva);

    /* Use a RAW address to get the difference between RVAs and RAWs */
    int32_t getRawToRvaDiff(const PeFile &pe, int64_t raw);

    /* Convert an {AVA,RVA,RAW} to an {AVA,RVA,RAW} */
    int64_t convertAddr(const PeFile &pe, int64_t addr, AddrType src, AddrType dst);
}}

#endif
