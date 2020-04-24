#include "ArchitectureDir.h"

#include "../Identification.h"
#include "DataDirectoryEntry.h"

using namespace Pepper;

ArchitectureDir::ArchitectureDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {

    }
}

const char* ArchitectureDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ArchitectureDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

