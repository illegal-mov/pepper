#include <pe_structures/DataDirectoryEntry.h>
#include <pe_structures/clr/CodeManager.h>

using namespace Pepper;

ClrCodeManager::ClrCodeManager(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{}

const char* ClrCodeManager::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ClrCodeManager::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

