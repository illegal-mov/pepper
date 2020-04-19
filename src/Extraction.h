#ifndef EXTRACTION_H
#define EXTRACTION_H

#include <string>

/* Non-member non-friend functions to dump specific bytes from a PE file */

// forward declarations
namespace Pepper {
    class ResourceData;
    class CertificateEntry;
}

namespace Pepper { namespace Extract {
    enum class ExceptionFlag {
        NO_THROW,
        MAY_THROW,
    };

    /* dump the resource data to disk */
    bool dumpResource(const std::string& path, const ResourceData& resource, ExceptionFlag throwFlag);

    /* dump the certificate data disk */
    bool dumpCertificate(const std::string& path, const CertificateEntry& certificate, ExceptionFlag throwFlag);
}}

#endif
