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
    SectionHeaders *sctns = (SectionHeaders*)pe.getHeaderPtr(PeFile::SECTION);
    for (const auto &section : sctns->sections()) {
        int32_t sctnBase = *(int32_t*)section.getFieldPtr(SectionHeaderEntry::VIRTUAL_ADDRESS);
        int32_t sctnSize = *(int32_t*)section.getFieldPtr(SectionHeaderEntry::VIRTUAL_SIZE);
        if (sctnBase <= dde.rva() && dde.rva() < sctnBase + sctnSize) {
            m_she = &section;
            m_diffOfRvaRaw = abs(sctnBase - *(int32_t*)section.getFieldPtr(SectionHeaderEntry::POINTER_TO_RAW_DATA));
            break;
        }
    }
}

/* pointer to base of directory as char* only if the directory exists */
const char* IDirectory::dir() const
{
    return (Ident::dirExists(*this))
        ? &m_headerPtr.get()[dirOffset()]
        : nullptr;
}

/* get size of directory */
int32_t IDirectory::size() const
{
    return m_dde->size();
}

