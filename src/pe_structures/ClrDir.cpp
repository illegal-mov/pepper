#include <pe_structures/ClrDir.h>

#include <Identification.h>

using namespace Pepper;

ClrDir::ClrDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        m_directoryEntries[CLR_METADATA] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, MetaData));
        if (m_directoryEntries[CLR_METADATA].rva() > 0 && m_directoryEntries[CLR_METADATA].size() > 0) {
            m_Meta = std::make_unique<ClrMetadata>(pe, fbytes, m_directoryEntries[CLR_METADATA]);
        }

        m_directoryEntries[CLR_RESOURCES] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, Resources));
        if (m_directoryEntries[CLR_RESOURCES].rva() > 0 && m_directoryEntries[CLR_RESOURCES].size() > 0) {
            m_Rsrc = std::make_unique<ClrResource>(pe, fbytes, m_directoryEntries[CLR_RESOURCES]);
        }

        m_directoryEntries[CLR_SIGNATURE] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, StrongNameSignature));
        if (m_directoryEntries[CLR_SIGNATURE].rva() > 0 && m_directoryEntries[CLR_SIGNATURE].size() > 0) {
            m_Signtr = std::make_unique<ClrSignature>(pe, fbytes, m_directoryEntries[CLR_SIGNATURE]);
        }

        m_directoryEntries[CLR_CODE_MANAGER] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, CodeManagerTable));
        if (m_directoryEntries[CLR_CODE_MANAGER].rva() > 0 && m_directoryEntries[CLR_CODE_MANAGER].size() > 0) {
            m_CodeMan = std::make_unique<ClrCodeManager>(pe, fbytes, m_directoryEntries[CLR_CODE_MANAGER]);
        }

        m_directoryEntries[CLR_VTABLE_FIXUPS] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, VTableFixups));
        if (m_directoryEntries[CLR_VTABLE_FIXUPS].rva() > 0 && m_directoryEntries[CLR_VTABLE_FIXUPS].size() > 0) {
            m_VTabFix = std::make_unique<ClrVTableFixup>(pe, fbytes, m_directoryEntries[CLR_VTABLE_FIXUPS]);
        }

        m_directoryEntries[CLR_EXPORT_JUMP] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, ExportAddressTableJumps));
        if (m_directoryEntries[CLR_EXPORT_JUMP].rva() > 0 && m_directoryEntries[CLR_EXPORT_JUMP].size() > 0) {
            m_XprtJmp = std::make_unique<ClrExportJump>(pe, fbytes, m_directoryEntries[CLR_EXPORT_JUMP]);
        }

        m_directoryEntries[CLR_NATIVE_HEADER] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, ManagedNativeHeader));
        if (m_directoryEntries[CLR_NATIVE_HEADER].rva() > 0 && m_directoryEntries[CLR_NATIVE_HEADER].size() > 0) {
            m_NatHead = std::make_unique<ClrNativeHeader>(pe, fbytes, m_directoryEntries[CLR_NATIVE_HEADER]);
        }
    }
}

const char* ClrDir::getHeaderName(const int index)
{
    switch (index) {
        case CLR_METADATA     : return "Metadata";
        case CLR_RESOURCES    : return "Resources";
        case CLR_SIGNATURE    : return "Strong Name Signature";
        case CLR_CODE_MANAGER : return "Code Manager Table";
        case CLR_VTABLE_FIXUPS: return "VTable Fixups";
        case CLR_EXPORT_JUMP  : return "Export Address Table Jumps";
        case CLR_NATIVE_HEADER: return "Managed Native Header";
        default               : return "<UNKNOWN>";
    }
}

const char* ClrDir::getFieldName(const int index)
{
    switch (index) {
        case SIZE                 : return "Size";
        case MAJOR_RUNTIME_VERSION: return "Major Runtime Version";
        case MINOR_RUNTIME_VERSION: return "Minor Runtime Version";
        case FLAGS                : return "Flags";
        case ENTRY_POINT_RVA      : return "EntryPoint Token or EntryPoint RVA";
        default                   : return "<UNKNOWN>";
    }
}

const void* ClrDir::getFieldPtr(const int index) const
{
    switch (index) {
        case SIZE                 : return &getStructPtr()->Size;
        case MAJOR_RUNTIME_VERSION: return &getStructPtr()->MajorRuntimeVersion;
        case MINOR_RUNTIME_VERSION: return &getStructPtr()->MinorRuntimeVersion;
        case FLAGS                : return &getStructPtr()->Flags;
        case ENTRY_POINT_RVA      : return &getStructPtr()->EntryPointRVA;
        default                   : return nullptr;
    }
}

