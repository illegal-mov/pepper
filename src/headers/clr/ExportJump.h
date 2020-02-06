#ifndef CLR_EXPORTJUMP_H
#define CLR_EXPORTJUMP_H

#include "../../generics/iDir.h"

/*
typedef struct _IMAGE_ {
} IMAGE_, *PIMAGE_;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/*
 */
class ClrExportJump final : public IDirectory {
private:
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ClrExportJump()
    : IDirectory()
    {}

    ClrExportJump(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* jump() const { return static_cast<const void*>(dir()); }
    const void* getFieldPtr(const int index) const override;

    // static functions
    static const char* getFieldName(const int index);
};
}

#endif
