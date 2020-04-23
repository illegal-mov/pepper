#include "../Types.h"
#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "ResourceDir.h"

using namespace Pepper;

size_t  ResourceDir::s_diskToMemDiff   = 0;
size_t *ResourceData::s_pDiskToMemDiff = &ResourceDir::s_diskToMemDiff;

size_t  ResourceDir::s_rsrcBase    = 0;
size_t *ResourceNode::s_pRsrcBase  = &ResourceDir::s_rsrcBase;
size_t *ResourceEntry::s_pRsrcBase = &ResourceDir::s_rsrcBase;
size_t *ResourceData::s_pRsrcBase  = &ResourceDir::s_rsrcBase;

template<>
ResourceString::GenericResourceString(const FileBytes& fbytes, const offset_t raw)
: IHeader(fbytes, raw)
, m_name(getStructPtr()->NameString, length())
{}

template<>
ResourceStringU::GenericResourceString(const FileBytes& fbytes, const offset_t raw)
: IHeader(fbytes, raw)
{
    const uint16_t *strBytes = getStructPtr()->NameString;
    m_name.resize(length());
    // I hate string encodings so much, so just hacky-copy each
    //  char16_t into a regular std::string and force ASCII range
    for (size_t i=0; i < length(); i++) {
        m_name[i] = strBytes[i] & 0x7F;
    }
}

ResourceData::ResourceData(const FileBytes& fbytes, const offset_t raw, const ResourceNode *parent)
: IHeader(fbytes, raw)
, m_parent(parent)
{}

ResourceEntry::ResourceEntry(const FileBytes& fbytes, const offset_t raw, const ResourceNode *parent, std::map<uint32_t, ResourceData*>& dataMap)
: IHeader(fbytes, raw)
{
    if (hasName()) {
        m_name = std::make_unique<ResourceStringU>(fbytes,
            *s_pRsrcBase + getStructPtr()->NameOffset);
    }

    if (isDirectory()) {
        m_node = std::make_unique<ResourceNode>(fbytes,
            *s_pRsrcBase + getStructPtr()->OffsetToDirectory, parent, dataMap);
    } else {
        m_data = std::make_unique<ResourceData>(fbytes,
            *s_pRsrcBase + getStructPtr()->OffsetToData, parent);
        dataMap[getStructPtr()->OffsetToData] = m_data.get();
    }
}

ResourceEntry::~ResourceEntry()
{
    if (isDirectory()) {
        m_node.~unique_ptr<ResourceNode>();
    } else {
        m_data.~unique_ptr<ResourceData>();
    }
}

ResourceNode::ResourceNode(const FileBytes& fbytes, const offset_t raw, const ResourceNode *parent, std::map<uint32_t, ResourceData*>& dataMap)
: IHeader(fbytes, raw)
, m_parent(parent)
{
    // pre-allocate for total number of entries
    size_t numEntries = static_cast<size_t>(getStructPtr()->NumberOfNamedEntries)
                      + getStructPtr()->NumberOfIdEntries;
    m_resourceEntries.reserve(numEntries);

    // construct each entry
    size_t entriesBase = hdrOffset() + sizeof(IMAGE_RESOURCE_DIRECTORY);
    for (size_t i=0; i < numEntries; i++) {
        m_resourceEntries.emplace_back(fbytes,
            entriesBase + (sizeof(IMAGE_RESOURCE_ENTRY) * i),
            this, dataMap);
    }
}

ResourceDir::ResourceDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        s_diskToMemDiff = m_diskToMemoryDifference;
        s_rsrcBase = dirOffset();
        m_root = std::make_unique<ResourceNode>(fbytes, dirOffset(), nullptr, m_dataMap);
    }
}

template <typename StringStruct> // requires variant declaration in header to link
const char* GenericResourceString<StringStruct>::getFieldName(const int index)
{
    switch (index) {
        case LENGTH     : return "Length";
        case NAME_STRING: return "Name String";
        default         : return "<UNKNOWN>";
    }
}

template <typename StringStruct> // requires variant declaration in header to link
const void* GenericResourceString<StringStruct>::getFieldPtr(const int index) const
{
    switch (index) {
        case LENGTH     : return &getStructPtr()->Length;
        case NAME_STRING: return &getStructPtr()->NameString;
        default         : return nullptr;
    }
}

const char* ResourceData::getFieldName(const int index)
{
    switch (index) {
        case OFFSET_TO_DATA: return "Offset to Data";
        case SIZE          : return "Size";
        case CODE_PAGE     : return "Code Page";
        case RESERVED      : return "<Reserved>";
        default            : return "<UNKNOWN>";
    }
}

const void* ResourceData::getFieldPtr(const int index) const
{
    switch (index) {
        case OFFSET_TO_DATA: return &getStructPtr()->OffsetToData;
        case SIZE          : return &getStructPtr()->Size;
        case CODE_PAGE     : return &getStructPtr()->CodePage;
        case RESERVED      : return &getStructPtr()->Reserved;
        default            : return nullptr;
    }
}

const char* ResourceEntry::getFieldName(const int index)
{
    switch (index) {
        case NAME          : return "Name";
        case OFFSET_TO_DATA: return "Offset to Data";
        default            : return "<UNKNOWN>";
    }
}

const void* ResourceEntry::getFieldPtr(const int index) const
{
    switch (index) {
        case NAME          : return &getStructPtr()->Name;
        case OFFSET_TO_DATA: return &getStructPtr()->OffsetToData;
        default            : return nullptr;
    }
}

const char* ResourceNode::getFieldName(const int index)
{
    switch (index) {
        case CHARACTERISTICS        : return "Characteristics";
        case TIMEDATESTAMP          : return "Timestamp";
        case MAJOR_VERSION          : return "Major Version";
        case MINOR_VERSION          : return "Minor Version";
        case NUMBER_OF_NAMED_ENTRIES: return "Number of Named Entries";
        case NUMBER_OF_ID_ENTRIES   : return "Number of ID Entries";
        default                     : return "<UNKNOWN>";
    }
}

const void* ResourceNode::getFieldPtr(const int index) const
{
    switch (index) {
        case CHARACTERISTICS        : return &getStructPtr()->Characteristics;
        case TIMEDATESTAMP          : return &getStructPtr()->TimeDateStamp;
        case MAJOR_VERSION          : return &getStructPtr()->MajorVersion;
        case MINOR_VERSION          : return &getStructPtr()->MinorVersion;
        case NUMBER_OF_NAMED_ENTRIES: return &getStructPtr()->NumberOfNamedEntries;
        case NUMBER_OF_ID_ENTRIES   : return &getStructPtr()->NumberOfIdEntries;
        default                     : return nullptr;
    }
}

const char* ResourceDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ResourceDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

