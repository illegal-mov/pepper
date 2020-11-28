#include <Identification.h>

#include <cstring>

#include <PeFile.h>
#include <generics/iDir.h>
#include <pe_structures/FileHdr.h>
#include <pe_structures/OptionalHdr.h>

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
    const auto& dbg = pe.debugDir();
    if (Ident::dirExists(dbg.get())) {
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
    const auto& clr = pe.clrDir();
    if (Ident::dirExists(clr.get())) {
        const auto& meta = clr->metadataHdr();
        const int32_t bsjbSig = *static_cast<const int32_t*>(meta->getFieldPtr(ClrMetadata::SIGNATURE));
        if ((bsjbSig != 0x42534A42) && (bsjbSig != 0x424A5342)) { // "BSJB" "BJSB"
            return false;
        }
    }
    return true;
}
} // namespace

bool Ident::canAppendNewSection(const PeFile& pe)
{
    offset_t sectionHeadersEnd = pe.sectionHdrs().getSections().back().hdrOffset()
        + sizeof(IMAGE_SECTION_HEADER);
    constexpr char null[sizeof(IMAGE_SECTION_HEADER)] = {0};
    uint8_t section[sizeof(IMAGE_SECTION_HEADER)];
    pe.readBytes(sectionHeadersEnd, section, sizeof(section));
    return memcmp(section, null, sizeof(null)) == 0;
}

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

bool Ident::dirExists(const IDirectory* id)
{
    return id != nullptr && id->getOwningSection() != nullptr;
}

bool Ident::dirExists(const IDirectory& id)
{
    return id.getOwningSection() != nullptr;
}

bool Ident::isAllSignaturesValid(const PeFile& pe)
{
    return isDosSigValid(pe)
        && isNtSigValid(pe)
        && isDebugRsdsSigValid(pe)
        && isClrMetadataSigValid(pe);
}

bool Ident::isMachineTypeX86(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::I386
        || type == FileHeader::Machine::AMD64;
}

bool Ident::isMachineTypeArmThumb(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::ARM
        || type == FileHeader::Machine::ARM64
        || type == FileHeader::Machine::ARMNT
        || type == FileHeader::Machine::THUMB;
}

bool Ident::isMachineTypeMips(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::MIPS16
        || type == FileHeader::Machine::MIPSFPU
        || type == FileHeader::Machine::MIPSFPU16
        || type == FileHeader::Machine::R4000
        || type == FileHeader::Machine::WCEMIPSV2;
}

bool Ident::isMachineTypeRiscv(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::RISCV32
        || type == FileHeader::Machine::RISCV64
        || type == FileHeader::Machine::RISCV128;
}

bool Ident::isMachineTypeThumb(const PeFile& pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::THUMB;
}

