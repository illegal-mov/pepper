#ifndef CLR_SIGNATURE_H
#define CLR_SIGNATURE_H

#include "../../generics/iDir.h"

/*
typedef struct _IMAGE_ {
} IMAGE_, *PIMAGE_;
*/

namespace Pepper {

/* 128 bytes of random data. Probably a hash of some sort.
 */
class ClrSignature final : public IDirectory {
private:
    size_t m_length{};
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ClrSignature()
    : IDirectory()
    {}

    ClrSignature(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const char* sig() const { return (char*)dir(); }
    const void* getFieldPtr(int index) const override;
    size_t length() const { return m_length; }

    // static functions
    static const char* getFieldName(int index);
};
}

#endif
