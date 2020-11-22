#ifndef EXTRACTION_H
#define EXTRACTION_H

#include <string>
#include "Exceptions.h"

/* Non-member non-friend functions to dump specific bytes from a PE file */

namespace Pepper {
    class ResourceData;
    class CertificateEntry;
}

namespace Pepper { namespace Extract {
    bool dumpResource(const std::string& path, const ResourceData& resource, ExceptionFlag throwFlag = ExceptionFlag::MAY_THROW);
    bool dumpCertificate(const std::string& path, const CertificateEntry& certificate, ExceptionFlag throwFlag = ExceptionFlag::MAY_THROW);
}} // namespace Pepper::Extract

#endif
