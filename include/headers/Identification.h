#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H

#include <memory>

/* Non-member non-friend functions to check key information about a PE file */

namespace Pepper {
    class PeFile;
    class IDirectory;
}

namespace Pepper
{
namespace Ident
{
    bool canAppendNewSection(const PeFile& pe);

    bool isDll(const PeFile& pe);

    bool is32bit(const PeFile& pe);

    bool is64bit(const PeFile& pe);

    bool isRom(const PeFile& pe);

    bool dirExists(const IDirectory* id);
    bool dirExists(const IDirectory& id);

    bool isAllSignaturesValid(const PeFile& pe);

    bool isMachineTypeX86(const PeFile& pe);

    bool isMachineTypeArmThumb(const PeFile& pe);

    bool isMachineTypeMips(const PeFile& pe);

    bool isMachineTypeRiscv(const PeFile& pe);

    bool isMachineTypeThumb(const PeFile& pe);
} // namespace Ident
} // namespace Pepper

#endif
