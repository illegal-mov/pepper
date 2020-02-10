#include "../Identification.h"
#include "../PeFile.h"
#include "DataDirectoryEntry.h"
#include "OptionalHdr.h"
#include "TlsDir.h"

using namespace Pepper;

template <typename ArchType>
size_t CallbacksTable<ArchType>::s_codeDiff = 0;

template <typename ArchType>
CallbacksTable<ArchType>::CallbacksTable(const PeFile &pe, const FileBytes &fbytes, const size_t raw)
: IHeader(fbytes, raw)
{
    const ArchType *cbArray = callbacks();
    while (cbArray[m_length] != 0) {
        m_length++;
    }

    ArchType codeRva = cbArray[0]; // AVAs in the table point to .text
    codeRva = Convert::convertAddr(pe, codeRva, Convert::AVA, Convert::RVA);
    s_codeDiff = Convert::getRvaToRawDiff(pe, codeRva);

    // Since these are AVAs, add ImageBase to
    //  s_codeDiff to more easily convert to RAW
    const OptionalHeader &poh = pe.optionalHdr();
    s_codeDiff += poh.imageBase();
}

TlsDir::TlsDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
, m_callbacks32()
{
    if (Ident::dirExists(*this)) {
        // get AVA to array of callback function pointers
        const bool is32bit = Ident::is32bit(pe);
        uint64_t callbacksPtr = (is32bit)
                              ? tls32()->AddressOfCallbacks
                              : tls64()->AddressOfCallbacks;
        // convert AVA to RAW, use RAW to construct table
        callbacksPtr = Convert::convertAddr(pe, callbacksPtr, Convert::AVA, Convert::RAW);
        if (is32bit) {
            m_callbacks32 = CallbacksTable<uint32_t>(pe, fbytes, callbacksPtr);
        } else {
            m_callbacks64 = CallbacksTable<uint64_t>(pe, fbytes, callbacksPtr);
        }
    }
}

template <typename ArchType> // requires variant declaration in header to link
const char* CallbacksTable<ArchType>::getFieldName(const int index)
{
    switch (index) {
        default: return "Callback Address";
    }
}

template <typename ArchType> // requires variant declaration in header to link
const void* CallbacksTable<ArchType>::getFieldPtr(const int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &callbacks()[uindex]
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
    const bool is32bit = Ident::is32bit(*m_pe);
    switch (index) {
        case RAW_DATA_START_VA   : return (is32bit) ? (void*)&tls32()->RawDataStartVA     : (void*)&tls64()->RawDataStartVA;
        case RAW_DATA_END_VA     : return (is32bit) ? (void*)&tls32()->RawDataEndVA       : (void*)&tls64()->RawDataEndVA;
        case ADDRESS_OF_INDEX    : return (is32bit) ? (void*)&tls32()->AddressOfIndex     : (void*)&tls64()->AddressOfIndex;
        case ADDRESS_OF_CALLBACKS: return (is32bit) ? (void*)&tls32()->AddressOfCallbacks : (void*)&tls64()->AddressOfCallbacks;
        case SIZE_OF_ZERO_FILL   : return (is32bit) ? (void*)&tls32()->SizeOfZeroFill     : (void*)&tls64()->SizeOfZeroFill;
        case CHARACTERISTICS     : return (is32bit) ? (void*)&tls32()->Characteristics    : (void*)&tls64()->Characteristics;
        default                  : return nullptr;
    }
}

