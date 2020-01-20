#include "../DataDirectoryEntry.h"
#include "ExportJump.h"

using namespace Pepper;

ClrExportJump::ClrExportJump(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{}

const char* ClrExportJump::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ClrExportJump::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

