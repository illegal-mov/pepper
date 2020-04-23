#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include "../Types.h"
#include "../generics/iDir.h"
#include "struct.h"

/* Online documentation on the LOAD_CONFIG structure is inconsistent.
 *
 * https://docs.microsoft.com/en-us/windows/win32/debug/pe-format#the-load-configuration-structure-image-only
 * https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-image_load_config_directory32
 *
 * The number of fields in this structure appear to depend on the OS version, with more fields coinciding with more recent versions of Windows.
 *
 * Note: DataDirectory[LDCFG].size() is only used for version checks. It must be 0x40 for 32bit Windows XP or earlier.
 */

/*
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
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/* The CODE_INTEGRITY structure is a single data structure contained within the LOAD_CONFIG data structure.
 */
class CodeIntegrity final : public IHeader {
public:
    enum Fields {
        FLAGS,
        CATALOG,
        CATALOG_OFFSET,
        RESERVED,
        _NUM_FIELDS,
    };

    CodeIntegrity() = default;

    CodeIntegrity(const FileBytes& fbytes, const offset_t raw);

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_LOAD_CONFIG_CODE_INTEGRITY* integrity() const
    {
        return static_cast<const IMAGE_LOAD_CONFIG_CODE_INTEGRITY*>(hdr());
    }

    // static functions
    static const char* getFieldName(const int index);
};

/* The LOAD_CONFIG directory is a single 32-bit or 64-bit data structure.
 */
class LoadConfigDir final : public IDirectory {
public:
    enum Fields {
        SIZE,
        TIMESTAMP,
        MAJOR_VERSION,
        MINOR_VERSION,
        GLOBAL_FLAGS_CLEAR,
        GLOBAL_FLAGS_SET,
        CRITICAL_SECTION_DEFAULT_TIMEOUT,
        DECOMMIT_FREE_BLOCK_THRESHOLD,
        DECOMMIT_TOTAL_FREE_THRESHOLD,
        LOCK_PREFIX_TABLE,
        MAXIMUM_ALLOCATION_SIZE,
        VIRTUAL_MEMORY_THRESHOLD,
        PROCESS_AFFINITY_MASK,
        PROCESS_HEAP_FLAGS,
        CSD_VERSION,
        DEPENDENT_LOAD_FLAGS,
        EDIT_LIST,
        SECURITY_COOKIE,
        SE_HANDLER_TABLE,
        SE_HANDLER_COUNT,
        GUARD_CF_CHECK_FUNCTION_POINTER,
        GUARD_CF_DISPATCH_FUNCTION_POINTER,
        GUARD_CF_FUNCTION_TABLE,
        GUARD_CF_FUNCTION_COUNT,
        GUARD_FLAGS,
        CODE_INTEGRITY,
        GUARD_ADDRESS_TAKEN_IAT_ENTRY_TABLE,
        GUARD_ADDRESS_TAKEN_IAT_ENTRY_COUNT,
        GUARD_LONG_JUMP_TARGET_TABLE,
        GUARD_LONG_JUMP_TARGET_COUNT,
        DYNAMIC_VALUE_RELOC_TABLE,
        CHPE_METADATA_POINTER,
        GUARD_RF_FAILURE_ROUTINE,
        GUARD_RF_FAILURE_ROUTINE_FUNCTION_POINTER,
        DYNAMIC_VALUE_RELOC_TABLE_OFFSET,
        DYNAMIC_VALUE_RELOC_TABLE_SECTION,
        RESERVED2,
        GUARD_RF_VERIFY_STACK_POINTER_FUNCTION_POINTER,
        HOT_PATCH_TABLE_OFFSET,
        RESERVED3,
        ENCLAVE_CONFIG_POINTER,
        VOLATILE_METADATA_POINTER,
        _NUM_FIELDS,
    };

    LoadConfigDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    // member functions
    const void* getFieldPtr(const int index) const override;

    const IMAGE_LOAD_CONFIG_DIRECTORY32* ldcfg32() const
    {
        return static_cast<const IMAGE_LOAD_CONFIG_DIRECTORY32*>(dir());
    }

    const IMAGE_LOAD_CONFIG_DIRECTORY64* ldcfg64() const
    {
        return static_cast<const IMAGE_LOAD_CONFIG_DIRECTORY64*>(dir());
    }

    // static functions
    static const char* getFieldName(const int index);

private:
    CodeIntegrity m_codeInteg;
};
} // namespace Pepper

#endif
