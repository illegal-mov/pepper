#ifndef IDIRECTORY_H
#define IDIRECTORY_H

#include "iHdr.h"

namespace Pepper {

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
    size_t dirOffset() const { return hdrOffset() - m_diskToMemoryDifference; }

    /* pointer to base of directory only if the directory exists */
    const void* dir() const;

    /* get size of directory */
    uint32_t size() const;

    /* get a pointer to the section containing this directory's data */
    const SectionHeaderEntry* getOwningSection() const { return m_sectionHeaderEntry; }

protected:
    IDirectory() = default;

    IDirectory(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    IDirectory(const IDirectory& id)
    : IHeader(id)
    , m_peFile(id.m_peFile)
    , m_dataDirectoryEntry(id.m_dataDirectoryEntry)
    , m_sectionHeaderEntry(id.m_sectionHeaderEntry)
    , m_diskToMemoryDifference(id.m_diskToMemoryDifference)
    {}

    IDirectory& operator=(const IDirectory& id)
    {
        IHeader::operator=(id);
        m_peFile = id.m_peFile;
        m_dataDirectoryEntry = id.m_dataDirectoryEntry;
        m_sectionHeaderEntry = id.m_sectionHeaderEntry;
        m_diskToMemoryDifference = id.m_diskToMemoryDifference;
        return *this;
    }

    virtual ~IDirectory()
    {
        m_peFile = nullptr;
        m_dataDirectoryEntry = nullptr;
        m_sectionHeaderEntry = nullptr;
        m_diskToMemoryDifference = 0;
    }

    const PeFile *m_peFile{};
    const DataDirectoryEntry *m_dataDirectoryEntry{};
    const SectionHeaderEntry *m_sectionHeaderEntry{};
    size_t m_diskToMemoryDifference{};
};
} // namespace Pepper

#endif
