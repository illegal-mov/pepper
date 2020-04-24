#include "GlobalPointerDir.h"

#include "../Identification.h"
#include "DataDirectoryEntry.h"

using namespace Pepper;

GlobalPointerDir::GlobalPointerDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {

    }
}

const char* GlobalPointerDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* GlobalPointerDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

