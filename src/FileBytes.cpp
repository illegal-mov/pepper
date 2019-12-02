#include <cstring>
#include <fstream>
#include <memory>

#include "Exceptions.h"
#include "FileBytes.h"

using namespace Pepper;

FileBytes::FileBytes(const std::string &path)
: m_fsize(0)
, m_bytes(nullptr)
{
    // open the file
    std::ifstream in(path, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open())
        throw FailedOpen("Unable to open the file");

    // get file size
    m_fsize = getFileSize(in);
    if (m_fsize > MAX_FSIZE) {
        in.close();
        throw OversizedFile("This file exceeds the maximum file size of 512 MB");
    }

    // allocate memory, copy file bytes into memory
    std::shared_ptr<char> tmp(new char [ m_fsize ]);
    m_bytes = std::move(tmp);
    in.read(m_bytes.get(), m_fsize);
    in.close();

    if (in.fail()) {
        throw FailedRead("Unable to read the file");
    }
}

/* Copy `bufLen` file bytes starting from `pos`
 * into the memory space pointed to by `buf`
 */
void FileBytes::readBytes(size_t pos, char *buf, size_t bufLen) const
{
    if (pos < (size_t)m_fsize) {
        size_t min = (pos + bufLen <= (size_t)m_fsize)
            ? bufLen
            : (size_t)m_fsize - pos;
        memcpy(buf, &bytes().get()[pos], min);
    } else {
        memset(buf, 0, bufLen);
    }
}

long FileBytes::getFileSize(std::ifstream &in)
{
    in.seekg(0, in.end);
    long size = in.tellg();
    in.seekg(0);
    return size;
}

