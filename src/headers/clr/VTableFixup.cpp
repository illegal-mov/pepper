#include "../DataDirectoryEntry.h"
#include "VTableFixup.h"

using namespace Pepper;

ClrVTableFixup::ClrVTableFixup(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{}

const char* ClrVTableFixup::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ClrVTableFixup::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

