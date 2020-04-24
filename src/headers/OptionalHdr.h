#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <array>

#include "../Types.h"
#include "../generics/iHdr.h"
#include "DataDirectoryEntry.h"
#include "FileHdr.h"

/*
typedef struct _IMAGE_OPTIONAL_HEADER32 {
    int16_t Magic;                                                    // identifies file as 32bit, 64bit, or ROM
    int8_t  MajorLinkerVersion;                                       // linker major version number
    int8_t  MinorLinkerVersion;                                       // linker minor version number
    int32_t SizeOfCode;                                               // sum of all code section sizes
    int32_t SizeOfInitializedData;                                    // sum of all initialized data section sizes
    int32_t SizeOfUninitializedData;                                  // sum of all uninitialized data section sizes
    int32_t AddressOfEntryPoint;                                      // entry point address relative to image base
    int32_t BaseOfCode;                                               // start of code section relative to image base
    int32_t BaseOfData;                                               // start of data section relative to image base
    int32_t ImageBase;                                                // preferred base address of binary (must be % 65536 == 0)
    int32_t SectionAlignment;                                         // byte alignment for section data in memory (must be >= FileAlignment)
    int32_t FileAlignment;                                            // byte alignment for section data on disk
    int16_t MajorOperatingSystemVersion;                              // operating system major version number
    int16_t MinorOperatingSystemVersion;                              // operating system minor version number
    int16_t MajorImageVersion;                                        // image major version number
    int16_t MinorImageVersion;                                        // image minor version number
    int16_t MajorSubsystemVersion;                                    // subsystem major version number
    int16_t MinorSubsystemVersion;                                    // subsystem minor version number
    int32_t Win32VersionValue;                                        // reserved, must be zero
    int32_t SizeOfImage;                                              // size of image, including headers, in bytes (must be % SectionAlignment == 0)
    int32_t SizeOfHeaders;                                            // sum of all header sizes
    int32_t CheckSum;                                                 // file checksum
    int16_t Subsystem;                                                // required subsystem
    int16_t DllCharacteristics;                                       // bit fields for various properties
    int32_t SizeOfStackReserve;                                       // stack size to reserve
    int32_t SizeOfStackCommit;                                        // stack size to commit
    int32_t SizeOfHeapReserve;                                        // heap size to reserve
    int32_t SizeOfHeapCommit;                                         // heap size to commit
    int32_t LoaderFlags;                                              // reserved, must be zero
    int32_t NumberOfRvaAndSizes;                                      // number of non-null DataDirectory entries
    IMAGE_DATA_DIRECTORY DataDirectory[__PEPPER_PASTE(DATA_DIR_LEN)]; // addresses and sizes of extra tables
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
    int16_t Magic;                                                    // identifies file as 32bit, 64bit, or ROM
    int8_t  MajorLinkerVersion;                                       // linker major version number
    int8_t  MinorLinkerVersion;                                       // linker minor version number
    int32_t SizeOfCode;                                               // sum of all code section sizes
    int32_t SizeOfInitializedData;                                    // sum of all initialized data section sizes
    int32_t SizeOfUninitializedData;                                  // sum of all uninitialized data section sizes
    int32_t AddressOfEntryPoint;                                      // entry point address relative to image base
    int32_t BaseOfCode;                                               // start of code section relative to image base
    int64_t ImageBase;                                                // preferred base address of binary (must be % 65536 == 0)
    int32_t SectionAlignment;                                         // byte alignment for section data in memory (must be >= FileAlignment)
    int32_t FileAlignment;                                            // byte alignment for section data on disk
    int16_t MajorOperatingSystemVersion;                              // operating system major version number
    int16_t MinorOperatingSystemVersion;                              // operating system minor version number
    int16_t MajorImageVersion;                                        // image major version number
    int16_t MinorImageVersion;                                        // image minor version number
    int16_t MajorSubsystemVersion;                                    // subsystem major version number
    int16_t MinorSubsystemVersion;                                    // subsystem minor version number
    int32_t Win32VersionValue;                                        // reserved, must be zero
    int32_t SizeOfImage;                                              // size of image, including headers, in bytes (must be % SectionAlignment == 0)
    int32_t SizeOfHeaders;                                            // sum of all header sizes
    int32_t CheckSum;                                                 // file checksum
    int16_t Subsystem;                                                // required subsystem
    int16_t DllCharacteristics;                                       // bit fields for various properties
    int64_t SizeOfStackReserve;                                       // stack size to reserve
    int64_t SizeOfStackCommit;                                        // stack size to commit
    int64_t SizeOfHeapReserve;                                        // heap size to reserve
    int64_t SizeOfHeapCommit;                                         // heap size to commit
    int32_t LoaderFlags;                                              // reserved, must be zero
    int32_t NumberOfRvaAndSizes;                                      // number of non-null DataDirectory entries
    IMAGE_DATA_DIRECTORY DataDirectory[__PEPPER_PASTE(DATA_DIR_LEN)]; // addresses and sizes of extra tables
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;
*/

namespace Pepper {

/* Optional header exists immediately after the File header.
 * Optional header has a 32bit form and a 64bit form determined by
 * OptionalHeader.Magic (0x10B for 32bit, 0x20B for 64bit)
 */
class OptionalHeader final : public IHeader {
public:
    enum Fields {
        MAGIC,
        MAJOR_LINKER_VERSION,
        MINOR_LINKER_VERSION,
        SIZE_OF_CODE,
        SIZE_OF_INITIALIZED_DATA,
        SIZE_OF_UNINITIALIZED_DATA,
        ADDRESS_OF_ENTRY_POINT,
        BASE_OF_CODE,
        BASE_OF_DATA,
        IMAGE_BASE,
        SECTION_ALIGNMENT,
        FILE_ALIGNMENT,
        MAJOR_OPERATING_SYSTEM_VERSION,
        MINOR_OPERATING_SYSTEM_VERSION,
        MAJOR_IMAGE_VERSION,
        MINOR_IMAGE_VERSION,
        MAJOR_SUBSYSTEM_VERSION,
        MINOR_SUBSYSTEM_VERSION,
        WIN32_VERSION_VALUE,
        SIZE_OF_IMAGE,
        SIZE_OF_HEADERS,
        CHECKSUM,
        SUBSYSTEM,
        DLL_CHARACTERISTICS,
        SIZE_OF_STACK_RESERVE,
        SIZE_OF_STACK_COMMIT,
        SIZE_OF_HEAP_RESERVE,
        SIZE_OF_HEAP_COMMIT,
        LOADER_FLAGS,
        NUMBER_OF_RVA_AND_SIZES,
        DATA_DIRECTORY,
        _NUM_FIELDS,
    };

    enum Magic {
        ROM   = 0x107,
        BIT32 = 0x10B,
        BIT64 = 0x20B,
    };

    enum Characteristics {
        RESERVED_1            = 0x0001,
        RESERVED_2            = 0x0002,
        RESERVED_4            = 0x0004,
        RESERVED_8            = 0x0008,
        RESERVED_10           = 0x0010,
        HIGH_ENTROPY_VA       = 0x0020,
        DYNAMIC_BASE          = 0x0040,
        FORCE_INTEGRITY       = 0x0080,
        NX_COMPAT             = 0x0100,
        NO_ISOLATION          = 0x0200,
        NO_SEH                = 0x0400,
        NO_BIND               = 0x0800,
        APPCONTAINER          = 0x1000,
        WDM_DRIVER            = 0x2000,
        GUARD_CF              = 0x4000,
        TERMINAL_SERVER_AWARE = 0x8000,
    };

    OptionalHeader(const FileBytes& fbytes, const FileHeader& file);

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_OPTIONAL_HEADER32* getStructPtr32() const { return static_cast<const IMAGE_OPTIONAL_HEADER32*>(hdr()); }
    const IMAGE_OPTIONAL_HEADER64* getStructPtr64() const { return static_cast<const IMAGE_OPTIONAL_HEADER64*>(hdr()); }
    // the `ImageBase` is important for address conversions,
    // so this gets to be its own special little function
    addr_t imageBase() const;

    // static functions
    static const char* getFieldName(const int index);
    static const char* getCharacteristicName(const int index);
};

/* DataDirectory is an array of structures built in to the OptionalHeader.
 */
class DataDirectory final {
public:
    enum Entries {
        XPRT,
        MPRT,
        RSRC,
        XCPT,
        CERT,
        RLOC,
        DBG,
        ARCH,   // unused, all zero
        GLBPTR,
        TLS,
        LDCFG,
        BMPRT,
        IAT,
        DMPRT,
        CLR,
        RSRV,  // null terminator
        _NUM_ENTRIES,
    };

    DataDirectory(const FileBytes& fbytes, const OptionalHeader& opt);

    // member functions
    const std::array<DataDirectoryEntry, _NUM_ENTRIES>& directories() const
    {
        return m_directoryEntries;
    }

    const void* getFieldPtr(const int index) const;
    size_t length() const { return _NUM_ENTRIES; };

    // static functions
    static const char* getFieldName(const int index);
    static const char* getDirectoryEntryName(const int index) { return getFieldName(index); }

private:
    std::array<DataDirectoryEntry, _NUM_ENTRIES> m_directoryEntries{};
};
} // namespace Pepper

#endif
