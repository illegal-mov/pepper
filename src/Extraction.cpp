#include <Extraction.h>

#include <fstream>

#include <Exceptions.h>
#include <pe_structures/CertificateDir.h>
#include <pe_structures/ResourceDir.h>

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

namespace
{
template <typename DirectoryType>
bool genericDump(const std::string& path, const DirectoryType& directory, ExceptionFlag throwFlag = ExceptionFlag::MAY_THROW)
{
    static_assert(is_dumpable<DirectoryType>::value, "Cannot dump this type to disk");
    std::ofstream out(path, std::ios_base::out | std::ios_base::binary);
    if (out.is_open()) {
        out.write(reinterpret_cast<const char*>(directory.bytes()), directory.size());
    } else {
        if (throwFlag == ExceptionFlag::MAY_THROW) {
            throw FailedOpen("Unable to open the file for writing");
        }
        return false;
    }

    out.close();

    return true;
}
} // namespace

bool Extract::dumpResource(const std::string& path, const ResourceData& resource, ExceptionFlag throwFlag)
{
    return genericDump<ResourceData>(path, resource, throwFlag);
}

bool Extract::dumpCertificate(const std::string& path, const CertificateEntry& certificate, ExceptionFlag throwFlag)
{
    return genericDump<CertificateEntry>(path, certificate, throwFlag);
}
