#include "../Conversion.h"
#include "../Types.h"
#include "DataDirectoryEntry.h"
#include "ExportDir.h"

using namespace Pepper;

size_t ExportAddressTable::s_diskToMemDiff = 0;
size_t ExportNameTable::s_diskToMemDiff = 0;

ExportAddressTable::ExportAddressTable(const PeFile &pe, const FileBytes &fbytes, const offset_t raw, const size_t len)
: IHeader(fbytes, raw)
, m_length(len)
{
    // RVAs in .edata point to .text
    s_diskToMemDiff = Convert::getRvaToRawDiff(pe, addresses()[0]);
}

ExportNameTable::ExportNameTable(const PeFile &pe, const FileBytes &fbytes, const offset_t raw, const size_t len)
: IHeader(fbytes, raw)
, m_length(len)
{
    s_diskToMemDiff = Convert::getRvaToRawDiff(pe, addresses()[0]);
}

ExportOrdinalTable::ExportOrdinalTable(const FileBytes &fbytes, const offset_t raw, const size_t len)
: IHeader(fbytes, raw)
, m_length(len)
{}

ExportDir::ExportDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
, m_addrTable()
, m_nameTable()
, m_ordTable()
{
    if (Ident::dirExists(*this)) {
        const IMAGE_EXPORT_DIRECTORY *pExpDir = xport();

        // construct each of the three export tables by copy assignment
        ExportAddressTable eat(pe, fbytes, pExpDir->ExportAddressTableRVA - m_diffOfRvaRaw,
            pExpDir->AddressTableEntries);
        m_addrTable = eat;

        ExportNameTable ent(pe, fbytes, pExpDir->NamePointerRVA - m_diffOfRvaRaw,
            pExpDir->NumberOfNamePointers);
        m_nameTable = ent;

        ExportOrdinalTable eot(fbytes, pExpDir->OrdinalTableRVA - m_diffOfRvaRaw,
            pExpDir->NumberOfNamePointers);
        m_ordTable = eot;
    }
}

const char* ExportAddressTable::getFieldName(const int index)
{
    switch (index) {
        default : return "Exported Function Address";
    }
}

const void* ExportAddressTable::getFieldPtr(const int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &addresses()[uindex]
        : nullptr;
}

const char* ExportNameTable::getFieldName(const int index)
{
    switch (index) {
        default : return "Exported Function Name";
    }
}

const void* ExportNameTable::getFieldPtr(const int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &addresses()[uindex]
        : nullptr;
}

const char* ExportOrdinalTable::getFieldName(const int index)
{
    switch (index) {
        default : return "Exported Function Ordinal";
    }
}

const void* ExportOrdinalTable::getFieldPtr(const int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &ordinals()[uindex]
        : nullptr;
}

const char* ExportDir::getFieldName(const int index)
{
    switch (index) {
        case EXPORT_FLAGS            : return "Export Flags";
        case TIMESTAMP               : return "Timestamp";
        case MAJOR_VERSION           : return "Major Version";
        case MINOR_VERSION           : return "Minor Version";
        case NAME_RVA                : return "Pointer to DLL Name";
        case ORDINAL_BASE            : return "Ordinal Starting Index";
        case ADDRESS_TABLE_ENTRIES   : return "Export Table Length";
        case NUMBER_OF_NAME_POINTERS : return "Ordinal Table Length";
        case EXPORT_ADDRESS_TABLE_RVA: return "Pointer to Export Table";
        case NAME_POINTER_RVA        : return "Pointer to Name Table";
        case ORDINAL_TABLE_RVA       : return "Pointer to Ordinal Table";
        default                      : return "<UNKNOWN>";
    }
}

const void* ExportDir::getFieldPtr(const int index) const
{
    switch (index) {
        case EXPORT_FLAGS            : return &xport()->ExportFlags;
        case TIMESTAMP               : return &xport()->TimeDateStamp;
        case MAJOR_VERSION           : return &xport()->MajorVersion;
        case MINOR_VERSION           : return &xport()->MinorVersion;
        case NAME_RVA                : return &xport()->NameRVA;
        case ORDINAL_BASE            : return &xport()->OrdinalBase;
        case ADDRESS_TABLE_ENTRIES   : return &xport()->AddressTableEntries;
        case NUMBER_OF_NAME_POINTERS : return &xport()->NumberOfNamePointers;
        case EXPORT_ADDRESS_TABLE_RVA: return &xport()->ExportAddressTableRVA;
        case NAME_POINTER_RVA        : return &xport()->NamePointerRVA;
        case ORDINAL_TABLE_RVA       : return &xport()->OrdinalTableRVA;
        default                      : return nullptr;
    }
}

