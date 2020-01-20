#ifndef RESOURCE_H
#define RESOURCE_H

#include <map>
#include <memory>
#include <vector>

#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_RESOURCE_DIRECTORY {
    int32_t Characteristics;      // reserved, must be zero
    int32_t TimeDateStamp;        // resource data creation time
    int16_t MajorVersion;
    int16_t MinorVersion;
    int16_t NumberOfNamedEntries;
    int16_t NumberOfIdEntries;
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

typedef struct _IMAGE_RESOURCE_ENTRY {
    union { // string or ID
        struct {
            int32_t NameOffset   : 31;
            int32_t NameIsString : 1;
        };
        int32_t Name;
        int16_t ID; // TODO: this ID identifies resource type only at root level
    };
    union { // dir ptr or data ptr
        int32_t OffsetToData;
        struct {
            int32_t OffsetToDirectory : 31;
            int32_t DataIsDirectory   : 1;
        };
    };
} IMAGE_RESOURCE_ENTRY, *PIMAGE_RESOURCE_ENTRY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING {
	uint16_t Length;
	char     NameString[];
} IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;

typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
	uint16_t Length;
	uint16_t NameString[]; // TODO: maybe char16_t
} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;

typedef struct _IMAGE_RESOURCE_DATA {
    int32_t OffsetToData;
    int32_t Size;
    int32_t CodePage;
    int32_t Reserved;     // must be zero
} IMAGE_RESOURCE_DATA, *PIMAGE_RESOURCE_DATA;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;
class ResourceNode;
template <typename T>
class GenericResourceString;

// typedefs that depend on the forward declaration

/* A 2-byte string length prefixing a non-null-terminated std::string
 */
typedef GenericResourceString<char> ResourceString;

/* A 2-byte string length prefixing a non-null-terminated std::u16string
 */
typedef GenericResourceString<uint16_t> ResourceStringU;

/* A 2-byte string length prefixing some kind of non-null-terminated string
 */
template <typename T>
class GenericResourceString : public IHeader {
private:
    std::string m_name{};
public:
    enum Fields {
        LENGTH,
        NAME_STRING,
        _NUM_FIELDS,
    };

    GenericResourceString()
    : IHeader()
    {}

    GenericResourceString(const FileBytes &fbytes, const size_t raw);

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_RESOURCE_DIR_STRING_U* string() const { return (PIMAGE_RESOURCE_DIR_STRING_U)hdr(); }
    uint16_t length() const { return string()->Length; }
    const std::string& name() const { return m_name; }

    // static functions
    static const char* getFieldName(const int index);
};

/* A leaf in the resource tree.
 * Describes the size of and offset to the embedded file content.
 */
class ResourceData : public IHeader {
private:
    static size_t *s_pDiskToMemDiff; // RVAs in .edata point to .text
    static size_t *s_pRsrcBase;
    const ResourceNode *m_parent{};
public:
    enum Fields {
        OFFSET_TO_DATA,
        SIZE,
        CODE_PAGE,
        RESERVED,
        _NUM_FIELDS,
    };

    ResourceData()
    : IHeader()
    {}

    ResourceData(const FileBytes &fbytes, const size_t raw, const ResourceNode *parent);

    ResourceData(const ResourceData &rd)
    : IHeader(rd)
    , m_parent(rd.m_parent)
    {}

    const ResourceData& operator=(const ResourceData &rd)
    {
        IHeader::operator=(rd);
        m_parent = rd.m_parent;
        return *this;
    }

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_RESOURCE_DATA* data() const { return (PIMAGE_RESOURCE_DATA)hdr(); }
    const char* bytes() const { return &mem()[data()->OffsetToData - *s_pDiskToMemDiff]; }
    uint32_t size() const { return data()->Size; }

    // static functions
    static const char* getFieldName(const int index);
};

/* A pair of DWORDs describing an entry's type and offset;
 * This is the structure that determines whether the
 *  next object in the tree is a leaf or a sub-tree.
 */
class ResourceEntry : public IHeader {
private:
    static size_t *s_pRsrcBase;
    ResourceStringU *m_rsrcName{};
    union {
        ResourceNode *m_node{};
        ResourceData *m_data;
    };
public:
    enum Fields {
        NAME,
        OFFSET_TO_DATA,
        _NUM_FIELDS,
    };

    ResourceEntry()
    : IHeader()
    {}

    ResourceEntry(const FileBytes &fbytes, const size_t raw, const ResourceNode *parent, std::map<uint32_t, ResourceData*> &dataMap);

    ResourceEntry(const ResourceEntry &re)
    : IHeader(re)
    , m_node(re.m_node)
    {}

    ~ResourceEntry();

    const ResourceEntry& operator=(const ResourceEntry &re)
    {
        IHeader::operator=(re);
        m_node = re.m_node;
        return *this;
    }

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_RESOURCE_ENTRY* entry() const { return (PIMAGE_RESOURCE_ENTRY)hdr(); }
    bool isDirectory() const { return entry()->DataIsDirectory; }
    bool hasName() const { return entry()->NameIsString; }
    const ResourceStringU* name() const { return m_rsrcName; }
    const ResourceNode* node() const { return m_node; }
    const ResourceData* data() const { return m_data; }

    // static functions
    static const char* getFieldName(const int index);
};

/* Exactly one RESOURCE_DIRECTORY immediately followed by a variable-length array of RESOURCE_ENTRY
 */
class ResourceNode : public IHeader {
private:
    static size_t *s_pRsrcBase;
    const ResourceNode *m_parent{};
    std::vector<ResourceEntry> m_entries{};
public:
    enum Fields {
        CHARACTERISTICS,
        TIMEDATESTAMP,
        MAJOR_VERSION,
        MINOR_VERSION,
        NUMBER_OF_NAMED_ENTRIES,
        NUMBER_OF_ID_ENTRIES,
        _NUM_FIELDS,
    };

    ResourceNode()
    : IHeader()
    {}

    ResourceNode(const FileBytes &fbytes, const size_t raw, const ResourceNode *parent, std::map<uint32_t, ResourceData*> &dataMap);

    ResourceNode(const ResourceNode &rn)
    : IHeader(rn)
    , m_parent(rn.m_parent)
    , m_entries(rn.m_entries)
    {}

    const ResourceNode& operator=(const ResourceNode &rn)
    {
        IHeader::operator=(rn);
        m_parent = rn.m_parent;
        m_entries = rn.m_entries;
        return *this;
    }

    // member functions
    const void* getFieldPtr(const int index) const override;
    const IMAGE_RESOURCE_DIRECTORY* header() const { return (PIMAGE_RESOURCE_DIRECTORY)hdr(); }
    const std::vector<ResourceEntry>& entries() const { return m_entries; }

    // static functions
    static const char* getFieldName(const int index);
};

/* The resource directory is a tree structure where each node has a variable-number of children.
 * Each child is either a ResourceNode with its own children, or a ResourceData.
 */
class ResourceDir final : public IDirectory {
private:
    static size_t s_diskToMemDiff;
    static size_t s_rsrcBase;
    std::unique_ptr<ResourceNode> m_root{};
    std::map<uint32_t, ResourceData*> m_dataMap{};
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ResourceDir()
    : IDirectory()
    {}

    ResourceDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    ResourceDir(const ResourceDir &rd) = delete;
    const ResourceDir& operator=(const ResourceDir &rd) = delete;

    // member functions
    const void* getFieldPtr(const int index) const override;
    const ResourceNode* resources() const { return m_root.get(); }
    const std::map<uint32_t, ResourceData*> map() const { return m_dataMap; }

    // static functions
    static const char* getFieldName(const int index);

    // classes that need special access to s_rsrcBase
    friend class ResourceData;
    friend class ResourceEntry;
    friend class ResourceNode;
};

// variant declarations
template class GenericResourceString<char>;
template class GenericResourceString<uint16_t>;

} // namespace Pepper

#endif // RESOURCE_H
