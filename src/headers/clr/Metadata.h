#ifndef CLR_METADATA_H
#define CLR_METADATA_H

#include <vector>

#include "../../Types.h"
#include "../../generics/iDir.h"
#include "../struct.h"

/*
typedef struct _IMAGE_COR20_METADATA_HEADER {
    int32_t Signature;       // always set to "BSJB"
    int16_t MajorVersion;    // metadata major version number
    int16_t MinorVersion;    // metadata minor version number
    int32_t Reserved;        // reserved, must be zero
    int32_t Length;          // Version length (multiple of 4)
    char    Version[];       // length-prefixed string
    // FLEX SPACE
//  int16_t Flags;           // reserved, must be zero
//  int16_t NumberOfStreams;
} IMAGE_COR20_METADATA_HEADER, *PIMAGE_COR20_METADATA_HEADER;

typedef struct _IMAGE_COR20_METADATA_STREAM_HEADER {
    int32_t Offset; // relative to METADATA_header
    int32_t Size;
    char    Name[];
} IMAGE_COR20_METADATA_STREAM_HEADER, *PIMAGE_COR20_METADATA_STREAM_HEADER;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/* A mysterious dump of bytes
 */
class ClrStream final : public IHeader {
public:
    enum Fields {
        OFFSET,
        SIZE,
        NAME,
        _NUM_FIELDS,
    };

    ClrStream(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    {}

    const void* getFieldPtr(const int index) const override;
    const IMAGE_COR20_METADATA_STREAM_HEADER* getStructPtr() const
    {
        return static_cast<const IMAGE_COR20_METADATA_STREAM_HEADER*>(hdr());
    }

    const char* data() const
    {
        return &mem()[*s_pMetadataBase + getStructPtr()->Offset];
    }

    static const char* getFieldName(const int index);

private:
    static size_t *s_pMetadataBase;
};

/* Exactly one COR20_METADATA_HEADER followed by a variable-number of STREAM_HEADERS
 */
class ClrMetadata final : public IDirectory {
public:
    enum Fields {
        SIGNATURE,
        MAJOR_VERSION,
        MINOR_VERSION,
        RESERVED,
        LENGTH,
        VERSION,
        FLAGS,
        NUMBER_OF_STREAMS,
        _NUM_FIELDS,
    };

    ClrMetadata() = default;

    ClrMetadata(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    const IMAGE_COR20_METADATA_HEADER* getStructPtr() const { return static_cast<const IMAGE_COR20_METADATA_HEADER*>(dir()); }
    const std::vector<ClrStream>& streams() const { return m_streams; }
    const void* getFieldPtr(const int index) const override;

    static const char* getFieldName(const int index);

    // classes that need special access to s_metadataBase
    friend class ClrStream;

private:
    std::vector<ClrStream> m_streams{};
    static size_t s_metadataBase;
};
}

#endif
