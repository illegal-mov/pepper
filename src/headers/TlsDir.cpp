#include "../Identification.h"
#include "../PeFile.h"
#include "DataDirectoryEntry.h"
#include "OptionalHdr.h"
#include "TlsDir.h"

using namespace Pepper;

template <typename T>
int CallbacksTable<T>::s_codeDiff = 0;

template <typename T>
CallbacksTable<T>::CallbacksTable(const PeFile &pe, const FileBytes &fbytes, int raw)
: IHeader(fbytes, raw)
{
    const T *cbArray = callbacks();
    while (cbArray[m_length] != 0)
        m_length++;

    T codeRva = cbArray[0]; // AVAs in the table point to .text
    codeRva = Convert::convertAddr(pe, codeRva, Convert::AVA, Convert::RVA);
    s_codeDiff = Convert::getRvaToRawDiff(pe, codeRva);

    // Since these are AVAs, add ImageBase to
    //  s_codeDiff to more easily convert to RAW
    OptionalHeader *poh = (OptionalHeader*)pe.getHeaderPtr(PeFile::OPTIONAL);
    s_codeDiff += poh->imageBase();
}

TlsDir::TlsDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
, m_callbacks32()
{
    if (Ident::dirExists(*this)) {
        // get AVA to array of callback function pointers
        int64_t callbacksPtr = (Ident::is32bit(pe))
                              ? tls32()->AddressOfCallbacks
                              : tls64()->AddressOfCallbacks;
        // convert AVA to RAW, use RAW to construct table
        callbacksPtr = Convert::convertAddr(pe, callbacksPtr, Convert::AVA, Convert::RAW);
        if (Ident::is32bit(pe))
            m_callbacks32 = CallbacksTable<int32_t>(pe, fbytes, callbacksPtr);
        else
            m_callbacks64 = CallbacksTable<int64_t>(pe, fbytes, callbacksPtr);
    }
}

template <typename T>
const char* CallbacksTable<T>::getFieldName(int index)
{
    switch (index) {
        default: return "Callback Address";
    }
}

template <typename T>
const void* CallbacksTable<T>::getFieldPtr(int index) const
{
    return (0 <= index && index < length())
        ? &callbacks()[index]
        : nullptr;
}

const char* TlsDir::getFieldName(int index)
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

const void* TlsDir::getFieldPtr(int index) const
{
    bool is32bit = Ident::is32bit(*m_pe);
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

