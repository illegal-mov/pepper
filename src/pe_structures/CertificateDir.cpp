#include "../../include/headers/pe_structures/CertificateDir.h"

#include "../../include/headers/Identification.h"
#include "../../include/headers/pe_structures/DataDirectoryEntry.h"

using namespace Pepper;

namespace
{
constexpr int kByteAlignment = 8;

size_t alignToPowerOfTwo(const size_t value, const size_t alignment)
{
    return value + ((alignment - 1) & (alignment - (value & (alignment - 1))));
}
} // namespace

CertificateDir::CertificateDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        // certificate is special and does not need a diff
        m_diskToMemoryDifference = 0;

        uint32_t currentSize = 0;
        while (currentSize < dde.size()) {
            m_certificates.emplace_back(fbytes, dirOffset() + currentSize);
            currentSize += m_certificates.back().getStructPtr()->Length;
            currentSize = alignToPowerOfTwo(currentSize, kByteAlignment);
        }
    }
}

const char* CertificateEntry::getFieldName(const int index)
{
    switch (index) {
        case LENGTH           : return "Certificate Size";
        case REVISION         : return "Certificate Version";
        case CERTIFICATE_TYPE : return "Certificate Type";
        case CERTIFICATE_BYTES: return "Certificate Bytes";
        default               : return "<UNKNOWN>";
    }
}

const void* CertificateEntry::getFieldPtr(const int index) const
{
    switch (index) {
        case LENGTH           : return &getStructPtr()->Length;
        case REVISION         : return &getStructPtr()->Revision;
        case CERTIFICATE_TYPE : return &getStructPtr()->CertificateType;
        case CERTIFICATE_BYTES: return &getStructPtr()->CertificateBytes;
        default               : return nullptr;
    }
}

const char* CertificateDir::getFieldName(const int index)
{
    switch (index) {
        default: return "Attribute Certificate";
    }
}

const void* CertificateDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

