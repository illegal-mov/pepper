#include "../Identification.h"
#include "../PeFile.h"
#include "../headers/DataDirectoryEntry.h"
#include "../headers/SectionHdr.h"
#include "iDir.h"

using namespace Pepper;

IDirectory::IDirectory(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IHeader(fbytes, dde.rva())
, m_pe(&pe)
, m_dde(&dde)
, m_she(nullptr)
, m_diffOfRvaRaw(0)
{
    // linear search for containing section
    const SectionHeaders *sctns = static_cast<const SectionHeaders*>(pe.getHeaderPtr(PeFile::SECTION));
    for (const auto &section : sctns->sections()) {
        const uint32_t sctnBase = section.entry()->VirtualAddress;
        const uint32_t sctnSize = section.entry()->VirtualSize;
        const uint32_t sctnRaw  = section.entry()->PointerToRawData;
        if (sctnBase <= dde.rva() && dde.rva() < sctnBase + sctnSize) {
            m_she = &section;
            m_diffOfRvaRaw = (sctnBase > sctnRaw) ? sctnBase - sctnRaw : sctnRaw - sctnBase;
            break;
        }
    }
}

/* pointer to base of directory as char* only if the directory exists */
const void* IDirectory::dir() const
{
    return (Ident::dirExists(*this))
        ? &m_headerPtr[dirOffset()]
        : nullptr;
}

/* get size of directory */
uint32_t IDirectory::size() const
{
    return m_dde->size();
}

