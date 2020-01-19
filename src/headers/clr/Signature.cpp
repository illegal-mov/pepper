#include "../DataDirectoryEntry.h"
#include "Signature.h"

using namespace Pepper;

ClrSignature::ClrSignature(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
, m_length(dde.size())
{}

const char* ClrSignature::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ClrSignature::getFieldPtr(int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &sig()[uindex]
        : nullptr;
}

