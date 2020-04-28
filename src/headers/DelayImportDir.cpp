#include "DelayImportDir.h"

#include <cstring>

#include "../Identification.h"
#include "DataDirectoryEntry.h"

using namespace Pepper;

size_t DelayImportDir::s_diskToMemDiff = 0;
template<> size_t *DelayImportDescriptor::s_pDiskToMemDiff = &DelayImportDir::s_diskToMemDiff;

DelayImportDir::DelayImportDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        s_diskToMemDiff = m_diskToMemoryDifference;
        constexpr char null[sizeof(IMAGE_DELAY_IMPORT_DESCRIPTOR)] = {0};
        const IMAGE_DELAY_IMPORT_DESCRIPTOR *pid = (PIMAGE_DELAY_IMPORT_DESCRIPTOR)dir();
        size_t i = 0;
        while (memcmp(&pid[i], &null, sizeof(null))) {
            m_descriptors.emplace_back(pe, fbytes, dirOffset() + (i * sizeof(null)));
            i++;
        }
    }
}

const char* DelayImportDir::getFieldName(const int index)
{
    switch (index) {
        default: return "Delay Import Descriptor";
    }
}

const void* DelayImportDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

