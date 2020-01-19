#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "LoadConfigDir.h"

using namespace Pepper;

CodeIntegrity::CodeIntegrity(const FileBytes &fbytes, size_t raw)
: IHeader(fbytes, raw)
{}

LoadConfigDir::LoadConfigDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
, m_codeInteg()
{
    if (Ident::dirExists(*this)) {
        bool is32bit = Ident::is32bit(*m_pe);
        uint32_t size = ldcfg32()->Size;
        if (is32bit && offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) < size) {
            m_codeInteg = CodeIntegrity(fbytes, dirOffset()
                + offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity));
        } else if (!is32bit && offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32, CodeIntegrity) < size) {
            m_codeInteg = CodeIntegrity(fbytes, dirOffset()
                + offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64, CodeIntegrity));
        }
    }
}

const char* CodeIntegrity::getFieldName(int index)
{
    switch (index) {
        case FLAGS         : return "Flags";
        case CATALOG       : return "Catalog";
        case CATALOG_OFFSET: return "Catalog Offset";
        case RESERVED      : return "<Reserved>";
        default            : return "<UNKNOWN>";
    }
}

const void* CodeIntegrity::getFieldPtr(int index) const
{
    switch (index) {
        case FLAGS         : return &integrity()->Flags;
        case CATALOG       : return &integrity()->Catalog;
        case CATALOG_OFFSET: return &integrity()->CatalogOffset;
        case RESERVED      : return &integrity()->Reserved;
        default            : return nullptr;
    }
}

const char* LoadConfigDir::getFieldName(int index)
{
    switch (index) {
        case SIZE                                          : return "Size";
        case TIMESTAMP                                     : return "Timestamp";
        case MAJOR_VERSION                                 : return "Major Version";
        case MINOR_VERSION                                 : return "Minor Version";
        case GLOBAL_FLAGS_CLEAR                            : return "Global Flags to Clear";
        case GLOBAL_FLAGS_SET                              : return "Global Flags to Set";
        case CRITICAL_SECTION_DEFAULT_TIMEOUT              : return "Critical Section Default Timeout";
        case DECOMMIT_FREE_BLOCK_THRESHOLD                 : return "Decommit Free Block";
        case DECOMMIT_TOTAL_FREE_THRESHOLD                 : return "Decommit Total Free";
        case LOCK_PREFIX_TABLE                             : return "Lock Prefix Table Address";
        case MAXIMUM_ALLOCATION_SIZE                       : return "Max Allocation Size";
        case VIRTUAL_MEMORY_THRESHOLD                      : return "Max Virtual Memory Size";
        case PROCESS_AFFINITY_MASK                         : return "Process Affinity Mask";
        case PROCESS_HEAP_FLAGS                            : return "Process Heap Flags";
        case CSD_VERSION                                   : return "CSD Version";
        case DEPENDENT_LOAD_FLAGS                          : return "Dependent Load Flags";
        case EDIT_LIST                                     : return "Edit List";
        case SECURITY_COOKIE                               : return "Security Cookie";
        case SE_HANDLER_TABLE                              : return "SEH Table Address";
        case SE_HANDLER_COUNT                              : return "SEH Table Length";
        case GUARD_CF_CHECK_FUNCTION_POINTER               : return "CFG Check Function Address";
        case GUARD_CF_DISPATCH_FUNCTION_POINTER            : return "CFG Dispatch Function Address";
        case GUARD_CF_FUNCTION_TABLE                       : return "CFG Function Pointer Table Address";
        case GUARD_CF_FUNCTION_COUNT                       : return "CFG Function Pointer Table Length";
        case GUARD_FLAGS                                   : return "CFG FLags";
        case CODE_INTEGRITY                                : return "Code Integrity";
        case GUARD_ADDRESS_TAKEN_IAT_ENTRY_TABLE           : return "CFG IAT Table Address";
        case GUARD_ADDRESS_TAKEN_IAT_ENTRY_COUNT           : return "CFG IAT Table Length";
        case GUARD_LONG_JUMP_TARGET_TABLE                  : return "CFG Jump Table Address";
        case GUARD_LONG_JUMP_TARGET_COUNT                  : return "CFG Jump Table Length";
        case DYNAMIC_VALUE_RELOC_TABLE                     : return "Dynamic Value Relocation Table Address";
        case CHPE_METADATA_POINTER                         : return "CHPE Metadata Pointer";
        case GUARD_RF_FAILURE_ROUTINE                      : return "CFG RF Failure Routine";
        case GUARD_RF_FAILURE_ROUTINE_FUNCTION_POINTER     : return "CFG RF Failure Routine Function Pointer";
        case DYNAMIC_VALUE_RELOC_TABLE_OFFSET              : return "Dynamic Value Relocation Table Offset";
        case DYNAMIC_VALUE_RELOC_TABLE_SECTION             : return "Dynamic Value Relocation Table Section";
        case RESERVED2                                     : return "<Reserved>";
        case GUARD_RF_VERIFY_STACK_POINTER_FUNCTION_POINTER: return "CFG RF Verify Stack Pointer Function Pointer";
        case HOT_PATCH_TABLE_OFFSET                        : return "Hot Patch Table Offset";
        case RESERVED3                                     : return "<Reserved>";
        case ENCLAVE_CONFIG_POINTER                        : return "Enclave Config Address";
        case VOLATILE_METADATA_POINTER                     : return "Volatile Metadata Address";
        default                                             : return "<UNKNOWN>";
    }
}

/* Always check that a field access is within range since
 * this structure has a varying number of fields depending
 * on the version of Windows the binary was compiled for.
 * This macro just reduces source code verbosity by
 * including the range and architecture checks.
 */
#define LDCFG_RET_FIELD(FIELD)                                       \
    (   is32bit && offsetof(IMAGE_LOAD_CONFIG_DIRECTORY32, FIELD) < size) \
    ? (void*)&ldcfg32()->FIELD                                       \
    : (!is32bit && offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64, FIELD) < size) \
    ? (void*)&ldcfg64()->FIELD : nullptr

const void* LoadConfigDir::getFieldPtr(int index) const
{
    bool is32bit = Ident::is32bit(*m_pe);
    uint32_t size = ldcfg32()->Size;
    switch (index) {
        case SIZE                                          : return LDCFG_RET_FIELD(Size);
        case TIMESTAMP                                     : return LDCFG_RET_FIELD(TimeDateStamp);
        case MAJOR_VERSION                                 : return LDCFG_RET_FIELD(MajorVersion);
        case MINOR_VERSION                                 : return LDCFG_RET_FIELD(MinorVersion);
        case GLOBAL_FLAGS_CLEAR                            : return LDCFG_RET_FIELD(GlobalFlagsClear);
        case GLOBAL_FLAGS_SET                              : return LDCFG_RET_FIELD(GlobalFlagsSet);
        case CRITICAL_SECTION_DEFAULT_TIMEOUT              : return LDCFG_RET_FIELD(CriticalSectionDefaultTimeout);
        case DECOMMIT_FREE_BLOCK_THRESHOLD                 : return LDCFG_RET_FIELD(DeCommitFreeBlockThreshold);
        case DECOMMIT_TOTAL_FREE_THRESHOLD                 : return LDCFG_RET_FIELD(DeCommitTotalFreeThreshold);
        case LOCK_PREFIX_TABLE                             : return LDCFG_RET_FIELD(LockPrefixTable);
        case MAXIMUM_ALLOCATION_SIZE                       : return LDCFG_RET_FIELD(MaximumAllocationSize);
        case VIRTUAL_MEMORY_THRESHOLD                      : return LDCFG_RET_FIELD(VirtualMemoryThreshold);
        case PROCESS_AFFINITY_MASK                         : return LDCFG_RET_FIELD(ProcessAffinityMask);
        case PROCESS_HEAP_FLAGS                            : return LDCFG_RET_FIELD(ProcessHeapFlags);
        case CSD_VERSION                                   : return LDCFG_RET_FIELD(CSDVersion);
        case DEPENDENT_LOAD_FLAGS                          : return LDCFG_RET_FIELD(DependentLoadFlags);
        case EDIT_LIST                                     : return LDCFG_RET_FIELD(EditList);
        case SECURITY_COOKIE                               : return LDCFG_RET_FIELD(SecurityCookie);
        case SE_HANDLER_TABLE                              : return LDCFG_RET_FIELD(SEHandlerTable);
        case SE_HANDLER_COUNT                              : return LDCFG_RET_FIELD(SEHandlerCount);
        case GUARD_CF_CHECK_FUNCTION_POINTER               : return LDCFG_RET_FIELD(GuardCFCheckFunctionPointer);
        case GUARD_CF_DISPATCH_FUNCTION_POINTER            : return LDCFG_RET_FIELD(GuardCFDispatchFunctionPointer);
        case GUARD_CF_FUNCTION_TABLE                       : return LDCFG_RET_FIELD(GuardCFFunctionTable);
        case GUARD_CF_FUNCTION_COUNT                       : return LDCFG_RET_FIELD(GuardCFFunctionCount);
        case GUARD_FLAGS                                   : return LDCFG_RET_FIELD(GuardFlags);
        case CODE_INTEGRITY                                : return (Ident::dirExists(*this)) ? &m_codeInteg : nullptr;
        case GUARD_ADDRESS_TAKEN_IAT_ENTRY_TABLE           : return LDCFG_RET_FIELD(GuardAddressTakenIatEntryTable);
        case GUARD_ADDRESS_TAKEN_IAT_ENTRY_COUNT           : return LDCFG_RET_FIELD(GuardAddressTakenIatEntryCount);
        case GUARD_LONG_JUMP_TARGET_TABLE                  : return LDCFG_RET_FIELD(GuardLongJumpTargetTable);
        case GUARD_LONG_JUMP_TARGET_COUNT                  : return LDCFG_RET_FIELD(GuardLongJumpTargetCount);
        case DYNAMIC_VALUE_RELOC_TABLE                     : return LDCFG_RET_FIELD(DynamicValueRelocTable);
        case CHPE_METADATA_POINTER                         : return LDCFG_RET_FIELD(CHPEMetadataPointer);
        case GUARD_RF_FAILURE_ROUTINE                      : return LDCFG_RET_FIELD(GuardRFFailureRoutine);
        case GUARD_RF_FAILURE_ROUTINE_FUNCTION_POINTER     : return LDCFG_RET_FIELD(GuardRFFailureRoutineFunctionPointer);
        case DYNAMIC_VALUE_RELOC_TABLE_OFFSET              : return LDCFG_RET_FIELD(DynamicValueRelocTableOffset);
        case DYNAMIC_VALUE_RELOC_TABLE_SECTION             : return LDCFG_RET_FIELD(DynamicValueRelocTableSection);
        case RESERVED2                                     : return LDCFG_RET_FIELD(Reserved2);
        case GUARD_RF_VERIFY_STACK_POINTER_FUNCTION_POINTER: return LDCFG_RET_FIELD(GuardRFVerifyStackPointerFunctionPointer);
        case HOT_PATCH_TABLE_OFFSET                        : return LDCFG_RET_FIELD(HotPatchTableOffset);
        case RESERVED3                                     : return LDCFG_RET_FIELD(Reserved3);
        case ENCLAVE_CONFIG_POINTER                        : return LDCFG_RET_FIELD(EnclaveConfigurationPointer);
        case VOLATILE_METADATA_POINTER                     : return LDCFG_RET_FIELD(VolatileMetadataPointer);
        default                                            : return nullptr;
    }
}

