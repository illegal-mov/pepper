#include <cstring>

#include <pe_structures/DataDirectoryEntry.h>
#include <pe_structures/clr/Metadata.h>

using namespace Pepper;

size_t  ClrMetadata::s_metadataBase = 0;
size_t& ClrStream::s_pMetadataBase  = ClrMetadata::s_metadataBase;

namespace
{
constexpr int kByteAlignment = 4;

size_t alignToPowerOfTwo(const size_t value, const size_t alignment)
{
    return value + ((alignment - 1) & (alignment - (value & (alignment - 1))));
}
} // namespace

ClrMetadata::ClrMetadata(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    s_metadataBase = dirOffset();

    const uint16_t numStreams = *static_cast<const uint16_t*>(getFieldPtr(NUMBER_OF_STREAMS));
    m_streams.reserve(numStreams);

    /* Because of the length-prefixed string in the middle of the
     * structure, this is a really ridiculous way of doing `sizeof`
     */
    size_t pos = offsetof(IMAGE_COR20_METADATA_HEADER, Version)
               + getStructPtr()->Length
               + sizeof(getStructPtr()->Flags)
               + sizeof(getStructPtr()->NumberOfStreams);

    for (size_t i=0; i < numStreams; ++i) {
        ClrStream tmp(fbytes, dirOffset() + pos);
        m_streams.push_back(tmp);

        const char* name = tmp.getStructPtr()->Name;
        pos += sizeof(IMAGE_COR20_METADATA_STREAM_HEADER) + strlen(name) - kByteAlignment + 1;
        pos = alignToPowerOfTwo(pos, kByteAlignment);
    }
}

const char* ClrStream::getFieldName(const int index)
{
    switch (index) {
        case OFFSET: return "Offset";
        case SIZE  : return "Size";
        case NAME  : return "Name";
        default    : return "<UNKNOWN>";
    }
}

const void* ClrStream::getFieldPtr(const int index) const
{
    switch (index) {
        case OFFSET: return &getStructPtr()->Offset;
        case SIZE  : return &getStructPtr()->Size;
        case NAME  : return &getStructPtr()->Name;
        default    : return nullptr;
    }
}

const char* ClrMetadata::getFieldName(const int index)
{
    switch (index) {
        case SIGNATURE        : return "Signature";
        case MAJOR_VERSION    : return "Major Version";
        case MINOR_VERSION    : return "Minor Version";
        case RESERVED         : return "<Reserved>";
        case LENGTH           : return "Length";
        case VERSION          : return "Version";
        case FLAGS            : return "Flags";
        case NUMBER_OF_STREAMS: return "Number of Streams";
        default               : return "<UNKNOWN>";
    }
}

const void* ClrMetadata::getFieldPtr(const int index) const
{
    switch (index) {
        case SIGNATURE        : return &getStructPtr()->Signature;
        case MAJOR_VERSION    : return &getStructPtr()->MajorVersion;
        case MINOR_VERSION    : return &getStructPtr()->MinorVersion;
        case RESERVED         : return &getStructPtr()->Reserved;
        case LENGTH           : return &getStructPtr()->Length;
        case VERSION          : return &getStructPtr()->Version;
        // the length-prefixed string makes this part weird
        case FLAGS            : return &static_cast<const char*>(dir())[offsetof(IMAGE_COR20_METADATA_HEADER, Version) + getStructPtr()->Length];
        case NUMBER_OF_STREAMS: return &static_cast<const char*>(dir())[offsetof(IMAGE_COR20_METADATA_HEADER, Version) + getStructPtr()->Length + sizeof(getStructPtr()->Flags)];
        default               : return nullptr;
    }
}

