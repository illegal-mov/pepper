#include "../Identification.h"
#include "ArchitectureDir.h"
#include "DataDirectoryEntry.h"

using namespace Pepper;

ArchitectureDir::ArchitectureDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {

    }
}

const char* ArchitectureDir::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ArchitectureDir::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

