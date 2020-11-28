#ifndef BOUNDIMPORT_H
#define BOUNDIMPORT_H

#include <generics/iDir.h>

/*
// one for each imported module
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    uint32_t ImportLookupTableRVA;  // pointer to array of thunk data
    int32_t  TimeDateStamp;         // dll timestamp after binding
    int32_t  ForwarderChain;        // index of first forwarder reference
    uint32_t NameRVA;               // pointer to module name
    uint32_t ImportAddressTableRVA; // pointer to import address table
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;

// one for each imported function per module
typedef struct _IMAGE_THUNK_DATA32 {
    union {
        struct {
            int32_t OrdinalNumber : 16;
            int32_t Null          : 15; // must be zero
            int32_t OrdinalFlag   : 1;
        };
        uint32_t HintNameTableRVA; // RVA to IMAGE_IMPORT_BY_NAME
    };
} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

// one for each imported function per module
typedef struct _IMAGE_THUNK_DATA64 {
    union {
        struct {
            int64_t OrdinalNumber : 16;
            int64_t Null          : 47; // must be zero
            int64_t OrdinalFlag   : 1;
        };
        uint64_t HintNameTableRVA; // RVA to IMAGE_IMPORT_BY_NAME
    };
} IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64;

typedef struct _IMAGE_IMPORT_BY_NAME {
    uint16_t Hint;   // Index into export name table pointer
    char     Name[]; // Imported function name as null-terminated ASCII string
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;
*/

namespace Pepper {

class PeFile;
class FileBytes;
class DataDirectoryEntry;

class BoundImportDir final : public IDirectory {
public:
    BoundImportDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

//  const IMAGE_BOUND_IMPORT_DIRECTORY* getStructPtr() const { return static_cast<const IMAGE_*>(dir()); }
    const void* getFieldPtr(const int index) const override;

    static const char* getFieldName(const int index);
};
} // namespace Pepper

#endif
