#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "IatDir.h"

using namespace Pepper;

template <typename T>
AddressList<T>::AddressList(const FileBytes &fbytes, int raw, int len)
: IHeader(fbytes, raw)
, m_length(len)
{}

template <typename T>
void IatDir::readAddrList(const FileBytes &fbytes, const DataDirectoryEntry &dde)
{
    T *addrs = (T*)dir();
    int i=0;
    int32_t bytesRead = 0;
    while (bytesRead < dde.size()) {
        // get current addrs list len
        int len = 0;
        while (addrs[i] != 0) {
            len++; // length of current row
            i++;   // index for whole table
        }

        m_list32.emplace_back(fbytes, dirOffset() + bytesRead, len);

        bytesRead += (len+1) * (int)sizeof(T); // +1 because of null terminator
        i++; // `i` was left sitting on a null-terminator, so step over
    }
}

IatDir::IatDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        if (Ident::is32bit(pe))
            readAddrList<int32_t>(fbytes, dde);
        else
            readAddrList<int64_t>(fbytes, dde);
    }
}

template <typename T> // requires variant declaration in header to link
const char* AddressList<T>::getFieldName(int index)
{
    switch (index) {
        default: return "Import Address";
    }
}

template <typename T> // requires variant declaration in header to link
const void* AddressList<T>::getFieldPtr(int index) const
{
    return (0 <= index && index < length())
        ? &addresses()[index]
        : nullptr;
}

const char* IatDir::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* IatDir::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

