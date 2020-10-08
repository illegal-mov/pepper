#ifndef RESOURCE_H
#define RESOURCE_H

#include <map>
#include <memory>
#include <vector>

#include "../Types.h"
#include "../generics/iDir.h"

/*
typedef struct _IMAGE_RESOURCE_DIRECTORY {
    int32_t  Characteristics;      // reserved, must be zero
    int32_t  TimeDateStamp;        // resource data creation time
    int16_t  MajorVersion;
    int16_t  MinorVersion;
    uint16_t NumberOfNamedEntries;
    uint16_t NumberOfIdEntries;
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

typedef struct _IMAGE_RESOURCE_ENTRY {
    union { // string or ID
        struct {
            uint32_t NameOffset   : 31;
            int32_t  NameIsString : 1;
        };
        int32_t Name;
        int16_t ID; // TODO: this ID identifies resource type only at root level
    };
    union { // dir ptr or data ptr
        uint32_t OffsetToData;
        struct {
            uint32_t OffsetToDirectory : 31;
            int32_t  DataIsDirectory   : 1;
        };
    };
} IMAGE_RESOURCE_ENTRY, *PIMAGE_RESOURCE_ENTRY;

typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING {
	uint16_t Length;
	char     NameString[];
} IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;

typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING_U {
	uint16_t Length;
	uint16_t NameString[]; // TODO: maybe char16_t
} IMAGE_RESOURCE_DIRECTORY_STRING_U, *PIMAGE_RESOURCE_DIRECTORY_STRING_U;

typedef struct _IMAGE_RESOURCE_DATA {
    uint32_t OffsetToData;
    uint32_t Size;
    int32_t  CodePage;
    int32_t  Reserved; // reserved, must be zero
} IMAGE_RESOURCE_DATA, *PIMAGE_RESOURCE_DATA;
*/

namespace Pepper {

class PeFile;
class FileBytes;
class DataDirectoryEntry;
class ResourceNode;
template <typename StringStruct>
class GenericResourceString;

/* A 2-byte string length prefixing a non-null-terminated std::string
 */
using ResourceString = GenericResourceString<IMAGE_RESOURCE_DIRECTORY_STRING>;

/* A 2-byte string length prefixing a non-null-terminated std::u16string
 */
using ResourceStringU = GenericResourceString<IMAGE_RESOURCE_DIRECTORY_STRING_U>;

/* A 2-byte string length prefixing some kind of non-null-terminated string
 */
template <typename StringStruct>
class GenericResourceString : public IHeader {
public:
    enum Fields {
        LENGTH,
        NAME_STRING,
        _NUM_FIELDS,
    };

    GenericResourceString(const FileBytes& fbytes, const offset_t raw);

    const void* getFieldPtr(const int index) const override;
    const StringStruct* getStructPtr() const { return static_cast<const StringStruct*>(hdr()); }
    uint16_t length() const { return getStructPtr()->Length; }
    const std::string& name() const { return m_name; }

    static const char* getFieldName(const int index);

private:
    std::string m_name{};
};

/* A leaf in the resource tree.
 * Describes the size of and offset to the embedded file content.
 */
class ResourceData : public IHeader {
public:
    enum Fields {
        OFFSET_TO_DATA,
        SIZE,
        CODE_PAGE,
        RESERVED,
        _NUM_FIELDS,
    };

    ResourceData(const FileBytes& fbytes, const offset_t raw, const ResourceNode* parent);

    ResourceData(const ResourceData& rd)
    : IHeader(rd)
    , m_parent(rd.m_parent)
    {}

    ResourceData& operator=(const ResourceData& rd)
    {
        IHeader::operator=(rd);
        m_parent = rd.m_parent;
        return *this;
    }

    const void* getFieldPtr(const int index) const override;
    const IMAGE_RESOURCE_DATA* getStructPtr() const { return static_cast<const IMAGE_RESOURCE_DATA*>(hdr()); }
    const uint8_t* bytes() const { return &mem()[getStructPtr()->OffsetToData - s_pDiskToMemDiff]; }
    uint32_t size() const { return getStructPtr()->Size; }

    static const char* getFieldName(const int index);

private:
    static size_t& s_pDiskToMemDiff; // RVAs in .edata point to .text
    static size_t& s_pRsrcBase;
    const ResourceNode* m_parent{};
};

/* A pair of DWORDs describing an entry's type and offset;
 * This is the structure that determines whether the
 *  next object in the tree is a leaf or a sub-tree.
 */
class ResourceEntry : public IHeader {
public:
    enum Fields {
        NAME,
        OFFSET_TO_DATA,
        _NUM_FIELDS,
    };

    ResourceEntry(const FileBytes& fbytes, const offset_t raw, const ResourceNode* parent, std::map<uint32_t, ResourceData*>& dataMap);

    ResourceEntry(ResourceEntry &&re)
    : m_name(std::move(re.m_name))
    , m_node(std::move(re.m_node))
    {}

    ~ResourceEntry();

    ResourceEntry(const ResourceEntry& re) = delete;
    ResourceEntry& operator=(const ResourceEntry& re) = delete;

    const void* getFieldPtr(const int index) const override;
    const IMAGE_RESOURCE_ENTRY* getStructPtr() const { return static_cast<const IMAGE_RESOURCE_ENTRY*>(hdr()); }
    bool isDirectory() const { return getStructPtr()->DataIsDirectory; }
    bool hasName() const { return getStructPtr()->NameIsString; }
    const ResourceStringU* name() const { return m_name.get(); }
    const ResourceNode* node() const { return isDirectory() ? m_node.get() : nullptr; }
    const ResourceData* data() const { return isDirectory() ? nullptr : m_data.get(); }

    static const char* getFieldName(const int index);

private:
    static size_t& s_pRsrcBase;
    std::unique_ptr<ResourceStringU> m_name{};
    union {
        std::unique_ptr<ResourceNode> m_node{};
        std::unique_ptr<ResourceData> m_data;
    };
};

/* Exactly one RESOURCE_DIRECTORY immediately followed by a variable-length array of RESOURCE_ENTRY
 */
class ResourceNode : public IHeader {
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

    ResourceNode(const FileBytes& fbytes, const offset_t raw, const ResourceNode* parent, std::map<uint32_t, ResourceData*>& dataMap);

    ResourceNode(const ResourceNode& rn) = delete;
    ResourceNode& operator=(const ResourceNode& rn) = delete;

    const void* getFieldPtr(const int index) const override;
    const IMAGE_RESOURCE_DIRECTORY* getStructPtr() const { return static_cast<const IMAGE_RESOURCE_DIRECTORY*>(hdr()); }
    const std::vector<ResourceEntry>& entries() const { return m_resourceEntries; }

    static const char* getFieldName(const int index);

private:
    static size_t& s_pRsrcBase;
    const ResourceNode* m_parent{};
    std::vector<ResourceEntry> m_resourceEntries{};
};

/* The resource directory is a tree structure where each node has a variable-number of children.
 * Each child is either a ResourceNode with its own children, or a ResourceData.
 */
class ResourceDir final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ResourceDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    ResourceDir(const ResourceDir& rd) = delete;
    ResourceDir& operator=(const ResourceDir& rd) = delete;

    const void* getFieldPtr(const int index) const override;
    const ResourceNode* resources() const { return m_root.get(); }
    const std::map<uint32_t, ResourceData*> map() const { return m_dataMap; }

    static const char* getFieldName(const int index);

    // classes that need special access to s_rsrcBase
    friend class ResourceData;
    friend class ResourceEntry;
    friend class ResourceNode;

private:
    static size_t s_diskToMemDiff;
    static size_t s_rsrcBase;
    std::unique_ptr<ResourceNode> m_root{};
    std::map<uint32_t, ResourceData*> m_dataMap{};
};

template class GenericResourceString<IMAGE_RESOURCE_DIRECTORY_STRING>;
template class GenericResourceString<IMAGE_RESOURCE_DIRECTORY_STRING_U>;

} // namespace Pepper

#endif
