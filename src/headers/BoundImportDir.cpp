#include "BoundImportDir.h"

#include "../Identification.h"
#include "DataDirectoryEntry.h"

using namespace Pepper;

BoundImportDir::BoundImportDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        /* This directory doesn't make any sense */
    }
}

const char* BoundImportDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* BoundImportDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

