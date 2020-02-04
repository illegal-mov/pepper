#ifndef IMPORT_H
#define IMPORT_H

#include <vector>

#include "../generics/iDir.h"
#include "ImportsCommon.h"
#include "struct.h"

/*
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    int32_t ImportLookupTableRVA;  // pointer to array of thunk data
    int32_t TimeDateStamp;         // dll timestamp after binding
    int32_t ForwarderChain;        // index of first forwarder reference
    int32_t NameRVA;               // pointer to module name
    int32_t ImportAddressTableRVA; // pointer to import address table
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
private:
    std::vector<ImportDescriptor> m_descriptors{};
    static size_t s_diskToMemDiff;
public:
    ImportDir()
    : IDirectory()
    {}

    ImportDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

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
};
} // namespace Pepper

#endif
