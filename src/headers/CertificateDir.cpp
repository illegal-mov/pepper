#include "../Identification.h"
#include "CertificateDir.h"
#include "DataDirectoryEntry.h"

using namespace Pepper;

CertificateDir::CertificateDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        // certificate is special and does not need a diff
        m_diffOfRvaRaw = 0;

        uint32_t currentSize = 0;
        while (currentSize < dde.size()) {
            m_elements.emplace_back(fbytes, dirOffset() + currentSize);
            currentSize += *(uint32_t*)m_elements.back().getFieldPtr(CertificateEntry::LENGTH);
            // round up to 8 byte alignment
            currentSize += (8 - (currentSize & 7)) & 7;
        }
    }
}

const char* CertificateEntry::getFieldName(int index)
{
    switch (index) {
        case LENGTH           : return "Certificate Size";
        case REVISION         : return "Certificate Version";
        case CERTIFICATE_TYPE : return "Certificate Type";
        case CERTIFICATE_BYTES: return "Certificate Bytes";
        default               : return "<UNKNOWN>";
    }
}

const void* CertificateEntry::getFieldPtr(int index) const
{
    switch (index) {
        case LENGTH           : return &cert()->Length;
        case REVISION         : return &cert()->Revision;
        case CERTIFICATE_TYPE : return &cert()->CertificateType;
        case CERTIFICATE_BYTES: return &cert()->CertificateBytes;
        default               : return nullptr;
    }
}

const char* CertificateDir::getFieldName(int index)
{
    switch (index) {
        default: return "Attribute Certificate";
    }
}

const void* CertificateDir::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

