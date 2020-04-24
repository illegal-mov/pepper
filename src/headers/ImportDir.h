#ifndef IMPORT_H
#define IMPORT_H

#include <vector>

#include "../generics/iDir.h"
#include "ImportsCommon.h"

/*
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    uint32_t ImportLookupTableRVA;  // pointer to array of thunk data
    int32_t  TimeDateStamp;         // dll timestamp after binding
    int32_t  ForwarderChain;        // index of first forwarder reference
    uint32_t NameRVA;               // pointer to module name
    uint32_t ImportAddressTableRVA; // pointer to import address table
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;
*/

namespace Pepper {

// forward declarations
class DataDirectoryEntry;

using ImportDescriptor = GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>;

/* Variable-length array of ImportDescriptors.
 * length is equal to number of modules being imported from.
 */
class ImportDir final : public IDirectory {
public:
    ImportDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    // member functions
    const void* getFieldPtr(const int index) const override;
    const std::vector<ImportDescriptor>& descriptors() const
    {
        return m_descriptors;
    }

    size_t length() const { return m_descriptors.size(); }

    // static functions
    static const char* getFieldName(const int index);

    // classes that need special access to s_diskToMemDiff
    friend class GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>;
    friend class ImportThunk<IMAGE_THUNK_DATA32>;
    friend class ImportThunk<IMAGE_THUNK_DATA64>;

private:
    std::vector<ImportDescriptor> m_descriptors{};
    static size_t s_diskToMemDiff;
};
} // namespace Pepper

#endif
