#ifndef CLR_H
#define CLR_H

#include "../generics/iDir.h"
#include "./clr/CodeManager.h"
#include "./clr/ExportJump.h"
#include "./clr/ManagedNative.h"
#include "./clr/Metadata.h"
#include "./clr/Resource.h"
#include "./clr/Signature.h"
#include "./clr/VTableFixup.h"
#include "DataDirectoryEntry.h"
#include "struct.h"

/*
typedef struct _IMAGE_COR20_HEADER {
    int32_t              Size;                // size of this header
    int16_t              MajorRuntimeVersion; // CLR major version number
    int16_t              MinorRuntimeVersion; // CLR minor version number
    IMAGE_DATA_DIRECTORY MetaData;
    int32_t              Flags;
    union {
        int32_t          EntryPointToken;
        int32_t          EntryPointRVA;
    };
    IMAGE_DATA_DIRECTORY Resources;
    IMAGE_DATA_DIRECTORY StrongNameSignature;
    IMAGE_DATA_DIRECTORY CodeManagerTable;
    IMAGE_DATA_DIRECTORY VTableFixups;
    IMAGE_DATA_DIRECTORY ExportAddressTableJumps;
    IMAGE_DATA_DIRECTORY ManagedNativeHeader;
} IMAGE_COR20_HEADER, *PIMAGE_COR20_HEADER;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;

class ClrDir final : public IDirectory {
public:
    enum Headers {
        CLR_METADATA,
        CLR_RESOURCES,
        CLR_SIGNATURE,
        CLR_CODE_MANAGER,
        CLR_VTABLE_FIXUPS,
        CLR_EXPORT_JUMP,
        CLR_NATIVE_HEADER,
        _NUM_HEADERS,
    };
private:
    std::array<DataDirectoryEntry, _NUM_HEADERS> m_directoryEntries{};
    ClrMetadata     m_Meta;
    ClrResource     m_Rsrc;
    ClrSignature    m_Signtr;
    ClrCodeManager  m_CodeMan;
    ClrVTableFixup  m_VTabFix;
    ClrExportJump   m_XprtJmp;
    ClrNativeHeader m_NatHead;
public:
    // commented out elements exist in the structure, but
    // should be accessed by iterating over `directories()`
    enum Fields {
        SIZE,
        MAJOR_RUNTIME_VERSION,
        MINOR_RUNTIME_VERSION,
//      METADATA_FIELD,
        FLAGS,
        ENTRY_POINT_TOKEN,
        ENTRY_POINT_RVA = ENTRY_POINT_TOKEN,
//      RESOURCES_FIELD,
//      STRONG_NAME_SIGNATURE,
//      CODE_MANAGER_TABLE,
//      VTABLE_FIXUPS,
//      EXPORT_ADDRESS_TABLE_JUMPS,
//      MANAGED_NATIVE_HEADER,
        _NUM_FIELDS,
    };

    ClrDir()
    : IDirectory()
    , m_Meta()
    , m_Rsrc()
    , m_Signtr()
    , m_CodeMan()
    , m_VTabFix()
    , m_XprtJmp()
    , m_NatHead()
    {}

    ClrDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const std::array<DataDirectoryEntry, _NUM_HEADERS>& directories() const
    {
        return m_directoryEntries;
    }

    const IMAGE_COR20_HEADER* clr() const { return (PIMAGE_COR20_HEADER)dir(); }
    const void* getHeaderPtr(int index) const;
    const void* getFieldPtr(int index) const override;

    // static functions
    static const char* getHeaderName(int index);
    static const char* getFieldName(int index);
};
} // namespace Pepper

#endif
