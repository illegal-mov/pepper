#ifndef DATADIRECTORYENTRY_H
#define DATADIRECTORYENTRY_H

#include "../generics/iHdr.h"
#include "struct.h"

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

    DataDirectoryEntry()
    : IHeader()
    {}

    DataDirectoryEntry(const FileBytes &fbytes, const size_t raw)
    : IHeader(fbytes, raw)
    {}

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_DATA_DIRECTORY* entry() const { return (IMAGE_DATA_DIRECTORY*)hdr(); }
    uint32_t rva() const { return *(uint32_t*)getFieldPtr(VIRTUAL_ADDRESS); }
    uint32_t size() const { return *(uint32_t*)getFieldPtr(SIZE); }

    // static functions
    static const char* getFieldName(const int index);
};
} // namespace Pepper

#endif
