#include <cstring>

#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "ImportDir.h"

using namespace Pepper;

// each class related to regular imports must see the same `diskToMemDiff` value
int ImportDir::s_diskToMemDiff = 0;
template<>
int *GenericImportDescriptor<IMAGE_IMPORT_DESCRIPTOR>::s_pDiskToMemDiff = &ImportDir::s_diskToMemDiff;
template<>
int *ImportThunk<IMAGE_THUNK_DATA32>::s_pDiskToMemDiff = &ImportDir::s_diskToMemDiff;
template<>
int *ImportThunk<IMAGE_THUNK_DATA64>::s_pDiskToMemDiff = &ImportDir::s_diskToMemDiff;

ImportDir::ImportDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        s_diskToMemDiff = m_diffOfRvaRaw;
        // append non-null IMPORT_DESCRIPTOR to m_descriptors vector
        char null[sizeof(IMAGE_IMPORT_DESCRIPTOR)] = {0};
        IMAGE_IMPORT_DESCRIPTOR *pid = (PIMAGE_IMPORT_DESCRIPTOR)dir();
        int i=0;
        while (memcmp(&pid[i], &null, sizeof(null))) {
            m_descriptors.emplace_back(pe, fbytes, dirOffset() + (i * (int)sizeof(null)));
            i++;
        }
    }
}

const char* ImportDir::getFieldName(int index)
{
    switch (index) {
        default: return "Import Descriptor";
    }
}

const void* ImportDir::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

