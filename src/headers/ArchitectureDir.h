#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include "../generics/iDir.h"
#include "struct.h"

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

class ArchitectureDir final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ArchitectureDir()
    : IDirectory()
    {}

    ArchitectureDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* getFieldPtr(int index) const override;
//  const IMAGE_ARCHITECTURE_DIRECTORY* arch() const { return (PIMAGE_ARCHITECTURE_DIRECTORY)dir(); }

    // static functions
    static const char* getFieldName(int index);
};
} // namespace Pepper

#endif
