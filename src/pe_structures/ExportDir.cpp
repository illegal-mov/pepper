#include "../../include/headers/pe_structures/ExportDir.h"

#include "../../include/headers/Conversion.h"
#include "../../include/headers/Identification.h"
#include "../../include/headers/Types.h"
#include "../../include/headers/pe_structures/DataDirectoryEntry.h"

using namespace Pepper;

size_t ExportAddressTable::s_diskToMemDiff = 0;
size_t ExportNameTable::s_diskToMemDiff = 0;

ExportAddressTable::ExportAddressTable(const PeFile& pe, const FileBytes& fbytes, const offset_t raw, const size_t len)
: IHeader(fbytes, raw)
, m_length(len)
{
    // RVAs in .edata point to .text
    s_diskToMemDiff = Convert::getRvaToRawDiff(pe, getStructPtr()[0]);
}

ExportNameTable::ExportNameTable(const PeFile& pe, const FileBytes& fbytes, const offset_t raw, const size_t len)
: IHeader(fbytes, raw)
, m_length(len)
{
    s_diskToMemDiff = Convert::getRvaToRawDiff(pe, getStructPtr()[0]);
}

ExportOrdinalTable::ExportOrdinalTable(const FileBytes& fbytes, const offset_t raw, const size_t len)
: IHeader(fbytes, raw)
, m_length(len)
{}

ExportDir::ExportDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
, m_addrTable()
, m_nameTable()
, m_ordTable()
{
    if (Ident::dirExists(*this)) {
        const IMAGE_EXPORT_DIRECTORY* pExpDir = getStructPtr();

        if (getStructPtr()->AddressTableEntries > 0) {
            ExportAddressTable eat(pe, fbytes, pExpDir->ExportAddressTableRVA - m_diskToMemoryDifference,
                pExpDir->AddressTableEntries);
            m_addrTable = eat;
        }

        if (getStructPtr()->NumberOfNamePointers > 0) {
            ExportNameTable ent(pe, fbytes, pExpDir->NamePointerRVA - m_diskToMemoryDifference,
                pExpDir->NumberOfNamePointers);
            m_nameTable = ent;

            ExportOrdinalTable eot(fbytes, pExpDir->OrdinalTableRVA - m_diskToMemoryDifference,
                pExpDir->NumberOfNamePointers);
            m_ordTable = eot;
        }
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
        ? &getStructPtr()[uindex]
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
        ? &getStructPtr()[uindex]
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
        ? &getStructPtr()[uindex]
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
        case EXPORT_FLAGS            : return &getStructPtr()->ExportFlags;
        case TIMESTAMP               : return &getStructPtr()->TimeDateStamp;
        case MAJOR_VERSION           : return &getStructPtr()->MajorVersion;
        case MINOR_VERSION           : return &getStructPtr()->MinorVersion;
        case NAME_RVA                : return &getStructPtr()->NameRVA;
        case ORDINAL_BASE            : return &getStructPtr()->OrdinalBase;
        case ADDRESS_TABLE_ENTRIES   : return &getStructPtr()->AddressTableEntries;
        case NUMBER_OF_NAME_POINTERS : return &getStructPtr()->NumberOfNamePointers;
        case EXPORT_ADDRESS_TABLE_RVA: return &getStructPtr()->ExportAddressTableRVA;
        case NAME_POINTER_RVA        : return &getStructPtr()->NamePointerRVA;
        case ORDINAL_TABLE_RVA       : return &getStructPtr()->OrdinalTableRVA;
        default                      : return nullptr;
    }
}

