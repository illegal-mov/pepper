#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "IatDir.h"

using namespace Pepper;

template <typename ArchType>
AddressList<ArchType>::AddressList(const FileBytes &fbytes, const size_t raw, const size_t len)
: IHeader(fbytes, raw)
, m_length(len)
{}

template <typename ArchType>
void IatDir::readAddrList(const FileBytes &fbytes, const DataDirectoryEntry &dde)
{
    const ArchType *addrs = static_cast<const ArchType*>(dir());
    size_t i=0;
    uint32_t bytesRead = 0;
    while (bytesRead < dde.size()) {
        // get current addrs list len
        size_t len = 0;
        while (addrs[i] != 0) {
            len++; // length of current row
            i++;   // index for whole table
        }

        m_list32.emplace_back(fbytes, dirOffset() + bytesRead, len);

        bytesRead += (len+1) * sizeof(ArchType); // +1 because of null terminator
        i++; // `i` was left sitting on a null-terminator, so step over
    }
}

IatDir::IatDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        if (Ident::is32bit(pe)) {
            readAddrList<uint32_t>(fbytes, dde);
        } else {
            readAddrList<uint64_t>(fbytes, dde);
        }
    }
}

template <typename ArchType> // requires variant declaration in header to link
const char* AddressList<ArchType>::getFieldName(const int index)
{
    switch (index) {
        default: return "Import Address";
    }
}

template <typename ArchType> // requires variant declaration in header to link
const void* AddressList<ArchType>::getFieldPtr(const int index) const
{
    size_t uindex = static_cast<size_t>(index);
    return (uindex < length())
        ? &addresses()[uindex]
        : nullptr;
}

const char* IatDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* IatDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

