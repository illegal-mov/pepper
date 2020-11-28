#ifndef DATADIRECTORYENTRY_H
#define DATADIRECTORYENTRY_H

#include <Types.h>
#include <generics/iHdr.h>

/*
typedef struct _IMAGE_DATA_DIRECTORY {
    int32_t VirtualAddress; // address of table in memory
    int32_t Size;           // size of table
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;
*/

namespace Pepper {

/* DataDirectoryEntry is class wrapping an entry in the DataDirectory array.
 */
class DataDirectoryEntry final : public IHeader {
public:
    enum Fields {
        VIRTUAL_ADDRESS,
        SIZE,
        _NUM_FIELDS,
    };

    DataDirectoryEntry() = default;

    DataDirectoryEntry(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    {}

    const void* getFieldPtr(const int index) const override;
    const IMAGE_DATA_DIRECTORY* getStructPtr() const { return static_cast<const IMAGE_DATA_DIRECTORY*>(hdr()); }
    uint32_t rva() const { return getStructPtr()->VirtualAddress; }
    uint32_t size() const { return getStructPtr()->Size; }

    static const char* getFieldName(const int index);
};
} // namespace Pepper

#endif
