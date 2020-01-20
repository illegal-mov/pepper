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
ResourceString::GenericResourceString(const FileBytes &fbytes, const size_t raw)
: IHeader(fbytes, raw)
, m_name((char*)getFieldPtr(NAME_STRING), length())
{}

template<>
ResourceStringU::GenericResourceString(const FileBytes &fbytes, const size_t raw)
: IHeader(fbytes, raw)
{
    const uint16_t *strBytes = (uint16_t*)getFieldPtr(NAME_STRING);
    m_name.resize(length());
    // I hate string encodings so much, so just hacky-copy each
    //  char16_t into a regular std::string and force ASCII range
    for (size_t i=0; i < length(); i++) {
        m_name[i] = strBytes[i] & 0x7F;
    }
}

ResourceData::ResourceData(const FileBytes &fbytes, const size_t raw, const ResourceNode *parent)
: IHeader(fbytes, raw)
, m_parent(parent)
{}

ResourceEntry::ResourceEntry(const FileBytes &fbytes, const size_t raw, const ResourceNode *parent, std::map<uint32_t, ResourceData*> &dataMap)
: IHeader(fbytes, raw)
{
    if (hasName()) {
        m_rsrcName = new ResourceStringU(fbytes,
            *s_pRsrcBase + entry()->NameOffset);
    }

    if (isDirectory()) {
        m_node = new ResourceNode(fbytes,
            *s_pRsrcBase + entry()->OffsetToDirectory, parent, dataMap);
    } else {
        m_data = new ResourceData(fbytes,
            *s_pRsrcBase + entry()->OffsetToData, parent);
        dataMap[entry()->OffsetToData] = m_data;
    }
}

ResourceEntry::~ResourceEntry()
{
    if (hasName()) {
        delete m_rsrcName;
        m_rsrcName = nullptr;
    }

    if (isDirectory()) {
        delete m_node;
    } else {
        delete m_data;
    }

    // these members are union'd together, so only set one to nullptr
    m_node = nullptr;
}

ResourceNode::ResourceNode(const FileBytes &fbytes, const size_t raw, const ResourceNode *parent, std::map<uint32_t, ResourceData*> &dataMap)
: IHeader(fbytes, raw)
, m_parent(parent)
{
    // pre-allocate for total number of entries
    size_t numEntries = static_cast<size_t>(header()->NumberOfNamedEntries)
                      + header()->NumberOfIdEntries;
    m_entries.reserve(numEntries);

    // construct each entry
    size_t entriesBase = hdrOffset() + sizeof(IMAGE_RESOURCE_DIRECTORY);
    for (size_t i=0; i < numEntries; i++) {
        m_entries.emplace_back(fbytes,
            entriesBase + (sizeof(IMAGE_RESOURCE_ENTRY) * i),
            this, dataMap);
    }
}

ResourceDir::ResourceDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        s_diskToMemDiff = m_diffOfRvaRaw;
        s_rsrcBase = dirOffset();
        m_root = std::make_unique<ResourceNode>(fbytes, dirOffset(), nullptr, m_dataMap);
    }
}

template <typename T> // requires variant declaration in header to link
const char* GenericResourceString<T>::getFieldName(const int index)
{
    switch (index) {
        case LENGTH     : return "Length";
        case NAME_STRING: return "Name String";
        default         : return "<UNKNOWN>";
    }
}

template <typename T> // requires variant declaration in header to link
const void* GenericResourceString<T>::getFieldPtr(const int index) const
{
    switch (index) {
        case LENGTH     : return &string()->Length;
        case NAME_STRING: return &string()->NameString;
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
        case OFFSET_TO_DATA: return &data()->OffsetToData;
        case SIZE          : return &data()->Size;
        case CODE_PAGE     : return &data()->CodePage;
        case RESERVED      : return &data()->Reserved;
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
        case NAME          : return &entry()->Name;
        case OFFSET_TO_DATA: return &entry()->OffsetToData;
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
        case CHARACTERISTICS        : return &header()->Characteristics;
        case TIMEDATESTAMP          : return &header()->TimeDateStamp;
        case MAJOR_VERSION          : return &header()->MajorVersion;
        case MINOR_VERSION          : return &header()->MinorVersion;
        case NUMBER_OF_NAMED_ENTRIES: return &header()->NumberOfNamedEntries;
        case NUMBER_OF_ID_ENTRIES   : return &header()->NumberOfIdEntries;
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

