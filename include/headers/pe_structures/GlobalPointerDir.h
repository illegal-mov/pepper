#ifndef GLOBALPOINTER_H
#define GLOBALPOINTER_H

#include <generics/iDir.h>

namespace Pepper {

class PeFile;
class FileBytes;
class DataDirectoryEntry;

class GlobalPointerDir final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    GlobalPointerDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    const void* getFieldPtr(const int index) const override;
//  const IMAGE_GLOBAL_POINTER_DIRECTORY* getStructPtr() const { return static_cast<const IMAGE_GLOBAL_POINTER_DIRECTORY*>(dir()); }

    static const char* getFieldName(const int index);
};
} // namespace Pepper

#endif
