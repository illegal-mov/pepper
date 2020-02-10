#ifndef STRUCT_H
#define STRUCT_H

/* The `Pepper` namespace is used for C++, while
 *  the `PEPPER_` identifier prefix is used for C.
 * These macros exist to concatenate the `PEPPER_` prefix.
 */
#define __PEPPER_PASTE3(prefix, token) prefix##token
#define __PEPPER_PASTE2(prefix, token) __PEPPER_PASTE3(prefix, token)
#define __PEPPER_PASTE(token) __PEPPER_PASTE2(__PEPPER_PREFIX, token)

#ifdef __cplusplus
    #include <cstdint>
    namespace Pepper {
    #define __PEPPER_PREFIX

    static constexpr int DATA_DIR_LEN = 16;
    static constexpr int NAME_LEN_SECTION = 8;
    static constexpr int SIG_LEN_DOS = 2;
    static constexpr int SIG_LEN_NT = 4;
#else
    #include <stdint.h>
    #define __PEPPER_PREFIX PEPPER_

    #define PEPPER_SC_ALIGN_NYBBLE 0x00F00000
    #define PEPPER_DATA_DIR_LEN 16
    #define PEPPER_NAME_SECTION_MAX_LEN 8
    #define PEPPER_SIG_LEN_DOS 2
    #define PEPPER_SIG_LEN_NT 4
#endif

// These enums are defined in their respective classes in C++
#ifndef __cplusplus

enum file_hdr_mach_type {
    PE_FM_UNKNOWN   = 0x0000,
    PE_FM_AM33      = 0x01d3,
    PE_FM_AMD64     = 0x8664,
    PE_FM_ARM       = 0x01c0,
    PE_FM_ARM64     = 0xaa64,
    PE_FM_ARMNT     = 0x01c4,
    PE_FM_EBC       = 0x0ebc,
    PE_FM_I386      = 0x014c,
    PE_FM_IA64      = 0x0200,
    PE_FM_M32R      = 0x9041,
    PE_FM_MIPS16    = 0x0266,
    PE_FM_MIPSFPU   = 0x0366,
    PE_FM_MIPSFPU16 = 0x0466,
    PE_FM_POWERPC   = 0x01f0,
    PE_FM_POWERPCFP = 0x01f1,
    PE_FM_R4000     = 0x0166,
    PE_FM_RISCV32   = 0x5032,
    PE_FM_RISCV64   = 0x5064,
    PE_FM_RISCV128  = 0x5128,
    PE_FM_SH3       = 0x01a2,
    PE_FM_SH3DSP    = 0x01a3,
    PE_FM_SH4       = 0x01a6,
    PE_FM_SH5       = 0x01a8,
    PE_FM_THUMB     = 0x01c2,
    PE_FM_WCEMIPSV2 = 0x0169,
};

enum file_hdr_charact {
    PE_FC_RELOCS_STRIPPED         = 0x0001,
    PE_FC_EXECUTABLE_IMAGE        = 0x0002,
    PE_FC_LINE_NUMS_STRIPPED      = 0x0004,
    PE_FC_LOCAL_SYMS_STRIPPED     = 0x0008,
    PE_FC_AGGRESIVE_WS_TRIM       = 0x0010, // deprecated since Windows 2000
    PE_FC_LARGE_ADDRESS_AWARE     = 0x0020,
    PE_FC_RESERVED_40             = 0x0040,
    PE_FC_BYTES_REVERSED_LO       = 0x0080,
    PE_FC_32BIT_MACHINE           = 0x0100,
    PE_FC_DEBUG_STRIPPED          = 0x0200,
    PE_FC_REMOVABLE_RUN_FROM_SWAP = 0x0400,
    PE_FC_NET_RUN_FROM_SWAP       = 0x0800,
    PE_FC_SYSTEM                  = 0x1000,
    PE_FC_DLL                     = 0x2000,
    PE_FC_UP_SYSTEM_ONLY          = 0x4000,
    PE_FC_BYTES_REVERSED_HI       = 0x8000,
};

enum opt_hdr_magic {
    PE_OM_ROM = 0x107,
    PE_OM_32  = 0x10B,
    PE_OM_64  = 0x20B,
};

enum opt_hdr_win_subsys {
    PE_WS_UNKNOWN                  = 0,
    PE_WS_NATIVE                   = 1,
    PE_WS_WINDOWS_GUI              = 2,
    PE_WS_WINDOWS_CUI              = 3,
    PE_WS_OS2_CUI                  = 5,
    PE_WS_POSIX_CUI                = 7,
    PE_WS_WINDOWS_CE_GUI           = 9,
    PE_WS_EFI_APPLICATION          = 10,
    PE_WS_EFI_BOOT_SERVICE_DRIVER  = 11,
    PE_WS_EFI_RUNTIME_DRIVER       = 12,
    PE_WS_EFI_ROM                  = 13,
    PE_WS_XBOX                     = 14,
    PE_WS_WINDOWS_BOOT_APPLICATION = 16,
};

enum opt_hdr_dll_charact {
    PE_DC_RESERVED_1            = 0x0001,
    PE_DC_RESERVED_2            = 0x0002,
    PE_DC_RESERVED_4            = 0x0004,
    PE_DC_RESERVED_8            = 0x0008,
    PE_DC_RESERVED_10           = 0x0010,
    PE_DC_HIGH_ENTROPY_VA       = 0x0020,
    PE_DC_DYNAMIC_BASE          = 0x0040,
    PE_DC_FORCE_INTEGRITY       = 0x0080,
    PE_DC_NX_COMPAT             = 0x0100,
    PE_DC_NO_ISOLATION          = 0x0200,
    PE_DC_NO_SEH                = 0x0400,
    PE_DC_NO_BIND               = 0x0800,
    PE_DC_APPCONTAINER          = 0x1000,
    PE_DC_WDM_DRIVER            = 0x2000,
    PE_DC_GUARD_CF              = 0x4000,
    PE_DC_TERMINAL_SERVER_AWARE = 0x8000,
};

enum opt_hdr_data_dir {
    PE_DD_EXPORT,
    PE_DD_IMPORT,
    PE_DD_RESOURCE,
    PE_DD_EXCEPTION,
    PE_DD_CERTIFICATE,
    PE_DD_RELOCATION,
    PE_DD_DEBUG,
    PE_DD_ARCHITECTURE, // reserved, must be zero
    PE_DD_GLOBALPTR,
    PE_DD_TLS,
    PE_DD_LOAD_CONFIG,
    PE_DD_BOUND_IMPORT,
    PE_DD_IAT,
    PE_DD_DELAY_IMPORT,
    PE_DD_CLR,
    PE_DD_NULL,         // reserved, must be zero
};

enum section_hdr_charact {
    PE_SC_RESERVED_1      = 0x00000001,
    PE_SC_RESERVED_2      = 0x00000002,
    PE_SC_RESERVED_4      = 0x00000004,
    PE_SC_TYPE_NO_PAD     = 0x00000008,
    PE_SC_RESERVED_10     = 0x00000010,
    PE_SC_CNT_CODE        = 0x00000020,
    PE_SC_CNT_INIT        = 0x00000040,
    PE_SC_CNT_UNINIT      = 0x00000080,
    PE_SC_RESERVED_100    = 0x00000100,
    PE_SC_LNK_INFO        = 0x00000200,
    PE_SC_RESERVED_400    = 0x00000400,
    PE_SC_LNK_REMOVE      = 0x00000800,
    PE_SC_LNK_COMDAT      = 0x00001000,
    PE_SC_RESERVED_2000   = 0x00002000,
    PE_SC_RESERVED_4000   = 0x00004000,
    PE_SC_GPREL           = 0x00008000,
    PE_SC_MEM_PURGEABLE   = 0x00010000,
    PE_SC_MEM_16BIT       = 0x00020000,
    PE_SC_MEM_LOCKED      = 0x00040000,
    PE_SC_MEM_PRELOAD     = 0x00080000,
    PE_SC_ALIGN_1BYTES    = 0x00100000,
    PE_SC_ALIGN_2BYTES    = 0x00200000,
    PE_SC_ALIGN_4BYTES    = 0x00300000,
    PE_SC_ALIGN_8BYTES    = 0x00400000,
    PE_SC_ALIGN_16BYTES   = 0x00500000,
    PE_SC_ALIGN_32BYTES   = 0x00600000,
    PE_SC_ALIGN_64BYTES   = 0x00700000,
    PE_SC_ALIGN_128BYTES  = 0x00800000,
    PE_SC_ALIGN_256BYTES  = 0x00900000,
    PE_SC_ALIGN_512BYTES  = 0x00A00000,
    PE_SC_ALIGN_1024BYTES = 0x00B00000,
    PE_SC_ALIGN_2048BYTES = 0x00C00000,
    PE_SC_ALIGN_4096BYTES = 0x00D00000,
    PE_SC_ALIGN_8192BYTES = 0x00E00000,
    PE_SC_LNK_NRELOC_OVFL = 0x01000000,
    PE_SC_MEM_DISCARDABLE = 0x02000000,
    PE_SC_MEM_NOT_CACHED  = 0x04000000,
    PE_SC_MEM_NOT_PAGED   = 0x08000000,
    PE_SC_MEM_SHARED      = 0x10000000,
    PE_SC_MEM_EXECUTE     = 0x20000000,
    PE_SC_MEM_READ        = 0x40000000,
    PE_SC_MEM_WRITE       = 0x80000000,
};

enum res_dir_res_types {
    PE_RT_CURSOR,
    PE_RT_BITMAP,
    PE_RT_ICON,
    PE_RT_MENU,
    PE_RT_DIALOG,
    PE_RT_STRING,
    PE_RT_FONTDIR,
    PE_RT_FONT,
    PE_RT_ACCELERATOR,
    PE_RT_RCDATA,
    PE_RT_MESSAGETABLE,
};

enum cert_dir_cert_ver {
    PE_CV1 = 0x100,
    PE_CV2 = 0x200,
};

enum cert_dir_cert_types {
    PE_CT_X509=1,
    PE_CT_PKCS_SIGNED_DATA,
    PE_CT_RESERVED_1,
    PE_CT_TS_STACK_SIGNED,
};

enum reloc_dir_reloc_types {
    PE_RT_ABSOLUTE       = 0x0,
    PE_RT_HIGH           = 0x1,
    PE_RT_LOW            = 0x2,
    PE_RT_HIGHLOW        = 0x3,
    PE_RT_HIGHADJ        = 0x4,
    PE_RT_MIPS_JMPADDR   = 0x5,
    PE_RT_ARM_MOV32      = 0x5,
    PE_RT_RISCV_HIGH20   = 0x5,
    PE_RT_THUMB_MOV32    = 0x7,
    PE_RT_RISCV_LOW12I   = 0x7,
    PE_RT_RISCV_LOW12S   = 0x8,
    PE_RT_MIPS_JMPADDR16 = 0x9,
    PE_RT_DIR64          = 0xA,
};

enum dbg_dir_dbg_types {
    PE_DT_UNKNOWN               = 0,
    PE_DT_COFF                  = 1,
    PE_DT_CODEVIEW              = 2,
    PE_DT_FPO                   = 3,
    PE_DT_MISC                  = 4,
    PE_DT_EXCEPTION             = 5,
    PE_DT_FIXUP                 = 6,
    PE_DT_OMAP_TO_SRC           = 7,
    PE_DT_OMAP_FROM_SRC         = 8,
    PE_DT_BORLAND               = 9,
    PE_DT_RESERVED10            = 10,
    PE_DT_CLSID                 = 11,
    PE_DT_REPRO                 = 16,
    PE_DT_EX_DLLCHARACTERISTICS = 20,
};

#endif

typedef struct _IMAGE_DOS_HEADER {
    char     e_magic[__PEPPER_PASTE(SIG_LEN_DOS)]; // signature
    int16_t  e_cblp;     // count of bytes on last page
    int16_t  e_cp;       // count of pages
    int16_t  e_crlc;     // count of relocations
    int16_t  e_cparhdr;  // count of paragraph headers
    int16_t  e_minalloc; // minimum allocations
    int16_t  e_maxalloc; // maximum allocations
    int16_t  e_ss;       // initial SS
    int16_t  e_sp;       // initial SP
    int16_t  e_csum;     // checksum
    int16_t  e_ip;       // initial IP
    int16_t  e_cs;       // initial CS
    int16_t  e_lfarlc;   // offset to relocations
    int16_t  e_ovno;     // overlay number
    int16_t  e_res[4];   // reserved1
    int16_t  e_oemid;    // oem Identifier
    int16_t  e_oeminfo;  // oem Info
    int16_t  e_res2[10]; // reserved2
    uint16_t e_lfanew;   // file offset to NT header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
    uint16_t Machine;              // type of cpu the binary was compiled for
    uint16_t NumberOfSections;     // number of section headers
    int32_t  TimeDateStamp;        // file creation time as UNIX epoch
    uint32_t PointerToSymbolTable; // file offset to symbol table (deprecated)
    uint32_t NumberOfSymbols;      // number of symbol table entries (deprecated)
    uint16_t SizeOfOptionalHeader; // size of optional header in bytes
    uint16_t Characteristics;      // bitfields for various things
    /* 0x0001 IMAGE_FILE_RELOCS_STRIPPED
     * 0x0002 IMAGE_FILE_EXECUTABLE_IMAGE
     * 0x0004 IMAGE_FILE_LINE_NUMS_STRIPPED
     * 0x0008 IMAGE_FILE_LOCAL_SYMS_STRIPPED
     * 0x0010 IMAGE_FILE_AGGRESIVE_WS_TRIM
     * 0x0020 IMAGE_FILE_LARGE_ADDRESS_AWARE
     * 0x0040 IMAGE_FILE_RESERVED_40
     * 0x0080 IMAGE_FILE_BYTES_REVERSED_LO
     * 0x0100 IMAGE_FILE_32BIT_MACHINE
     * 0x0200 IMAGE_FILE_DEBUG_STRIPPED
     * 0x0400 IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP
     * 0x0800 IMAGE_FILE_NET_RUN_FROM_SWAP
     * 0x1000 IMAGE_FILE_SYSTEM
     * 0x2000 IMAGE_FILE_IS_DLL
     * 0x4000 IMAGE_FILE_UP_SYSTEM_ONLY
     */
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
    uint32_t VirtualAddress;                 // address of table in memory
    uint32_t Size;                           // size of table
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER32 {
    uint16_t Magic;                                                    // identifies file as 32bit, 64bit, or ROM
    /* 0x10b IMAGE_NT_OPTIONAL_HDR32_MAGIC
     * 0x20b IMAGE_NT_OPTIONAL_HDR64_MAGIC
     * 0x107 IMAGE_ROM_OPTIONAL_HDR_MAGIC
     */
    int8_t   MajorLinkerVersion;                                       // linker major version number
    int8_t   MinorLinkerVersion;                                       // linker minor version number
    uint32_t SizeOfCode;                                               // sum of all code section sizes
    uint32_t SizeOfInitializedData;                                    // sum of all initialized data section sizes
    uint32_t SizeOfUninitializedData;                                  // sum of all uninitialized data section sizes
    uint32_t AddressOfEntryPoint;                                      // entry point address relative to image base
    uint32_t BaseOfCode;                                               // start of code section relative to image base
    uint32_t BaseOfData;                                               // start of data section relative to image base
    uint32_t ImageBase;                                                // preferred base address of binary (must be % 65536 == 0)
    uint32_t SectionAlignment;                                         // byte alignment for section data in memory (must be >= FileAlignment)
    uint32_t FileAlignment;                                            // byte alignment for section data on disk
    int16_t  MajorOperatingSystemVersion;                              // operating system major version number
    int16_t  MinorOperatingSystemVersion;                              // operating system minor version number
    int16_t  MajorImageVersion;                                        // image major version number
    int16_t  MinorImageVersion;                                        // image minor version number
    int16_t  MajorSubsystemVersion;                                    // subsystem major version number
    int16_t  MinorSubsystemVersion;                                    // subsystem minor version number
    int32_t  Win32VersionValue;                                        // reserved, must be zero
    uint32_t SizeOfImage;                                              // size of image, including headers, in bytes (must be % SectionAlignment == 0)
    uint32_t SizeOfHeaders;                                            // sum of all header sizes
    uint32_t CheckSum;                                                 // file checksum
    int16_t  Subsystem;                                                // required subsystem
    /* 0  IMAGE_SUBSYSTEM_UNKNOWN
     * 1  IMAGE_SUBSYSTEM_NATIVE
     * 2  IMAGE_SUBSYSTEM_WINDOWS_GUI
     * 3  IMAGE_SUBSYSTEM_WINDOWS_CUI
     * 5  IMAGE_SUBSYSTEM_OS2_CUI
     * 7  IMAGE_SUBSYSTEM_POSIX_CUI
     * 9  IMAGE_SUBSYSTEM_WINDOWS_CE_GUI
     * 10 IMAGE_SUBSYSTEM_EFI_APPLICATION
     * 11 IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER
     * 12 IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER
     * 13 IMAGE_SUBSYSTEM_EFI_ROM
     * 14 IMAGE_SUBSYSTEM_XBOX
     * 16 IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION
     */
    uint16_t DllCharacteristics;                                       // bit fields for various properties
    /* 0x0001 IMAGE_DLLCHARACTERISTICS_RESERVED_1
     * 0x0002 IMAGE_DLLCHARACTERISTICS_RESERVED_2
     * 0x0004 IMAGE_DLLCHARACTERISTICS_RESERVED_4
     * 0x0008 IMAGE_DLLCHARACTERISTICS_RESERVED_8
     * 0x0010 IMAGE_DLLCHARACTERISTICS_RESERVED_10
     * 0x0020 IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA
     * 0x0040 IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE
     * 0x0080 IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY
     * 0x0100 IMAGE_DLLCHARACTERISTICS_NX_COMPAT
     * 0x0200 IMAGE_DLLCHARACTERISTICS_NO_ISOLATION
     * 0x0400 IMAGE_DLLCHARACTERISTICS_NO_SEH
     * 0x0800 IMAGE_DLLCHARACTERISTICS_NO_BIND
     * 0x1000 IMAGE_DLLCHARACTERISTICS_APPCONTAINER
     * 0x2000 IMAGE_DLLCHARACTERISTICS_WDM_DRIVER
     * 0x4000 IMAGE_DLLCHARACTERISTICS_GUARD_CF
     * 0x8000 IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE
     */
    uint32_t SizeOfStackReserve;                                       // stack size to reserve
    uint32_t SizeOfStackCommit;                                        // stack size to commit
    uint32_t SizeOfHeapReserve;                                        // heap size to reserve
    uint32_t SizeOfHeapCommit;                                         // heap size to commit
    int32_t  LoaderFlags;                                              // reserved, must be zero
    uint32_t NumberOfRvaAndSizes;                                      // number of non-null DataDirectory entries
    IMAGE_DATA_DIRECTORY DataDirectory[__PEPPER_PASTE(DATA_DIR_LEN)]; // addresses and sizes of extra tables
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
    uint16_t Magic;                                                    // identifies file as 32bit, 64bit, or ROM
    /* 0x10b IMAGE_NT_OPTIONAL_HDR32_MAGIC
     * 0x20b IMAGE_NT_OPTIONAL_HDR64_MAGIC
     * 0x107 IMAGE_ROM_OPTIONAL_HDR_MAGIC
     */
    int8_t   MajorLinkerVersion;                                       // linker major version number
    int8_t   MinorLinkerVersion;                                       // linker minor version number
    uint32_t SizeOfCode;                                               // sum of all code section sizes
    uint32_t SizeOfInitializedData;                                    // sum of all initialized data section sizes
    uint32_t SizeOfUninitializedData;                                  // sum of all uninitialized data section sizes
    uint32_t AddressOfEntryPoint;                                      // entry point address relative to image base
    uint32_t BaseOfCode;                                               // start of code section relative to image base
    uint64_t ImageBase;                                                // preferred base address of binary (must be % 65536 == 0)
    uint32_t SectionAlignment;                                         // byte alignment for section data in memory (must be >= FileAlignment)
    uint32_t FileAlignment;                                            // byte alignment for section data on disk
    int16_t  MajorOperatingSystemVersion;                              // operating system major version number
    int16_t  MinorOperatingSystemVersion;                              // operating system minor version number
    int16_t  MajorImageVersion;                                        // image major version number
    int16_t  MinorImageVersion;                                        // image minor version number
    int16_t  MajorSubsystemVersion;                                    // subsystem major version number
    int16_t  MinorSubsystemVersion;                                    // subsystem minor version number
    int32_t  Win32VersionValue;                                        // reserved, must be zero
    uint32_t SizeOfImage;                                              // size of image, including headers, in bytes (must be % SectionAlignment == 0)
    uint32_t SizeOfHeaders;                                            // sum of all header sizes
    uint32_t CheckSum;                                                 // file checksum
    int16_t  Subsystem;                                                // required subsystem
    /* 0  IMAGE_SUBSYSTEM_UNKNOWN
     * 1  IMAGE_SUBSYSTEM_NATIVE
     * 2  IMAGE_SUBSYSTEM_WINDOWS_GUI
     * 3  IMAGE_SUBSYSTEM_WINDOWS_CUI
     * 5  IMAGE_SUBSYSTEM_OS2_CUI
     * 7  IMAGE_SUBSYSTEM_POSIX_CUI
     * 9  IMAGE_SUBSYSTEM_WINDOWS_CE_GUI
     * 10 IMAGE_SUBSYSTEM_EFI_APPLICATION
     * 11 IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER
     * 12 IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER
     * 13 IMAGE_SUBSYSTEM_EFI_ROM
     * 14 IMAGE_SUBSYSTEM_XBOX
     * 16 IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION
     */
    uint16_t DllCharacteristics;                                       // bit fields for various properties
    /* 0x0001 IMAGE_DLLCHARACTERISTICS_RESERVED_1
     * 0x0002 IMAGE_DLLCHARACTERISTICS_RESERVED_2
     * 0x0004 IMAGE_DLLCHARACTERISTICS_RESERVED_4
     * 0x0008 IMAGE_DLLCHARACTERISTICS_RESERVED_8
     * 0x0010 IMAGE_DLLCHARACTERISTICS_RESERVED_10
     * 0x0020 IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA
     * 0x0040 IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE
     * 0x0080 IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY
     * 0x0100 IMAGE_DLLCHARACTERISTICS_NX_COMPAT
     * 0x0200 IMAGE_DLLCHARACTERISTICS_NO_ISOLATION
     * 0x0400 IMAGE_DLLCHARACTERISTICS_NO_SEH
     * 0x0800 IMAGE_DLLCHARACTERISTICS_NO_BIND
     * 0x1000 IMAGE_DLLCHARACTERISTICS_APPCONTAINER
     * 0x2000 IMAGE_DLLCHARACTERISTICS_WDM_DRIVER
     * 0x4000 IMAGE_DLLCHARACTERISTICS_GUARD_CF
     * 0x8000 IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE
     */
    uint64_t SizeOfStackReserve;                                       // stack size to reserve
    uint64_t SizeOfStackCommit;                                        // stack size to commit
    uint64_t SizeOfHeapReserve;                                        // heap size to reserve
    uint64_t SizeOfHeapCommit;                                         // heap size to commit
    int32_t  LoaderFlags;                                              // reserved, must be zero
    uint32_t NumberOfRvaAndSizes;                                      // number of non-null DataDirectory entries
    IMAGE_DATA_DIRECTORY DataDirectory[__PEPPER_PASTE(DATA_DIR_LEN)]; // addresses and sizes of extra tables
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_NT_HEADERS {
    char   Signature[__PEPPER_PASTE(SIG_LEN_NT)];
    IMAGE_FILE_HEADER File;
    union {
        IMAGE_OPTIONAL_HEADER32 Opt32;
        IMAGE_OPTIONAL_HEADER64 Opt64;
    };
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

typedef struct _IMAGE_SECTION_HEADER {
    char     Name[__PEPPER_PASTE(NAME_LEN_SECTION)]; // section name as ASCII string
    uint32_t VirtualSize;                            // section size in memory
    uint32_t VirtualAddress;                         // address of section in memory
    uint32_t SizeOfRawData;                          // section size on disk (must be % FileAlignment == 0)
    uint32_t PointerToRawData;                       // offset of section on disk (must be % FileAlignment == 0)
    uint32_t PointerToRelocations;                   // offset to relocations on disk
    uint32_t PointerToLineNumbers;                   // offset to line numbers on disk
    uint16_t NumberOfRelocations;                    // number of relocation entries
    uint16_t NumberOfLineNumbers;                    // number of line number entries
    uint32_t Characteristics;                        // bit fields for various properties
    /* 0x00000008 IMAGE_SCN_TYPE_NO_PAD
     * 0x00000020 IMAGE_SCN_CNT_CODE
     * 0x00000040 IMAGE_SCN_CNT_INITIALIZED_DATA
     * 0x00000080 IMAGE_SCN_CNT_UNINITIALIZED_DATA
     * 0x00000100 IMAGE_SCN_LNK_OTHER
     * 0x00000200 IMAGE_SCN_LNK_INFO
     * 0x00000800 IMAGE_SCN_LNK_REMOVE
     * 0x00001000 IMAGE_SCN_LNK_COMDAT
     * 0x00004000 IMAGE_SCN_NO_DEFER_SPEC_EXC
     * 0x00008000 IMAGE_SCN_GPREL
     * 0x00020000 IMAGE_SCN_MEM_PURGEABLE
     * 0x00040000 IMAGE_SCN_MEM_LOCKED
     * 0x00080000 IMAGE_SCN_MEM_PRELOAD
     * 0x00100000 IMAGE_SCN_ALIGN_1BYTES
     * 0x00200000 IMAGE_SCN_ALIGN_2BYTES
     * 0x00300000 IMAGE_SCN_ALIGN_4BYTES
     * 0x00400000 IMAGE_SCN_ALIGN_8BYTES
     * 0x00500000 IMAGE_SCN_ALIGN_16BYTES
     * 0x00600000 IMAGE_SCN_ALIGN_32BYTES
     * 0x00700000 IMAGE_SCN_ALIGN_64BYTES
     * 0x00800000 IMAGE_SCN_ALIGN_128BYTES
     * 0x00900000 IMAGE_SCN_ALIGN_256BYTES
     * 0x00A00000 IMAGE_SCN_ALIGN_512BYTES
     * 0x00B00000 IMAGE_SCN_ALIGN_1024BYTES
     * 0x00C00000 IMAGE_SCN_ALIGN_2048BYTES
     * 0x00D00000 IMAGE_SCN_ALIGN_4096BYTES
     * 0x00E00000 IMAGE_SCN_ALIGN_8192BYTES
     * 0x01000000 IMAGE_SCN_LNK_NRELOC_OVFL
     * 0x02000000 IMAGE_SCN_MEM_DISCARDABLE
     * 0x04000000 IMAGE_SCN_MEM_NOT_CACHED
     * 0x08000000 IMAGE_SCN_MEM_NOT_PAGED
     * 0x10000000 IMAGE_SCN_MEM_SHARED
     * 0x20000000 IMAGE_SCN_MEM_EXECUTE
     * 0x40000000 IMAGE_SCN_MEM_READ
     * 0x80000000 IMAGE_SCN_MEM_WRITE
     */
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

/* EXPORT DIRECTORY */

typedef struct _IMAGE_EXPORT_DIRECTORY {
    int32_t  ExportFlags;           // reserved, must be zero
    int32_t  TimeDateStamp;         // export data creation time
    int16_t  MajorVersion;          // major version number
    int16_t  MinorVersion;          // minor version number
    uint32_t NameRVA;               // pointer to dll name
    uint32_t OrdinalBase;           // ordinal starting index
    uint32_t AddressTableEntries;   // number of export table entries
    uint32_t NumberOfNamePointers;  // number of ordinal table entries
    uint32_t ExportAddressTableRVA; // pointer to export table
    uint32_t NamePointerRVA;        // pointer to name table
    uint32_t OrdinalTableRVA;       // pointer to ordinal table
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

/* IMPORT DIRECTORY */

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    uint32_t ImportLookupTableRVA;  // pointer to array of thunk data
    int32_t  TimeDateStamp;         // dll timestamp after binding
    int32_t  ForwarderChain;        // index of first forwarder reference
    uint32_t NameRVA;               // pointer to module name
    uint32_t ImportAddressTableRVA; // pointer to import address table
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;

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

/* RESOURCE DIRECTORY */

typedef struct _IMAGE_RESOURCE_DIRECTORY {
    int32_t  Characteristics;      // reserved, must be zero
    int32_t  TimeDateStamp;        // resource data creation time
    int16_t  MajorVersion;
    int16_t  MinorVersion;
    uint16_t NumberOfNamedEntries;
    uint16_t NumberOfIdEntries;
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

typedef struct _IMAGE_RESOURCE_ENTRY {
    union { // string or ID
        struct {
            uint32_t NameOffset   : 31;
            int32_t  NameIsString : 1;
        };
        int32_t Name;
        int16_t ID; // TODO: this ID identifies resource type only at root level
    };
    union { // dir ptr or data ptr
        uint32_t OffsetToData;
        struct {
            uint32_t OffsetToDirectory : 31;
            int32_t  DataIsDirectory   : 1;
        };
    };
} IMAGE_RESOURCE_ENTRY, *PIMAGE_RESOURCE_ENTRY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING {
	uint16_t Length;
	char     NameString[];
} IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;

typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
	uint16_t Length;
	uint16_t NameString[]; // TODO: maybe char16_t
} IMAGE_RESOURCE_DIRECTORY_STRING_U, *PIMAGE_RESOURCE_DIRECTORY_STRING_U;

typedef struct _IMAGE_RESOURCE_DATA {
    uint32_t OffsetToData;
    uint32_t Size;
    int32_t  CodePage;
    int32_t  Reserved; // reserved, must be zero
} IMAGE_RESOURCE_DATA, *PIMAGE_RESOURCE_DATA;

/* EXCEPTION DIRECTORY */

typedef struct _IMAGE_EXCEPTION_ENTRY32 {
    uint32_t BeginAddress;     // AVA to start of exception-handling code
    uint32_t EndAddress;       // AVA to end of exception-handling code
    uint32_t ExceptionHandler; // RVA to exception handler to execute
    uint32_t HandlerData;      // RVA to extra exception handler data
    uint32_t PrologEndAddress; // AVA to end of function prolog
} IMAGE_EXCEPTION_ENTRY32, *PIMAGE_EXCEPTION_ENTRY32;

typedef struct _IMAGE_EXCEPTION_ENTRY64 {
    uint32_t BeginAddress;      // RVA to start of exception-handling code
    uint32_t EndAddress;        // RVA to end of exception-handling code
    uint32_t UnwindInformation; // RVA to unwind information
} IMAGE_EXCEPTION_ENTRY64, *PIMAGE_EXCEPTION_ENTRY64;

typedef struct _IMAGE_EXCEPTION_ENTRY_ARM {
    uint32_t BeginAddress;        // AVA to start of exception-handling code
    uint32_t PrologLength   : 8;  // Number of instructions in prolog
    uint32_t FunctionLength : 22; // number of instructions in function
    int32_t  Is32BitCode    : 1;  // (==1) ? 32bit insns : 16bit insn
    int32_t  ExceptionFlag  : 1;  // (==1) ? handler exists : no handler
} IMAGE_EXCEPTION_ENTRY_ARM, *PIMAGE_EXCEPTION_ENTRY_ARM;

/* CERTIFICATE / SECURITY DIRECTORY */

typedef struct _IMAGE_ATTRIBUTE_CERTIFICATE {
    uint32_t Length;           // certificate size in bytes
    int16_t  Revision;         // certificate version
    /* 0x0100 WIN_CERT_REVISION_1_0
     * 0x0200 WIN_CERT_REVISION_2_0
     */
    int16_t CertificateType;  // certificate type
    /* 0x0001 WIN_CERT_TYPE_X509
     * 0x0002 WIN_CERT_TYPE_PKCS_SIGNED_DATA
     * 0x0003 WIN_CERT_TYPE_RESERVED_1
     * 0x0004 WIN_CERT_TYPE_TS_STACK_SIGNED
     */
    char    CertificateBytes[]; // certificate data
} IMAGE_ATTRIBUTE_CERTIFICATE, *PIMAGE_ATTRIBUTE_CERTIFICATE;

/* RELOCATIONS DIRECTORY */

typedef struct _IMAGE_BASE_RELOCATION {
    uint32_t PageRVA;
    uint32_t BlockSize;
} IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

typedef struct _IMAGE_BASE_RELOCATION_ENTRY {
    uint16_t Offset : 12;
    uint16_t Type   : 4;
    /* 0x0 IMAGE_REL_BASED_ABSOLUTE
     * 0x1 IMAGE_REL_BASED_HIGH
     * 0x2 IMAGE_REL_BASED_LOW
     * 0x3 IMAGE_REL_BASED_HIGHLOW
     * 0x4 IMAGE_REL_BASED_HIGHADJ
     * 0x5 IMAGE_REL_BASED_MIPS_JMPADDR
     * 0x5 IMAGE_REL_BASED_ARM_MOV32
     * 0x5 IMAGE_REL_BASED_RISCV_HIGH20
     * 0x6 <Reserved>                     // unused
     * 0x7 IMAGE_REL_BASED_THUMB_MOV20
     * 0x7 IMAGE_REL_BASED_RISCV_LOW12I
     * 0x8 IMAGE_REL_BASED_RISCV_LOW12S
     * 0x9 IMAGE_REL_BASED_MIPS_JMPADDR16
     * 0xA IMAGE_REL_BASED_DIR64
     */
} IMAGE_BASE_RELOCATION_ENTRY, *PIMAGE_BASE_RELOCATION_ENTRY;

/* DEBUG DIRECTORY */

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

/* ARCHITECTURE DIRECTORY */

// unused

/* GLOBAL POINTER DIRECTORY */

//

/* TLS DIRECTORY */

typedef struct _IMAGE_TLS_DIRECTORY32 {
    int32_t  RawDataStartVA;
    int32_t  RawDataEndVA;
    uint32_t AddressOfIndex;
    uint32_t AddressOfCallbacks;
    uint32_t SizeOfZeroFill;
    int32_t  Characteristics;
} IMAGE_TLS_DIRECTORY32, *PIMAGE_TLS_DIRECTORY32;

typedef struct _IMAGE_TLS_DIRECTORY64 {
    int64_t  RawDataStartVA;
    int64_t  RawDataEndVA;
    uint64_t AddressOfIndex;
    uint64_t AddressOfCallbacks;
    uint32_t SizeOfZeroFill;
    int32_t  Characteristics;
} IMAGE_TLS_DIRECTORY64, *PIMAGE_TLS_DIRECTORY64;

/* LOAD CONFIG DIRECTORY */

typedef struct _IMAGE_LOAD_CONFIG_CODE_INTEGRITY {
    int16_t  Flags;
    int16_t  Catalog;
    uint32_t CatalogOffset;
    int32_t  Reserved;
} IMAGE_LOAD_CONFIG_CODE_INTEGRITY, *PIMAGE_LOAD_CONFIG_CODE_INTEGRITY;

typedef struct _IMAGE_LOAD_CONFIG_DIRECTORY32 {
    uint32_t                         Size;
    int32_t                          TimeDateStamp;
    int16_t                          MajorVersion;
    int16_t                          MinorVersion;
    int32_t                          GlobalFlagsClear;
    int32_t                          GlobalFlagsSet;
    int32_t                          CriticalSectionDefaultTimeout;
    int32_t                          DeCommitFreeBlockThreshold;
    int32_t                          DeCommitTotalFreeThreshold;
    int32_t                          LockPrefixTable;
    uint32_t                         MaximumAllocationSize;
    int32_t                          VirtualMemoryThreshold;
    int32_t                          ProcessAffinityMask;
    int32_t                          ProcessHeapFlags;
    int16_t                          CSDVersion;
    int16_t                          DependentLoadFlags;                       // reserved, must be zero
    int32_t                          EditList;
    int32_t                          SecurityCookie;
    int32_t                          SEHandlerTable;
    int32_t                          SEHandlerCount;
    uint32_t                         GuardCFCheckFunctionPointer;
    uint32_t                         GuardCFDispatchFunctionPointer;
    int32_t                          GuardCFFunctionTable;
    int32_t                          GuardCFFunctionCount;
    int32_t                          GuardFlags;
    IMAGE_LOAD_CONFIG_CODE_INTEGRITY CodeIntegrity;
    uint32_t                         GuardAddressTakenIatEntryTable;
    uint32_t                         GuardAddressTakenIatEntryCount;
    int32_t                          GuardLongJumpTargetTable;
    int32_t                          GuardLongJumpTargetCount;
    int32_t                          DynamicValueRelocTable;
    uint32_t                         CHPEMetadataPointer;
    int32_t                          GuardRFFailureRoutine;
    uint32_t                         GuardRFFailureRoutineFunctionPointer;
    uint32_t                         DynamicValueRelocTableOffset;
    int16_t                          DynamicValueRelocTableSection;
    int16_t                          Reserved2;
    uint32_t                         GuardRFVerifyStackPointerFunctionPointer;
    uint32_t                         HotPatchTableOffset;
    int32_t                          Reserved3;
    uint32_t                         EnclaveConfigurationPointer;
    uint32_t                         VolatileMetadataPointer;
} IMAGE_LOAD_CONFIG_DIRECTORY32, *PIMAGE_LOAD_CONFIG_DIRECTORY32;

typedef struct _IMAGE_LOAD_CONFIG_DIRECTORY64 {
    uint32_t                         Size;
    int32_t                          TimeDateStamp;
    int16_t                          MajorVersion;
    int16_t                          MinorVersion;
    int32_t                          GlobalFlagsClear;
    int32_t                          GlobalFlagsSet;
    int32_t                          CriticalSectionDefaultTimeout;
    int64_t                          DeCommitFreeBlockThreshold;
    int64_t                          DeCommitTotalFreeThreshold;
    int64_t                          LockPrefixTable;
    uint64_t                         MaximumAllocationSize;
    int64_t                          VirtualMemoryThreshold;
    int64_t                          ProcessAffinityMask;
    int32_t                          ProcessHeapFlags;
    int16_t                          CSDVersion;
    int16_t                          DependentLoadFlags;                       // reserved, must be zero
    int64_t                          EditList;
    int64_t                          SecurityCookie;
    int64_t                          SEHandlerTable;
    int64_t                          SEHandlerCount;
    uint64_t                         GuardCFCheckFunctionPointer;
    uint64_t                         GuardCFDispatchFunctionPointer;
    int64_t                          GuardCFFunctionTable;
    int64_t                          GuardCFFunctionCount;
    int32_t                          GuardFlags;
    IMAGE_LOAD_CONFIG_CODE_INTEGRITY CodeIntegrity;
    uint64_t                         GuardAddressTakenIatEntryTable;
    uint64_t                         GuardAddressTakenIatEntryCount;
    int64_t                          GuardLongJumpTargetTable;
    int64_t                          GuardLongJumpTargetCount;
    int64_t                          DynamicValueRelocTable;
    uint64_t                         CHPEMetadataPointer;
    int64_t                          GuardRFFailureRoutine;
    uint64_t                         GuardRFFailureRoutineFunctionPointer;
    uint32_t                         DynamicValueRelocTableOffset;
    int16_t                          DynamicValueRelocTableSection;
    int16_t                          Reserved2;
    uint64_t                         GuardRFVerifyStackPointerFunctionPointer;
    uint32_t                         HotPatchTableOffset;
    int32_t                          Reserved3;
    uint64_t                         EnclaveConfigurationPointer;
    uint64_t                         VolatileMetadataPointer;
} IMAGE_LOAD_CONFIG_DIRECTORY64, *PIMAGE_LOAD_CONFIG_DIRECTORY64;

/* BOUND IMPORT DIRECTORY */

typedef struct _IMAGE_BOUND_IMPORT_DESCRIPTOR {
    int32_t  TimeDateStamp;
    uint16_t OffsetModuleName;
    uint16_t NumberOfModuleForwarderRefs;
} IMAGE_BOUND_IMPORT_DESCRIPTOR,*PIMAGE_BOUND_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_BOUND_FORWARDER_REF {
    int32_t  TimeDateStamp;
    uint16_t OffsetModuleName;
    int16_t  Reserved;
} IMAGE_BOUND_FORWARDER_REF,*PIMAGE_BOUND_FORWARDER_REF;

/* IAT DIRECTORY */

// It pretty much recycles THUNK_DATA from regular imports

/* DELAY IMPORT DIRECTORY */

typedef struct _IMAGE_DELAY_IMPORT_DESCRIPTOR {
    int32_t  Attributes;                 // reserved, must be zero
    uint32_t NameRVA;                    // pointer to DLL's name
    uint32_t ModuleHandleRVA;            // pointer to DLL's module handle
    uint32_t DelayImportAddressTableRVA; // pointer to delay-load import address table
    uint32_t DelayImportNameTableRVA;    // pointer to delay-load import name table
    uint32_t BoundDelayImportTableRVA;   // pointer to bound delay-load address table
    uint32_t UnloadDelayImportTableRVA;  // pointer to unload delay-load address table
    int32_t  Timestamp;                  // dll timestamp after binding
} IMAGE_DELAY_IMPORT_DESCRIPTOR, *PIMAGE_DELAY_IMPORT_DESCRIPTOR;

/* CLR DIRECTORY */

typedef struct _IMAGE_COR20_HEADER {
    uint32_t             Size;                // size of this header
    int16_t              MajorRuntimeVersion; // CLR major version number
    int16_t              MinorRuntimeVersion; // CLR minor version number
    IMAGE_DATA_DIRECTORY MetaData;
    int32_t              Flags;
    union {
        int32_t          EntryPointToken;
        uint32_t         EntryPointRVA;
    };
    IMAGE_DATA_DIRECTORY Resources;
    IMAGE_DATA_DIRECTORY StrongNameSignature;
    IMAGE_DATA_DIRECTORY CodeManagerTable;
    IMAGE_DATA_DIRECTORY VTableFixups;
    IMAGE_DATA_DIRECTORY ExportAddressTableJumps;
    IMAGE_DATA_DIRECTORY ManagedNativeHeader;
} IMAGE_COR20_HEADER, *PIMAGE_COR20_HEADER;

typedef struct _IMAGE_COR20_METADATA_HEADER {
    int32_t  Signature;       // always set to "BSJB"
    int16_t  MajorVersion;    // metadata major version number
    int16_t  MinorVersion;    // metadata minor version number
    int32_t  Reserved;        // reserved, must be zero
    uint32_t Length;          // Version length (multiple of 4)
    char     Version[];       // length-prefixed string
    // FLEX SPACE
//  int16_t  Flags;           // reserved, must be zero
//  uint16_t NumberOfStreams;
} IMAGE_COR20_METADATA_HEADER, *PIMAGE_COR20_METADATA_HEADER;

typedef struct _IMAGE_COR20_METADATA_STREAM_HEADER {
    uint32_t Offset; // relative to METADATA_header
    uint32_t Size;
    char     Name[];
} IMAGE_COR20_METADATA_STREAM_HEADER, *PIMAGE_COR20_METADATA_STREAM_HEADER;

typedef struct _IMAGE_COR20_RESOURCES_HEADER {
    int32_t  Unknown1; // TODO: is this total size - 4?
    uint32_t Magic;    // always set to 0xBEEFCACE
    int32_t  Unknown2; // TODO: is this a count of something?
    uint32_t Size;
} IMAGE_COR20_RESOURCES_HEADER, *PIMAGE_COR20_RESOURCES_HEADER;

// custom structs used in CLR directory

typedef struct _IMAGE_COR20_RESOURCES_STRING {
    uint8_t Length;
    char    String[];
} IMAGE_COR20_RESOURCES_STRING, *PIMAGE_COR20_RESOURCES_STRING;

typedef struct _IMAGE_COR20_RESOURCES_WSTRING {
    uint8_t  Length;
    uint16_t String[]; // TODO: maybe char16_t
} IMAGE_COR20_RESOURCES_WSTRING, *PIMAGE_COR20_RESOURCES_WSTRING;

typedef struct _IMAGE_COR20_RESOURCES_TAGGED_STRING {
    uint8_t Type;
    uint8_t Length;
    char    String[];
} IMAGE_COR20_RESOURCES_TAGGED_STRING, *PIMAGE_COR20_RESOURCES_TAGGED_STRING;

typedef struct _IMAGE_COR20_RESOURCES_TAGGED_WSTRING {
    uint8_t  Type;
    uint8_t  Length;
    uint16_t String[]; // TODO: maybe char16_t
} IMAGE_COR20_RESOURCES_TAGGED_WSTRING, *PIMAGE_COR20_RESOURCES_TAGGED_WSTRING;

#ifdef __cplusplus
} // namespace Pepper
#endif

#endif
