#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H

#include <memory>

/* Non-member non-friend functions to check key information about a PE file */

// forward declarations
namespace Pepper {
    class PeFile;
    class IDirectory;
}

namespace Pepper { namespace Ident {
    /* return true if and only if the file is a DLL */
    bool isDll(const PeFile& pe);

    /* return true if and only if the file is a 32bit binary */
    bool is32bit(const PeFile& pe);

    /* return true if and only if the file is a 64bit binary */
    bool is64bit(const PeFile& pe);

    /* return true if and only if the file is a ROM image */
    bool isRom(const PeFile& pe);

    /* return true if and only if the directory exists and has data */
    bool dirIsValid(const std::shared_ptr<IDirectory>& id);

    /* return true if and only if the directory has data */
    bool dirHasData(const IDirectory& id);

    /* return true if and only if the directory exists */
    bool dirExists(const IDirectory& id);

    /* return true if and only if DOS magic and NT signature are correct */
    bool isAllSigsValid(const PeFile& pe);

    /* return true if and only if the machine type is x86 or x86_64 */
    bool isX86(const PeFile& pe);

    /* return true if and only if the machine type is ARM or Thumb */
    bool isArmThumb(const PeFile& pe);

    /* return true if and only if the machine type is MIPS */
    bool isMips(const PeFile& pe);

    /* return true if and only if the machine type is RISC-V */
    bool isRiscv(const PeFile& pe);

    /* return true if and only if the machine type is Thumb */
    bool isThumb(const PeFile& pe);
}}

#endif
