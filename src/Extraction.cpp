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
template <typename T>
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

template <typename T>
static bool genericDump(const std::string &path, const T &t, bool mayThrow)
{
    // `static_assert` knows at compile time when you've done a bad thing
    static_assert(is_dumpable<T>::value, "Cannot dump this type to disk");
    std::ofstream out(path, std::ios_base::out | std::ios_base::binary);
    if (out.is_open()) {
        out.write(t.bytes(), (size_t)t.size());
    } else {
        if (!mayThrow)
            return false;
        throw FailedOpen("Unable to open the file for writing");
    }

    out.close();

    return true;
}

bool Extract::dumpResource(const std::string &path, const ResourceData &rd, bool mayThrow)
{
    return genericDump<ResourceData>(path, rd, mayThrow);
}

bool Extract::dumpCertificate(const std::string &path, const CertificateEntry &ce, bool mayThrow)
{
    return genericDump<CertificateEntry>(path, ce, mayThrow);
}
