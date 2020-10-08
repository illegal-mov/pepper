#include "../../include/headers/pe_structures/ImportsCommon.h"

#include <sstream>

#include "../../include/headers/Conversion.h"
#include "../../include/headers/Types.h"

using namespace Pepper;

namespace
{
    template <typename ArchType>
    std::string makeOrdinalString(ArchType ordinal)
    {
        std::stringstream ss;
        ss << std::hex << "[Ordinal: 0x";
        ss.fill('0');
        ss.width(sizeof(ArchType)<<1); // 2 hex digits per byte
        ss << ordinal << ']';
        return ss.str();
    }
} // namespace

/* Load up the vector with data from the ImportAddressTable
 */
template <typename DescriptorType> // for the class
template <typename ArchType> // for the function
void GenericImportDescriptor<DescriptorType>::readAddresses(size_t raw)
{
    const ArchType* addr = reinterpret_cast<const ArchType*>(&mem()[raw]);
    size_t i = 0;
    while (addr[i] != 0) {
        m_addresses32.push_back(addr[i]);
        i++;
    }
}

/* Load up the vector with data from the ImportLookupTable
 */
template <typename DescriptorType> // for the class
template <typename ArchType> // for the function
void GenericImportDescriptor<DescriptorType>::readThunks(const FileBytes& fbytes, const offset_t raw)
{
    const ArchType* thunk = reinterpret_cast<const ArchType*>(&mem()[raw]);
    size_t i = 0;
    while (thunk[i].HintNameTableRVA != 0) {
        if (thunk[i].OrdinalFlag) {
            std::string ordinal = makeOrdinalString(thunk[i].OrdinalNumber);
            m_thunks32.emplace_back(fbytes, raw + (i * sizeof(ArchType)), ordinal);
        } else {
            m_thunks32.emplace_back(fbytes, raw + (i * sizeof(ArchType)));
        }

        i++;
    }
}

template <typename DescriptorType>
template <int ILT, int IAT, int TIMESTAMP>
void GenericImportDescriptor<DescriptorType>::makeDescriptor(const PeFile& pe, const FileBytes& fbytes)
{
    /* `ImportLookupTableRVA` may be zero, especially for install files.
     * Switch to `ImportAddressTableRVA` if so, and check if IAT is bound
     * (contains AVAs) by seeing if TimeDateStamp == -1 (don't ask).
     * Nothing to parse if ImportLookupTableRVA is zero and IAT is bound.
     */
    ptr32_t iltRva = *static_cast<const ptr32_t*>(getFieldPtr(ILT));
    ptr32_t iatRva = *static_cast<const ptr32_t*>(getFieldPtr(IAT));
    if (iltRva == 0) {
        if (*static_cast<const int32_t*>(getFieldPtr(TIMESTAMP)) == -1) {
            return; // it's bound, do nothing
        } else {
            iltRva = *static_cast<const ptr32_t*>(getFieldPtr(IAT));
        }
    }

    iltRva = Convert::convertAddr(pe, iltRva, Convert::AddrType::RVA, Convert::AddrType::RAW);
    iatRva = Convert::convertAddr(pe, iatRva, Convert::AddrType::RVA, Convert::AddrType::RAW);

    if (Ident::is32bit(pe)) {
        readThunks<IMAGE_THUNK_DATA32>(fbytes, iltRva);
        readAddresses<ptr32_t>(iatRva);
    } else {
        readThunks<IMAGE_THUNK_DATA64>(fbytes, iltRva);
        readAddresses<ptr64_t>(iatRva);
    }
}

/* Template specialized constructors.
 * These constructors call a separate function since
 * the implementations are practically the same.
 */
template<>
GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::GenericImportDescriptor(const PeFile& pe, const FileBytes& fbytes, const offset_t raw)
: IHeader(fbytes, raw)
{
    makeDescriptor<IMPORT_LOOKUP_TABLE_RVA, IMPORT_ADDRESS_TABLE_RVA, TIMESTAMP>(pe, fbytes);
}

template<>
GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>::GenericImportDescriptor(const PeFile& pe, const FileBytes& fbytes, const offset_t raw)
: IHeader(fbytes, raw)
{
    makeDescriptor<DELAY_IMPORT_NAME_TABLE_RVA, DELAY_IMPORT_ADDRESS_TABLE_RVA, TIMESTAMP>(pe, fbytes);
}

const char* ImportName::getFieldName(const int index)
{
    switch (index) {
        case HINT: return "Hint";
        case NAME: return "Name";
        default  : return "<UNKNOWN>";
    }
}

const void* ImportName::getFieldPtr(const int index) const
{
    switch (index) {
        case HINT: return &getStructPtr()->Hint;
        case NAME: return &getStructPtr()->Name;
        default  : return nullptr;
    }
}

template <typename ArchType>
const char* ImportThunk<ArchType>::getFieldName(const int index)
{
    switch (index) {
        case HINTNAMERVA: return "Pointer to Hint/Name Table";
        default         : return "<UNKNOWN>";
    }
}

template <typename ArchType>
const void* ImportThunk<ArchType>::getFieldPtr(const int index) const
{
    switch (index) {
        case HINTNAMERVA: return &getStructPtr()->HintNameTableRVA;
        default         : return nullptr;
    }
}

template<>
const char* GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::getFieldName(const int index)
{
    switch (index) {
        case IMPORT_LOOKUP_TABLE_RVA : return "Pointer to Import Lookup Table";
        case TIMESTAMP               : return "Timestamp";
        case FORWARDER_CHAIN         : return "Forwarder Chain";
        case NAME_RVA                : return "Pointer to DLL Name";
        case IMPORT_ADDRESS_TABLE_RVA: return "Pointer to Import Address Table";
        default                      : return "<UNKNOWN>";
    }
}

template<>
const void* GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::getFieldPtr(const int index) const
{
    switch (index) {
        case IMPORT_LOOKUP_TABLE_RVA : return &getStructPtr()->ImportLookupTableRVA;
        case TIMESTAMP               : return &getStructPtr()->TimeDateStamp;
        case FORWARDER_CHAIN         : return &getStructPtr()->ForwarderChain;
        case NAME_RVA                : return &getStructPtr()->NameRVA;
        case IMPORT_ADDRESS_TABLE_RVA: return &getStructPtr()->ImportAddressTableRVA;
        default                      : return nullptr;
    }
}

template<>
const char* GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::dllName() const
{
    ptr32_t nameRva = getStructPtr()->NameRVA;
    return reinterpret_cast<const char*>(&mem()[nameRva - s_pDiskToMemDiff]);
}

template<>
const char* GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>::getFieldName(const int index)
{
    switch (index) {
        case ATTRIBUTES                    : return "Attributes";
        case NAME_RVA                      : return "Pointer to DLL Name";
        case MODULE_HANDLE_RVA             : return "Pointer to Module Handle";
        case DELAY_IMPORT_ADDRESS_TABLE_RVA: return "Pointer to Delay Import Address Table";
        case DELAY_IMPORT_NAME_TABLE_RVA   : return "Pointer to Delay Import Name Table";
        case BOUND_DELAY_IMPORT_TABLE_RVA  : return "Pointer to Bound Delay Import Table";
        case UNLOAD_DELAY_IMPORT_TABLE_RVA : return "Pointer to Unload Delay Import Table";
        case TIMESTAMP                     : return "Timestamp";
        default                            : return "<UNKNOWN>";
    }
}

template<>
const void* GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>::getFieldPtr(const int index) const
{
    switch (index) {
        case ATTRIBUTES                    : return &getStructPtr()->Attributes;
        case NAME_RVA                      : return &getStructPtr()->NameRVA;
        case MODULE_HANDLE_RVA             : return &getStructPtr()->ModuleHandleRVA;
        case DELAY_IMPORT_ADDRESS_TABLE_RVA: return &getStructPtr()->DelayImportAddressTableRVA;
        case DELAY_IMPORT_NAME_TABLE_RVA   : return &getStructPtr()->DelayImportNameTableRVA;
        case BOUND_DELAY_IMPORT_TABLE_RVA  : return &getStructPtr()->BoundDelayImportTableRVA;
        case UNLOAD_DELAY_IMPORT_TABLE_RVA : return &getStructPtr()->UnloadDelayImportTableRVA;
        case TIMESTAMP                     : return &getStructPtr()->Timestamp;
        default                            : return nullptr;
    }
}

template<>
const char* GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>::dllName() const
{
    ptr32_t nameRva = getStructPtr()->NameRVA;
    return reinterpret_cast<const char*>(&mem()[nameRva - s_pDiskToMemDiff]);
}

