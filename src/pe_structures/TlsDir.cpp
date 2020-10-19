#include "../../include/headers/pe_structures/TlsDir.h"

#include "../../include/headers/Conversion.h"
#include "../../include/headers/Identification.h"
#include "../../include/headers/PeFile.h"
#include "../../include/headers/pe_structures/OptionalHdr.h"

using namespace Pepper;

template <typename ArchType>
size_t CallbacksTable<ArchType>::s_codeDiff = 0;

template <typename ArchType>
CallbacksTable<ArchType>::CallbacksTable(const PeFile& pe, const FileBytes& fbytes, const offset_t raw)
: IHeader(fbytes, raw)
{
    const ArchType* cbArray = getStructPtr();
    while (cbArray[m_length] != 0) {
        ++m_length;
    }

    ArchType codeRva = cbArray[0]; // AVAs in the table point to .text
    codeRva = Convert::convertAddr(pe, codeRva, Convert::AddrType::AVA, Convert::AddrType::RVA);
    s_codeDiff = Convert::getRvaToRawDiff(pe, codeRva);

    // Since these are AVAs, add ImageBase to
    //  s_codeDiff to more easily convert to RAW
    const OptionalHeader& poh = pe.optionalHdr();
    s_codeDiff += poh.imageBase();
}

TlsDir::TlsDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
, m_callbacks32()
{
    if (Ident::dirExists(*this)) {
        const bool is32bit = Ident::is32bit(pe);
        addr_t callbacksAddress = (is32bit)
                                ? getStructPtr32()->AddressOfCallbacks
                                : getStructPtr64()->AddressOfCallbacks;

        callbacksAddress = Convert::convertAddr(pe, callbacksAddress, Convert::AddrType::AVA, Convert::AddrType::RAW);
        if (is32bit) {
            m_callbacks32 = CallbacksTable<ptr32_t>(pe, fbytes, callbacksAddress);
        } else {
            m_callbacks64 = CallbacksTable<ptr64_t>(pe, fbytes, callbacksAddress);
        }
    }
}

template <typename ArchType>
const char* CallbacksTable<ArchType>::getFieldName(const int index)
{
    switch (index) {
        default: return "Callback Address";
    }
}

template <typename ArchType>
const void* CallbacksTable<ArchType>::getFieldPtr(const int index) const
{
    const size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &getStructPtr()[uindex]
        : nullptr;
}

const char* TlsDir::getFieldName(const int index)
{
    switch (index) {
        case RAW_DATA_START_VA   : return "Raw Data Start Address";
        case RAW_DATA_END_VA     : return "Raw Data End Address";
        case ADDRESS_OF_INDEX    : return "Address of Index";
        case ADDRESS_OF_CALLBACKS: return "Address of Callbacks";
        case SIZE_OF_ZERO_FILL   : return "Zero Fill Size";
        case CHARACTERISTICS     : return "Characteristics";
        default                  : return "<UNKNOWN>";
    }
}

const void* TlsDir::getFieldPtr(const int index) const
{
    const bool is32bit = Ident::is32bit(*m_peFile);
    switch (index) {
        case RAW_DATA_START_VA   : return (is32bit) ? (void*)&getStructPtr32()->RawDataStartVA     : (void*)&getStructPtr64()->RawDataStartVA;
        case RAW_DATA_END_VA     : return (is32bit) ? (void*)&getStructPtr32()->RawDataEndVA       : (void*)&getStructPtr64()->RawDataEndVA;
        case ADDRESS_OF_INDEX    : return (is32bit) ? (void*)&getStructPtr32()->AddressOfIndex     : (void*)&getStructPtr64()->AddressOfIndex;
        case ADDRESS_OF_CALLBACKS: return (is32bit) ? (void*)&getStructPtr32()->AddressOfCallbacks : (void*)&getStructPtr64()->AddressOfCallbacks;
        case SIZE_OF_ZERO_FILL   : return (is32bit) ? (void*)&getStructPtr32()->SizeOfZeroFill     : (void*)&getStructPtr64()->SizeOfZeroFill;
        case CHARACTERISTICS     : return (is32bit) ? (void*)&getStructPtr32()->Characteristics    : (void*)&getStructPtr64()->Characteristics;
        default                  : return nullptr;
    }
}

