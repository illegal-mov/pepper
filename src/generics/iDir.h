#ifndef IDIRECTORY_H
#define IDIRECTORY_H

#include "iHdr.h"

namespace Pepper {

// foward declarations
class PeFile;
class DataDirectoryEntry;
class SectionHeaderEntry;

/* Directories are like headers except that they may not even exist,
 * and, if they do exist, their data may be merged into a section.
 * The effect of this is a directory's addresses may be offset by some
 * amount that can only be determined by examining the relevant section.
 */
class IDirectory : public IHeader {
public:
    /* get true offset into PE file content where directory is located */
    size_t dirOffset() const { return hdrOffset() - m_diffOfRvaRaw; }

    /* pointer to base of directory as char* only if the directory exists */
    const void* dir() const;

    /* get size of directory */
    uint32_t size() const;

    /* get a pointer to the section containing this directory's data */
    const SectionHeaderEntry* owningSection() const { return m_she; }

protected:
    IDirectory() = default;

    IDirectory(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    IDirectory(const IDirectory& id)
    : IHeader(id)
    , m_pe(id.m_pe)
    , m_dde(id.m_dde)
    , m_she(id.m_she)
    , m_diffOfRvaRaw(id.m_diffOfRvaRaw)
    {}

    IDirectory& operator=(const IDirectory& id)
    {
        IHeader::operator=(id);
        m_pe = id.m_pe;
        m_dde = id.m_dde;
        m_she = id.m_she;
        m_diffOfRvaRaw = id.m_diffOfRvaRaw;
        return *this;
    }

    virtual ~IDirectory()
    {
        m_pe = nullptr;
        m_dde = nullptr;
        m_she = nullptr;
        m_diffOfRvaRaw = 0;
    }

    const PeFile *m_pe{};
    const DataDirectoryEntry *m_dde{};
    const SectionHeaderEntry *m_she{};
    size_t m_diffOfRvaRaw{};
};
} // namespace Pepper

#endif
