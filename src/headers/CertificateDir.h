#ifndef CERTIFICATE_H
#define CERTIFICATE_H

#include <vector>

#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_ATTRIBUTE_CERTIFICATE {
    int32_t Length;             // certificate size in bytes
    int16_t Revision;           // certificate version
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

    CertificateEntry()
    : IHeader()
    {}

    CertificateEntry(const FileBytes &fbytes, int raw)
    : IHeader(fbytes, raw)
    {}

    // member functions
    const IMAGE_ATTRIBUTE_CERTIFICATE* cert() const { return (PIMAGE_ATTRIBUTE_CERTIFICATE)hdr(); }
    const void* getFieldPtr(int index) const override;
    const char* bytes() const { return cert()->CertificateBytes; }
    int32_t size() const { return cert()->Length; }

    // static functions
    static const char* getFieldName(int index);
};

class CertificateDir final : public IDirectory {
private:
    std::vector<CertificateEntry> m_elements{};
public:
    enum Fields {
        _NUM_FIELDS,
    };

    CertificateDir()
    : IDirectory()
    {}

    CertificateDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* getFieldPtr(int index) const override;
    const std::vector<CertificateEntry>& certs() const { return m_elements; }
    int length() const { return (int)m_elements.size(); }

    // static functions
    static const char* getFieldName(int index);
};
} // namespace Pepper

#endif
