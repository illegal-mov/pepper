#include "Identification.h"

#include "PeFile.h"
#include "generics/iDir.h"
#include "headers/FileHdr.h"
#include "headers/OptionalHdr.h"

/* Non-member non-friend functions to check key information about a PE file */

using namespace Pepper;

namespace
{
uint16_t getOptionalHeaderMagic(const PeFile& pe)
{
    const OptionalHeader& poh = pe.optionalHdr();
    return poh.getStructPtr32()->Magic;
}

uint16_t getFileHeaderMachine(const PeFile& pe)
{
    const FileHeader& pfh = pe.fileHdr();
    return pfh.getStructPtr()->Machine;
}

bool isDosSigValid(const PeFile& pe)
{
    const DosHeader& dos = pe.dosHdr();
    const uint16_t dosMagic = *static_cast<const uint16_t*>(dos.getFieldPtr(DosHeader::E_MAGIC));
    if ((dosMagic != 0x4D5A) && (dosMagic != 0x5A4D)) {
        return false;
    }
    return true;
}

bool isNtSigValid(const PeFile& pe)
{
    const FileHeader& file = pe.fileHdr();
    const int32_t ntSig = *reinterpret_cast<const int32_t*>(file.ntSig());
    if ((ntSig != 0x00004550) && (ntSig != 0x50450000)) {
        return false;
    }
    return true;
}

bool isDebugRsdsSigValid(const PeFile& pe)
{
    const std::shared_ptr<DebugDir> dbg = pe.debugDir();
    if (Ident::dirExists(dbg)) {
        for (const auto& entry : dbg->entries()) {
            const int32_t rsdsSig = *static_cast<const int32_t*>(entry.rsds().getFieldPtr(DebugRsds::SIGNATURE));
            if ((rsdsSig != 0x52534453) && (rsdsSig != 0x53445352)) { // "RSDS" "SDSR"
                return false;
            }
        }
    }
    return true;
}

bool isClrMetadataSigValid(const PeFile& pe)
{
    const std::shared_ptr<ClrDir> clr = pe.clrDir();
    if (Ident::dirExists(clr)) {
        const std::shared_ptr<ClrMetadata> meta = clr->metadataHdr();
        const int32_t bsjbSig = *static_cast<const int32_t*>(meta->getFieldPtr(ClrMetadata::SIGNATURE));
        if ((bsjbSig != 0x42534A42) && (bsjbSig != 0x424A5342)) { // "BSJB" "BJSB"
            return false;
        }
    }
    return true;
}
} // namespace

bool Ident::isDll(const PeFile& pe)
{
    const FileHeader& pfh = pe.fileHdr();
    const uint16_t charact = pfh.getStructPtr()->Characteristics;
    return charact & FileHeader::Characteristics::DLL;
}

bool Ident::is32bit(const PeFile& pe)
{
    return getOptionalHeaderMagic(pe) == OptionalHeader::Magic::BIT32;
}

bool Ident::is64bit(const PeFile& pe)
{
    return getOptionalHeaderMagic(pe) == OptionalHeader::Magic::BIT64;
}

bool Ident::isRom(const PeFile& pe)
{
    return getOptionalHeaderMagic(pe) == OptionalHeader::Magic::ROM;
}

bool Ident::dirExists(const std::shared_ptr<IDirectory>& id)
{
    return id != nullptr;
}

bool Ident::dirExists(const IDirectory& id)
{
    return id.getOwningSection() != nullptr;
}

bool Ident::isAllSigsValid(const PeFile& pe)
{
    return isDosSigValid(pe)
        && isNtSigValid(pe)
        && isDebugRsdsSigValid(pe)
        && isClrMetadataSigValid(pe);
}

bool Ident::isX86(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::I386
        || type == FileHeader::Machine::AMD64;
}

bool Ident::isArmThumb(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::ARM
        || type == FileHeader::Machine::ARM64
        || type == FileHeader::Machine::ARMNT
        || type == FileHeader::Machine::THUMB;
}

bool Ident::isMips(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::MIPS16
        || type == FileHeader::Machine::MIPSFPU
        || type == FileHeader::Machine::MIPSFPU16
        || type == FileHeader::Machine::R4000
        || type == FileHeader::Machine::WCEMIPSV2;
}

bool Ident::isRiscv(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::RISCV32
        || type == FileHeader::Machine::RISCV64
        || type == FileHeader::Machine::RISCV128;
}

bool Ident::isThumb(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::THUMB;
}

