#include "../Conversion.h"
#include "ImportsCommon.h"

using namespace Pepper;

/* Load up the vector with data from the ImportAddressTable
 */
template <typename T> // for the class
template <typename U> // for the function
void GenericImportDescriptor<T>::readAddresses(size_t raw)
{
    U *addr = (U*)&mem()[raw];
    size_t i=0;
    while (addr[i] != 0) {
        m_addresses32.push_back(addr[i]);
        i++;
    }
}

/* Load up the vector with data from the ImportLookupTable
 */
template <typename T> // for the class
template <typename U> // for the function
void GenericImportDescriptor<T>::readThunks(const FileBytes &fbytes, const size_t raw)
{
    U *thunk = (U*)&mem()[raw];
    // count number of IMAGE_IMPORT_DESCRIPTORS up until the null descriptor
    size_t i=0;
    while (thunk[i].HintNameTableRVA != 0) {
        if (thunk[i].OrdinalFlag) {
            // make ordinal string
            std::stringstream ss;
            ss << std::hex << "[Ordinal: 0x";
            ss.fill('0');
            ss.width(sizeof(U)<<1); // 2 hex digits per byte
            ss << thunk[i].OrdinalNumber << ']';

            m_thunks32.emplace_back(fbytes, raw + (i * sizeof(U)), ss.str());
        } else {
            m_thunks32.emplace_back(fbytes, raw + (i * sizeof(U)));
        }

        i++;
    }
}

template <typename T>
template <int ILT, int IAT, int TIMESTAMP>
void GenericImportDescriptor<T>::makeDescriptor(const PeFile &pe, const FileBytes &fbytes)
{
    /* `ImportLookupTableRVA` may be zero, especially for install files.
     * Switch to `ImportAddressTableRVA` if so, and check if IAT is bound
     * (contains AVAs) by seeing if TimeDateStamp == -1 (don't ask).
     * Nothing to parse if ImportLookupTableRVA is zero and IAT is bound.
     */
    uint32_t iltRva = *(uint32_t*)getFieldPtr(ILT);
    uint32_t iatRva = *(uint32_t*)getFieldPtr(IAT);
    if (iltRva == 0) {
        if (*(int32_t*)getFieldPtr(TIMESTAMP) == -1)
            return; // it's bound, do nothing
        else
            iltRva = *(uint32_t*)getFieldPtr(IAT);
    }

    iltRva = Convert::convertAddr(pe, iltRva, Convert::RVA, Convert::RAW);
    iatRva = Convert::convertAddr(pe, iatRva, Convert::RVA, Convert::RAW);

    if (Ident::is32bit(pe)) {
        readThunks<IMAGE_THUNK_DATA32>(fbytes, iltRva);
        readAddresses<uint32_t>(iatRva);
    } else {
        readThunks<IMAGE_THUNK_DATA64>(fbytes, iltRva);
        readAddresses<uint64_t>(iatRva);
    }
}

/* Template specialized constructors.
 * These constructors call a separate function since
 * the implementations are practically the same.
 */
template<>
GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::GenericImportDescriptor(const PeFile &pe, const FileBytes &fbytes, const size_t raw)
: IHeader(fbytes, raw)
{
    makeDescriptor<IMPORT_LOOKUP_TABLE_RVA, IMPORT_ADDRESS_TABLE_RVA, TIMESTAMP>(pe, fbytes);
}

template<>
GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>::GenericImportDescriptor(const PeFile &pe, const FileBytes &fbytes, const size_t raw)
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
        case HINT: return &hintname()->Hint;
        case NAME: return &hintname()->Name;
        default  : return nullptr;
    }
}

template <typename T> // requires variant declaration in header to link
const char* ImportThunk<T>::getFieldName(const int index)
{
    switch (index) {
        case HINTNAMERVA: return "Pointer to Hint/Name Table";
        default         : return "<UNKNOWN>";
    }
}

template <typename T> // requires variant declaration in header to link
const void* ImportThunk<T>::getFieldPtr(const int index) const
{
    switch (index) {
        case HINTNAMERVA: return &thunk()->HintNameTableRVA;
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
        case IMPORT_LOOKUP_TABLE_RVA : return &descriptor()->ImportLookupTableRVA;
        case TIMESTAMP               : return &descriptor()->TimeDateStamp;
        case FORWARDER_CHAIN         : return &descriptor()->ForwarderChain;
        case NAME_RVA                : return &descriptor()->NameRVA;
        case IMPORT_ADDRESS_TABLE_RVA: return &descriptor()->ImportAddressTableRVA;
        default                      : return nullptr;
    }
}

template<>
const char* GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::dllName() const
{
    uint32_t nameRva = *(uint32_t*)getFieldPtr(NAME_RVA);
    return &mem()[nameRva - *s_pDiskToMemDiff];
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
        case ATTRIBUTES                    : return &descriptor()->Attributes;
        case NAME_RVA                      : return &descriptor()->NameRVA;
        case MODULE_HANDLE_RVA             : return &descriptor()->ModuleHandleRVA;
        case DELAY_IMPORT_ADDRESS_TABLE_RVA: return &descriptor()->DelayImportAddressTableRVA;
        case DELAY_IMPORT_NAME_TABLE_RVA   : return &descriptor()->DelayImportNameTableRVA;
        case BOUND_DELAY_IMPORT_TABLE_RVA  : return &descriptor()->BoundDelayImportTableRVA;
        case UNLOAD_DELAY_IMPORT_TABLE_RVA : return &descriptor()->UnloadDelayImportTableRVA;
        case TIMESTAMP                     : return &descriptor()->Timestamp;
        default                            : return nullptr;
    }
}

template<>
const char* GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>::dllName() const
{
    uint32_t nameRva = *(uint32_t*)getFieldPtr(NAME_RVA);
    return &mem()[nameRva - *s_pDiskToMemDiff];
}

