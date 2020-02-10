#ifndef CLR_RESOURCE_H
#define CLR_RESOURCE_H

#include "../../generics/iDir.h"

/*
typedef struct _IMAGE_COR20_RESOURCES_HEADER {
    int32_t Unknown1; // TODO: is this total size - 4?
    int32_t Magic;    // always set to 0xBEEFCACE
    int32_t Unknown2; // TODO: is this a count of something?
    int32_t Size;
} IMAGE_COR20_RESOURCES_HEADER, *PIMAGE_COR20_RESOURCES_HEADER;

typedef struct _IMAGE_COR20_RESOURCES_STRING {
    uint8_t Length;
    char    String[]; // flexible member because it is length-prefixed
} IMAGE_COR20_RESOURCES_STRING, *PIMAGE_COR20_RESOURCES_STRING;

typedef struct _IMAGE_COR20_RESOURCES_WSTRING {
    uint8_t  Length;
    uint16_t String[]; // flexible member because it is length-prefixed
} IMAGE_COR20_RESOURCES_WSTRING, *PIMAGE_COR20_RESOURCES_WSTRING;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/* Exactly one COR20_RESOURCES_HEADER followed by some RESOURCES_W?STRING and other data
 */
class ClrResource final : public IDirectory {
private:
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ClrResource() = default;

    ClrResource(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* rsrc() const { return static_cast<const void*>(dir()); }
    const void* getFieldPtr(const int index) const override;

    // static functions
    static const char* getFieldName(const int index);
};
}

#endif
