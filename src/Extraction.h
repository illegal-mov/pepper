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
    /* dump the resource data to disk */
    bool dumpResource(const std::string &path, const ResourceData &rd, bool mayThrow=true);

    /* dump the certificate data disk */
    bool dumpCertificate(const std::string &path, const CertificateEntry &ce, bool mayThrow=true);
}}

#endif
