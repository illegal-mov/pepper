#include "Extraction.h"

#include <fstream>

#include "Exceptions.h"
#include "headers/CertificateDir.h"
#include "headers/ResourceDir.h"

/* Non-member non-friend functions to dump specific bytes from a PE file */

using namespace Pepper;

/* I wanted to experiment with traits.
 * An interface class might make more sense, seeing as any
 *  type passed to `genericDump` must have the `char* bytes()`
 *  and `size_t size()` methods.
 */
template <typename DirectoryType>
struct is_dumpable {
    static constexpr bool value = false;
};

template<>
struct is_dumpable<ResourceData> {
    static constexpr bool value = true;
};

template<>
struct is_dumpable<CertificateEntry> {
    static constexpr bool value = true;
};

template <typename DirectoryType>
static bool genericDump(const std::string &path, const DirectoryType &t, Extract::ExceptionFlag throwFlag)
{
    // `static_assert` knows at compile time when you've done a bad thing
    static_assert(is_dumpable<DirectoryType>::value, "Cannot dump this type to disk");
    std::ofstream out(path, std::ios_base::out | std::ios_base::binary);
    if (out.is_open()) {
        out.write(t.bytes(), t.size());
    } else {
        if (throwFlag == Extract::ExceptionFlag::MAY_THROW) {
            throw FailedOpen("Unable to open the file for writing");
        }
        return false;
    }

    out.close();

    return true;
}

bool Extract::dumpResource(const std::string &path, const ResourceData &rd, ExceptionFlag throwFlag)
{
    return genericDump<ResourceData>(path, rd, throwFlag);
}

bool Extract::dumpCertificate(const std::string &path, const CertificateEntry &ce, ExceptionFlag throwFlag)
{
    return genericDump<CertificateEntry>(path, ce, throwFlag);
}
