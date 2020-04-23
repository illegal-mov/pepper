#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include <vector>

#include "../Types.h"
#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_ATTRIBUTE_CERTIFICATE {
    uint32_t Length;            // certificate size in bytes
    int16_t  Revision;          // certificate version
    int16_t CertificateType;    // certificate type
    char    CertificateBytes[]; // certificate data
} IMAGE_ATTRIBUTE_CERTIFICATE, *PIMAGE_ATTRIBUTE_CERTIFICATE;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

class CertificateEntry final : public IHeader {
public:
    enum Fields {
        LENGTH,
        REVISION,
        CERTIFICATE_TYPE,
        CERTIFICATE_BYTES,
        _NUM_FIELDS,
    };

    CertificateEntry(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    {}

    // member functions
    const IMAGE_ATTRIBUTE_CERTIFICATE* getStructPtr() const { return static_cast<const IMAGE_ATTRIBUTE_CERTIFICATE*>(hdr()); }
    const void* getFieldPtr(const int index) const override;
    const char* bytes() const { return getStructPtr()->CertificateBytes; }
    uint32_t size() const { return getStructPtr()->Length; }

    // static functions
    static const char* getFieldName(const int index);
};

class CertificateDir final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    CertificateDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    // member functions
    const void* getFieldPtr(const int index) const override;
    const std::vector<CertificateEntry>& certs() const { return m_certificates; }
    size_t length() const { return m_certificates.size(); }

    // static functions
    static const char* getFieldName(const int index);

private:
    std::vector<CertificateEntry> m_certificates{};
};
} // namespace Pepper

#endif
