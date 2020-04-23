#include <cstring>

#include "../Types.h"
#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "DebugDir.h"

using namespace Pepper;

DebugEntry::DebugEntry(const FileBytes& fbytes, const offset_t raw)
: IHeader(fbytes, raw)
, m_dbgRsds()
{
    m_dbgRsds = DebugRsds(fbytes, getStructPtr()->PointerToRawData);
}

DebugDir::DebugDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        const size_t len = dde.size() / sizeof(IMAGE_DEBUG_DIRECTORY);
        for (size_t i=0; i < len; i++) {
            m_debugEntries.emplace_back(fbytes, dirOffset() + i*sizeof(IMAGE_DEBUG_DIRECTORY));
        }
    }
}

const char* DebugRsds::getFieldName(const int index)
{
    switch (index) {
        case SIGNATURE: return "Signature";
        case GUID     : return "GUID";
        case AGE      : return "Age";
        case PDB      : return "PDB Path";
        default       : return "<UNKNOWN>";
    }
}

const void* DebugRsds::getFieldPtr(const int index) const
{
    switch (index) {
        case SIGNATURE: return &getStructPtr()->Signature;
        case GUID     : return &getStructPtr()->Guid;
        case AGE      : return &getStructPtr()->Age;
        case PDB      : return &getStructPtr()->Pdb;
        default       : return nullptr;
    }
}

const char* DebugEntry::getFieldName(const int index)
{
    switch (index) {
        case CHARACTERISTICS    : return "Characteristics";
        case TIMEDATESTAMP      : return "Timestamp";
        case MAJOR_VERSION      : return "Debug Major Version";
        case MINOR_VERSION      : return "Debug Minor Version";
        case TYPE               : return "Type";
        case SIZE_OF_DATA       : return "Size of Raw Data";
        case ADDRESS_OF_RAW_DATA: return "Address of Raw Data";
        case POINTER_TO_RAW_DATA: return "Pointer to Raw Data";
        default                 : return "<UNKNOWN>";
    }
}

const void* DebugEntry::getFieldPtr(const int index) const
{
    switch (index) {
        case CHARACTERISTICS    : return &getStructPtr()->Characteristics;
        case TIMEDATESTAMP      : return &getStructPtr()->TimeDateStamp;
        case MAJOR_VERSION      : return &getStructPtr()->MajorVersion;
        case MINOR_VERSION      : return &getStructPtr()->MinorVersion;
        case TYPE               : return &getStructPtr()->Type;
        case SIZE_OF_DATA       : return &getStructPtr()->SizeOfData;
        case ADDRESS_OF_RAW_DATA: return &getStructPtr()->AddressOfRawData;
        case POINTER_TO_RAW_DATA: return &getStructPtr()->PointerToRawData;
        default                 : return nullptr;
    }
}

const char* DebugEntry::getDebugTypeName(const int index)
{
    switch (index) {
        case UNKNOWN       : return "Unknown";
        case COFF          : return "COFF";
        case CODEVIEW      : return "Visual C++";
        case FPO           : return "Frame Pointer Omission";
        case MISC          : return "DBG File Location";
        case EXCEPTION     : return "Exception Section Copy";
        case FIXUP         : return "<Reserved>";
        case OMAP_TO_SRC   : return "Map to Source";
        case OMAP_FROM_SRC : return "Map from Source";
        case BORLAND       : return "<Reserved>";
        case RESERVED10    : return "<Reserved>";
        case CLSID         : return "<Reserved>";
        case REPRO         : return "Reproducible/Deterministic";
        case EX_DLL_CHARACT: return "Extended DLL Characteristics";
        default            : return "<UNKNOWN>";
    }
}

const char* DebugDir::getFieldName(const int index)
{
    switch (index) {
        default: return "Debug Table";
    }
}

const void* DebugDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

