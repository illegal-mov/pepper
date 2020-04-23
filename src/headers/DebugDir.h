#ifndef DEBUG_H
#define DEBUG_H

#include <vector>

#include "../Types.h"
#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_DEBUG_DIRECTORY {
    int32_t  Characteristics;  // reserved, must be zero
    int32_t  TimeDateStamp;    // debug data creation time
    int16_t  MajorVersion;     // debug format major version number
    int16_t  MinorVersion;     // debug format minor version
    int32_t  Type;             // debug info format
    uint32_t SizeOfData;       // debug data size
    uint32_t AddressOfRawData; // virtual address to debug data
    uint32_t PointerToRawData; // file pointer to debug data
} IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

typedef struct _RSDSI {
    char    Signature[4]; // always set to "RSDS"
    char    Guid[16];     // some kind of unique ID
    int32_t Age;          // <unknown>
    char    Pdb[];        // path to PDB file
} RSDSI, *PRSDSI;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/* Exactly one per DebugEntry.
 * Pointed to by IMAGE_DEBUG_DIRECTORY.PointerToRawData
 */
class DebugRsds final : public IHeader {
public:
    enum Fields {
        SIGNATURE,
        GUID,
        AGE,
        PDB,
        _NUM_FIELDS,
    };

    DebugRsds() = default;

    DebugRsds(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    {}

    // member functions
    const void* getFieldPtr(const int index) const override;
    const RSDSI* getStructPtr() const { return static_cast<const RSDSI*>(hdr()); }

    // static functions
    static const char* getFieldName(const int index);
};

/* An IMAGE_DEBUG_DIRECTORY struct.
 * Contains a field for the file offset to an RSDSI struct.
 */
class DebugEntry final : public IHeader {
public:
    enum Fields {
        CHARACTERISTICS,
        TIMEDATESTAMP,
        MAJOR_VERSION,
        MINOR_VERSION,
        TYPE,
        SIZE_OF_DATA,
        ADDRESS_OF_RAW_DATA,
        POINTER_TO_RAW_DATA,
        _NUM_FIELDS,
    };

    enum Types {
        UNKNOWN        = 0,
        COFF           = 1,
        CODEVIEW       = 2,
        FPO            = 3,
        MISC           = 4,
        EXCEPTION      = 5,
        FIXUP          = 6,
        OMAP_TO_SRC    = 7,
        OMAP_FROM_SRC  = 8,
        BORLAND        = 9,
        RESERVED10     = 10,
        CLSID          = 11,
        REPRO          = 16,
        EX_DLL_CHARACT = 20,
    };

    DebugEntry(const FileBytes& fbytes, const offset_t raw);

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_DEBUG_DIRECTORY* getStructPtr() const { return static_cast<const IMAGE_DEBUG_DIRECTORY*>(hdr()); }
    const DebugRsds& rsds() const { return m_dbgRsds; }

    // static functions
    static const char* getFieldName(const int index);
    static const char* getDebugTypeName(const int index);

private:
    DebugRsds m_dbgRsds;
};

/* Array of IMAGE_DEBUG_DIRECTORY structs.
 * Array length is given by (totalDirectorySize / sizeof(debugStruct))
 */
class DebugDir final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    DebugDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    // member functions
    const std::vector<DebugEntry>& entries() const { return m_debugEntries; }
    const void* getFieldPtr(const int index) const override;

    // static functions
    static const char* getFieldName(const int index);

private:
    std::vector<DebugEntry> m_debugEntries{};
};
} // namespace Pepper

#endif
