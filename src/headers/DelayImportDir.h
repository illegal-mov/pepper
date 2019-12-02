#ifndef DELAYIMPORT_H
#define DELAYIMPORT_H

#include <vector>

#include "../generics/iDir.h"
#include "ImportsCommon.h"
#include "struct.h"

/*
typedef struct _IMAGE_DELAY_IMPORT_DESCRIPTOR {
    int32_t Attributes;                 // reserved, must be zero
    int32_t NameRVA;                    // pointer to DLL's name
    int32_t ModuleHandleRVA;            // pointer to DLL's module handle
    int32_t DelayImportAddressTableRVA; // pointer to delay-load import address table
    int32_t DelayImportNameTableRVA;    // pointer to delay-load import name table
    int32_t BoundDelayImportTableRVA;   // pointer to bound delay-load address table
    int32_t UnloadDelayImportTableRVA;  // pointer to unload delay-load address table
    int32_t Timestamp;                  // dll timestamp after binding
} IMAGE_DELAY_IMPORT_DESCRIPTOR, *PIMAGE_DELAY_IMPORT_DESCRIPTOR;
*/

namespace Pepper {

// forward declarations
class DataDirectoryEntry;

typedef GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR> DelayImportDescriptor;

/* Variable-length array of DelayImportDescriptors.
 * length is equal to number of modules being imported from.
 */
class DelayImportDir final : public IDirectory {
private:
    std::vector<DelayImportDescriptor> m_descriptors{};
    static int s_diskToMemDiff;
public:
    DelayImportDir()
    : IDirectory()
    {}

    DelayImportDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* getFieldPtr(int index) const override;
    const std::vector<DelayImportDescriptor>& descriptors() const
    {
        return m_descriptors;
    }

    int length() const { return (int)m_descriptors.size(); }

    // static functions
    static const char* getFieldName(int index);

    // classes that need special access to s_diskToMemDiff
    friend class GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>;
};
} // namespace Pepper

#endif
