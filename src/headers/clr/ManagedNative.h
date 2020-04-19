#ifndef CLR_MANAGEDNATIVE_H
#define CLR_MANAGEDNATIVE_H

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
class ClrNativeHeader final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ClrNativeHeader() = default;

    ClrNativeHeader(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* head() const { return static_cast<const void*>(dir()); }
    const void* getFieldPtr(const int index) const override;

    // static functions
    static const char* getFieldName(const int index);
};
}

#endif
