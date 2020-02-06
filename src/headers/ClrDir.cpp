#include "../Identification.h"
#include "ClrDir.h"

using namespace Pepper;

ClrDir::ClrDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
, m_Meta()
, m_Rsrc()
, m_Signtr()
, m_CodeMan()
, m_VTabFix()
, m_XprtJmp()
, m_NatHead()
{
    if (Ident::dirExists(*this)) {
        m_directoryEntries[CLR_METADATA] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, MetaData));
        m_Meta = ClrMetadata(pe, fbytes, m_directoryEntries[CLR_METADATA]);

        m_directoryEntries[CLR_RESOURCES] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, Resources));
        m_Rsrc = ClrResource(pe, fbytes, m_directoryEntries[CLR_RESOURCES]);

        m_directoryEntries[CLR_SIGNATURE] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, StrongNameSignature));
        m_Signtr = ClrSignature(pe, fbytes, m_directoryEntries[CLR_SIGNATURE]);

        m_directoryEntries[CLR_CODE_MANAGER] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, CodeManagerTable));
        m_CodeMan = ClrCodeManager(pe, fbytes, m_directoryEntries[CLR_CODE_MANAGER]);

        m_directoryEntries[CLR_VTABLE_FIXUPS] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, VTableFixups));
        m_VTabFix = ClrVTableFixup(pe, fbytes, m_directoryEntries[CLR_VTABLE_FIXUPS]);

        m_directoryEntries[CLR_EXPORT_JUMP] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, ExportAddressTableJumps));
        m_XprtJmp = ClrExportJump(pe, fbytes, m_directoryEntries[CLR_EXPORT_JUMP]);

        m_directoryEntries[CLR_NATIVE_HEADER] = DataDirectoryEntry(fbytes,
            dirOffset() + offsetof(IMAGE_COR20_HEADER, ManagedNativeHeader));
        m_NatHead = ClrNativeHeader(pe, fbytes, m_directoryEntries[CLR_NATIVE_HEADER]);
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
        case SIZE                 : return &clr()->Size;
        case MAJOR_RUNTIME_VERSION: return &clr()->MajorRuntimeVersion;
        case MINOR_RUNTIME_VERSION: return &clr()->MinorRuntimeVersion;
        case FLAGS                : return &clr()->Flags;
        case ENTRY_POINT_RVA      : return &clr()->EntryPointRVA;
        default                   : return nullptr;
    }
}

