#ifndef CLR_SIGNATURE_H
#define CLR_SIGNATURE_H

#include <generics/iDir.h>

/*
typedef struct _IMAGE_ {
} IMAGE_, *PIMAGE_;
*/

namespace Pepper {

/* 128 bytes of random data. Probably a hash of some sort.
 */
class ClrSignature final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ClrSignature() = default;

    ClrSignature(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    const char* getStructPtr() const { return static_cast<const char*>(dir()); }
    const void* getFieldPtr(const int index) const override;
    size_t length() const { return m_length; }

    static const char* getFieldName(const int index);

private:
    size_t m_length{};
};
}

#endif
