#ifndef CLR_CODEMANAGER_H
#define CLR_CODEMANAGER_H

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
class ClrCodeManager final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ClrCodeManager() = default;

    ClrCodeManager(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* codeman() const { return static_cast<const void*>(dir()); }
    const void* getFieldPtr(const int index) const override;

    // static functions
    static const char* getFieldName(const int index);
};
}

#endif
