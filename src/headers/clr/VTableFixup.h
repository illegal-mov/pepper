#ifndef CLR_VTABLEFIXUP_H
#define CLR_VTABLEFIXUP_H

#include "../../generics/iDir.h"

/*
typedef struct _IMAGE_ {
} IMAGE_, *PIMAGE_;
*/

namespace Pepper {

/*
 */
class ClrVTableFixup final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ClrVTableFixup() = default;

    ClrVTableFixup(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    // member functions
    const void* vtable() const { return static_cast<const void*>(dir()); }
    const void* getFieldPtr(const int index) const override;

    // static functions
    static const char* getFieldName(const int index);
};
}

#endif
