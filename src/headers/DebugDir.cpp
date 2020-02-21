#include <cstring>

#include "../Types.h"
#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "DebugDir.h"

using namespace Pepper;

DebugEntry::DebugEntry(const FileBytes &fbytes, const offset_t raw)
: IHeader(fbytes, raw)
, m_dbgRsds()
{
    m_dbgRsds = DebugRsds(fbytes, dbg()->PointerToRawData);
}

DebugDir::DebugDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        constexpr char null[sizeof(IMAGE_DEBUG_DIRECTORY)] = {0};
        for (size_t i=0; memcmp(&static_cast<const char*>(dir())[i*sizeof(null)], &null, sizeof(null)); i++) {
            m_elements.emplace_back(fbytes, dirOffset() + i*sizeof(null));
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
        case SIGNATURE: return &rsds()->Signature;
        case GUID     : return &rsds()->Guid;
        case AGE      : return &rsds()->Age;
        case PDB      : return &rsds()->Pdb;
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
        case CHARACTERISTICS    : return &dbg()->Characteristics;
        case TIMEDATESTAMP      : return &dbg()->TimeDateStamp;
        case MAJOR_VERSION      : return &dbg()->MajorVersion;
        case MINOR_VERSION      : return &dbg()->MinorVersion;
        case TYPE               : return &dbg()->Type;
        case SIZE_OF_DATA       : return &dbg()->SizeOfData;
        case ADDRESS_OF_RAW_DATA: return &dbg()->AddressOfRawData;
        case POINTER_TO_RAW_DATA: return &dbg()->PointerToRawData;
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

