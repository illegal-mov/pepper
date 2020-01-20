#include "OptionalHdr.h"

using namespace Pepper;

OptionalHeader::OptionalHeader(const FileBytes &fbytes, const FileHeader &file)
: IHeader(fbytes, 0)
{
    m_baseOffset = file.hdrOffset() + sizeof(IMAGE_FILE_HEADER);
}

DataDirectory::DataDirectory(const FileBytes &fbytes, const OptionalHeader &opt)
{
    // get offset to data directory
    const bool is32bit = opt.optional32()->Magic == OptionalHeader::BIT32;
    const size_t base = opt.hdrOffset() + ((is32bit) ?
        offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory):
        offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory));

    // construct each data directory entry by copy assignment
    size_t index = 0;
    for (auto &entry : m_directoryEntries) {
        entry = DataDirectoryEntry(fbytes, base + (index * sizeof(IMAGE_DATA_DIRECTORY)));
        index++;
    }
}

uint64_t OptionalHeader::imageBase() const
{
    return (optional32()->Magic == BIT32)
        ? optional32()->ImageBase
        : optional64()->ImageBase;
}

const char* OptionalHeader::getFieldName(const int index)
{
    switch (index) {
        case MAGIC                         : return "Magic";
        case MAJOR_LINKER_VERSION          : return "Linker Major Version";
        case MINOR_LINKER_VERSION          : return "Linker Minor Version";
        case SIZE_OF_CODE                  : return "Size of Code";
        case SIZE_OF_INITIALIZED_DATA      : return "Size of Initialized Data";
        case SIZE_OF_UNINITIALIZED_DATA    : return "Size of Uninitialized Data";
        case ADDRESS_OF_ENTRY_POINT        : return "Address of Entry Point";
        case BASE_OF_CODE                  : return "Base of Code";
        case BASE_OF_DATA                  : return "Base of Data";
        case IMAGE_BASE                    : return "Image Base Address";
        case SECTION_ALIGNMENT             : return "Section Alignment";
        case FILE_ALIGNMENT                : return "File Alignment";
        case MAJOR_OPERATING_SYSTEM_VERSION: return "OS Major Version";
        case MINOR_OPERATING_SYSTEM_VERSION: return "OS Minor Version";
        case MAJOR_IMAGE_VERSION           : return "Image Major Version";
        case MINOR_IMAGE_VERSION           : return "Image Minor Version";
        case MAJOR_SUBSYSTEM_VERSION       : return "Subsystem Major Version";
        case MINOR_SUBSYSTEM_VERSION       : return "Subsystem Minor Version";
        case WIN32_VERSION_VALUE           : return "Win32 Version";
        case SIZE_OF_IMAGE                 : return "Size of Image";
        case SIZE_OF_HEADERS               : return "Size of Headers";
        case CHECKSUM                      : return "Image File Checksum";
        case SUBSYSTEM                     : return "Required Subsystem";
        case DLL_CHARACTERISTICS           : return "DLL Characteristics";
        case SIZE_OF_STACK_RESERVE         : return "Size of Stack Reserve";
        case SIZE_OF_STACK_COMMIT          : return "Size of Stack Commit";
        case SIZE_OF_HEAP_RESERVE          : return "Size of Heap Reserve";
        case SIZE_OF_HEAP_COMMIT           : return "Size of Heap Commit";
        case LOADER_FLAGS                  : return "Loader Flags";
        case NUMBER_OF_RVA_AND_SIZES       : return "Data Directory Length";
        case DATA_DIRECTORY                : return "Data Directory";
        default                            : return "<UNKNOWN>";
    }
}

const void* OptionalHeader::getFieldPtr(const int index) const
{
    const bool is32bit = optional32()->Magic == BIT32;
    switch (index) {
        case MAGIC                         : return (is32bit) ? (void*)&optional32()->Magic                       : (void*)&optional64()->Magic;
        case MAJOR_LINKER_VERSION          : return (is32bit) ? (void*)&optional32()->MajorLinkerVersion          : (void*)&optional64()->MajorLinkerVersion;
        case MINOR_LINKER_VERSION          : return (is32bit) ? (void*)&optional32()->MinorLinkerVersion          : (void*)&optional64()->MinorLinkerVersion;
        case SIZE_OF_CODE                  : return (is32bit) ? (void*)&optional32()->SizeOfCode                  : (void*)&optional64()->SizeOfCode;
        case SIZE_OF_INITIALIZED_DATA      : return (is32bit) ? (void*)&optional32()->SizeOfInitializedData       : (void*)&optional64()->SizeOfInitializedData;
        case SIZE_OF_UNINITIALIZED_DATA    : return (is32bit) ? (void*)&optional32()->SizeOfUninitializedData     : (void*)&optional64()->SizeOfUninitializedData;
        case ADDRESS_OF_ENTRY_POINT        : return (is32bit) ? (void*)&optional32()->AddressOfEntryPoint         : (void*)&optional64()->AddressOfEntryPoint;
        case BASE_OF_CODE                  : return (is32bit) ? (void*)&optional32()->BaseOfCode                  : (void*)&optional64()->BaseOfCode;
        case BASE_OF_DATA                  : return (is32bit) ? (void*)&optional32()->BaseOfData                  : (void*)nullptr;
        case IMAGE_BASE                    : return (is32bit) ? (void*)&optional32()->ImageBase                   : (void*)&optional64()->ImageBase;
        case SECTION_ALIGNMENT             : return (is32bit) ? (void*)&optional32()->SectionAlignment            : (void*)&optional64()->SectionAlignment;
        case FILE_ALIGNMENT                : return (is32bit) ? (void*)&optional32()->FileAlignment               : (void*)&optional64()->FileAlignment;
        case MAJOR_OPERATING_SYSTEM_VERSION: return (is32bit) ? (void*)&optional32()->MajorOperatingSystemVersion : (void*)&optional64()->MajorOperatingSystemVersion;
        case MINOR_OPERATING_SYSTEM_VERSION: return (is32bit) ? (void*)&optional32()->MinorOperatingSystemVersion : (void*)&optional64()->MinorOperatingSystemVersion;
        case MAJOR_IMAGE_VERSION           : return (is32bit) ? (void*)&optional32()->MajorImageVersion           : (void*)&optional64()->MajorImageVersion;
        case MINOR_IMAGE_VERSION           : return (is32bit) ? (void*)&optional32()->MinorImageVersion           : (void*)&optional64()->MinorImageVersion;
        case MAJOR_SUBSYSTEM_VERSION       : return (is32bit) ? (void*)&optional32()->MajorSubsystemVersion       : (void*)&optional64()->MajorSubsystemVersion;
        case MINOR_SUBSYSTEM_VERSION       : return (is32bit) ? (void*)&optional32()->MinorSubsystemVersion       : (void*)&optional64()->MinorSubsystemVersion;
        case WIN32_VERSION_VALUE           : return (is32bit) ? (void*)&optional32()->Win32VersionValue           : (void*)&optional64()->Win32VersionValue;
        case SIZE_OF_IMAGE                 : return (is32bit) ? (void*)&optional32()->SizeOfImage                 : (void*)&optional64()->SizeOfImage;
        case SIZE_OF_HEADERS               : return (is32bit) ? (void*)&optional32()->SizeOfHeaders               : (void*)&optional64()->SizeOfHeaders;
        case CHECKSUM                      : return (is32bit) ? (void*)&optional32()->CheckSum                    : (void*)&optional64()->CheckSum;
        case SUBSYSTEM                     : return (is32bit) ? (void*)&optional32()->Subsystem                   : (void*)&optional64()->Subsystem;
        case DLL_CHARACTERISTICS           : return (is32bit) ? (void*)&optional32()->DllCharacteristics          : (void*)&optional64()->DllCharacteristics;
        case SIZE_OF_STACK_RESERVE         : return (is32bit) ? (void*)&optional32()->SizeOfStackReserve          : (void*)&optional64()->SizeOfStackReserve;
        case SIZE_OF_STACK_COMMIT          : return (is32bit) ? (void*)&optional32()->SizeOfStackCommit           : (void*)&optional64()->SizeOfStackCommit;
        case SIZE_OF_HEAP_RESERVE          : return (is32bit) ? (void*)&optional32()->SizeOfHeapReserve           : (void*)&optional64()->SizeOfHeapReserve;
        case SIZE_OF_HEAP_COMMIT           : return (is32bit) ? (void*)&optional32()->SizeOfHeapCommit            : (void*)&optional64()->SizeOfHeapCommit;
        case LOADER_FLAGS                  : return (is32bit) ? (void*)&optional32()->LoaderFlags                 : (void*)&optional64()->LoaderFlags;
        case NUMBER_OF_RVA_AND_SIZES       : return (is32bit) ? (void*)&optional32()->NumberOfRvaAndSizes         : (void*)&optional64()->NumberOfRvaAndSizes;
        case DATA_DIRECTORY                : return (is32bit) ? (void*)&optional32()->DataDirectory               : (void*)&optional64()->DataDirectory;
        default             : return nullptr;
    }
}

const char* OptionalHeader::getCharacteristicName(const int index)
{
    switch (index) {
        case RESERVED_1           : return "<Reserved>";
        case RESERVED_2           : return "<Reserved>";
        case RESERVED_4           : return "<Reserved>";
        case RESERVED_8           : return "<Reserved>";
        case RESERVED_10          : return "<Reserved>";
        case HIGH_ENTROPY_VA      : return "High Entropy Address Support";
        case DYNAMIC_BASE         : return "ASLR Support";
        case FORCE_INTEGRITY      : return "Code Integrity Checks Enabled";
        case NX_COMPAT            : return "DEP Support";
        case NO_ISOLATION         : return "Isolation Disabled";
        case NO_SEH               : return "SEH Disabled";
        case NO_BIND              : return "Image Binding Disabled";
        case APPCONTAINER         : return "Must Execute in AppContainer";
        case WDM_DRIVER           : return "Image is WDM Driver";
        case GUARD_CF             : return "Control Flow Guard Support";
        case TERMINAL_SERVER_AWARE: return "Terminal Server Aware";
        default                   : return "<UNKNOWN>";
    }
}

const char* DataDirectory::getFieldName(const int index)
{
    switch (index) {
        case XPRT  : return "Export";
        case MPRT  : return "Import";
        case RSRC  : return "Resource";
        case XCPT  : return "Exception";
        case CERT  : return "Certificate";
        case RLOC  : return "Relocation";
        case DBG   : return "Debug";
        case ARCH  : return "Architecture";
        case GLBPTR: return "Global Pointer";
        case TLS   : return "TLS";
        case LDCFG : return "Load Config";
        case BMPRT : return "Bound Import";
        case IAT   : return "IAT";
        case DMPRT : return "Delay Import";
        case CLR   : return "CLR";
        case RSRV  : return "<Reserved>";
        default    : return "<UNKNOWN>";
    }
}

const void* DataDirectory::getFieldPtr(const int index) const
{
    switch (index) {
        case XPRT  : return &directories()[XPRT];
        case MPRT  : return &directories()[MPRT];
        case RSRC  : return &directories()[RSRC];
        case XCPT  : return &directories()[XCPT];
        case CERT  : return &directories()[CERT];
        case RLOC  : return &directories()[RLOC];
        case DBG   : return &directories()[DBG];
        case ARCH  : return &directories()[ARCH];   // unused
        case GLBPTR: return &directories()[GLBPTR];
        case TLS   : return &directories()[TLS];
        case LDCFG : return &directories()[LDCFG];
        case BMPRT : return &directories()[BMPRT];
        case IAT   : return &directories()[IAT];
        case DMPRT : return &directories()[DMPRT];
        case CLR   : return &directories()[CLR];
        default    : return nullptr;
    }
}

