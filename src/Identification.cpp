#include "Identification.h"

#include "PeFile.h"
#include "generics/iDir.h"
#include "headers/FileHdr.h"
#include "headers/OptionalHdr.h"

/* Non-member non-friend functions to check key information about a PE file */

using namespace Pepper;

bool Ident::isDll(const PeFile &pe)
{
    const FileHeader *pfh = (FileHeader*)pe.getHeaderPtr(PeFile::Headers::FILE);
    const uint16_t charact = *(uint16_t*)pfh->getFieldPtr(FileHeader::Fields::CHARACTERISTICS);
    return charact & FileHeader::Characteristics::DLL;
}

static uint16_t getOptionalHeaderMagic(const PeFile &pe)
{
    const OptionalHeader *poh = (OptionalHeader*)pe.getHeaderPtr(PeFile::Headers::OPTIONAL);
    return *(uint16_t*)poh->getFieldPtr(OptionalHeader::Fields::MAGIC);
}

bool Ident::is32bit(const PeFile &pe)
{
    return getOptionalHeaderMagic(pe) == OptionalHeader::Magic::BIT32;
}

bool Ident::is64bit(const PeFile &pe)
{
    return getOptionalHeaderMagic(pe) == OptionalHeader::Magic::BIT64;
}

bool Ident::isRom(const PeFile &pe)
{
    return getOptionalHeaderMagic(pe) == OptionalHeader::Magic::ROM;
}

bool Ident::dirHasData(const IDirectory &id)
{
    return id.hdrOffset() != 0; // TODO: not the most accurate way of checking for data
}

bool Ident::dirExists(const IDirectory &id)
{
    return id.owningSection() != nullptr;
}

bool Ident::isAllSigsValid(const PeFile &pe)
{
    // check DOS signature
    const DosHeader *dos = (DosHeader*)pe.getHeaderPtr(PeFile::DOS);
    const uint16_t dosMagic = *(uint16_t*)dos->getFieldPtr(DosHeader::E_MAGIC);
    if (dosMagic != 0x4D5A && dosMagic != 0x5A4D) {
        return false;
    }

    // check NT signature
    const FileHeader *file = (FileHeader*)pe.getHeaderPtr(PeFile::FILE);
    const int32_t ntSig = *(int32_t*)file->ntSig();
    if (ntSig != 0x00004550 && ntSig != 0x50450000) {
        return false;
    }

    return true;
}

static uint16_t getFileHeaderMachine(const PeFile &pe)
{
    const FileHeader *pfh = (FileHeader*)pe.getHeaderPtr(PeFile::Headers::FILE);
    return *(uint16_t*)pfh->getFieldPtr(FileHeader::Fields::MACHINE);
}

bool Ident::isX86(const PeFile &pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::I386
        || type == FileHeader::Machine::AMD64;
}

bool Ident::isArmThumb(const PeFile &pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::ARM
        || type == FileHeader::Machine::ARM64
        || type == FileHeader::Machine::ARMNT
        || type == FileHeader::Machine::THUMB;
}

bool Ident::isMips(const PeFile &pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::MIPS16
        || type == FileHeader::Machine::MIPSFPU
        || type == FileHeader::Machine::MIPSFPU16
        || type == FileHeader::Machine::R4000
        || type == FileHeader::Machine::WCEMIPSV2;
}

bool Ident::isRiscv(const PeFile &pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::RISCV32
        || type == FileHeader::Machine::RISCV64
        || type == FileHeader::Machine::RISCV128;
}

bool Ident::isThumb(const PeFile &pe)
{
    const uint16_t type = getFileHeaderMachine(pe);
    return type == FileHeader::Machine::THUMB;
}

