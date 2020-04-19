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
    const IMAGE_ATTRIBUTE_CERTIFICATE* cert() const { return (PIMAGE_ATTRIBUTE_CERTIFICATE)hdr(); }
    const void* getFieldPtr(const int index) const override;
    const char* bytes() const { return cert()->CertificateBytes; }
    uint32_t size() const { return cert()->Length; }

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
    const std::vector<CertificateEntry>& certs() const { return m_elements; }
    size_t length() const { return m_elements.size(); }

    // static functions
    static const char* getFieldName(const int index);

private:
    std::vector<CertificateEntry> m_elements{};
};
} // namespace Pepper

#endif
