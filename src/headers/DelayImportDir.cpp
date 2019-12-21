#include <cstring>

#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "DelayImportDir.h"

using namespace Pepper;

int DelayImportDir::s_diskToMemDiff = 0;
template<> int *DelayImportDescriptor::s_pDiskToMemDiff = &DelayImportDir::s_diskToMemDiff;

DelayImportDir::DelayImportDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        s_diskToMemDiff = m_diffOfRvaRaw;
        // append non-null IMPORT_DESCRIPTOR to m_descriptors vector
        char null[sizeof(IMAGE_DELAY_IMPORT_DESCRIPTOR)] = {0};
        IMAGE_DELAY_IMPORT_DESCRIPTOR *pid = (PIMAGE_DELAY_IMPORT_DESCRIPTOR)dir();
        int i=0;
        while (memcmp(&pid[i], &null, sizeof(null))) {
            m_descriptors.emplace_back(pe, fbytes, dirOffset() + (i * (int)sizeof(null)));
            i++;
        }
    }
}

const char* DelayImportDir::getFieldName(int index)
{
    switch (index) {
        default: return "Delay Import Descriptor";
    }
}

const void* DelayImportDir::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

