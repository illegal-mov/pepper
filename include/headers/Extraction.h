#ifndef EXTRACTION_H
#define EXTRACTION_H

#include <string>

/* Non-member non-friend functions to dump specific bytes from a PE file */

namespace Pepper {
    class ResourceData;
    class CertificateEntry;
}

namespace Pepper { namespace Extract {
    enum class ExceptionFlag {
        NO_THROW,
        MAY_THROW,
    };

    bool dumpResource(const std::string& path, const ResourceData& resource, ExceptionFlag throwFlag);
    bool dumpCertificate(const std::string& path, const CertificateEntry& certificate, ExceptionFlag throwFlag);
}}

#endif
