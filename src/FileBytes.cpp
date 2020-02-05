#include <cstring>
#include <fstream>
#include <iterator>
#include <memory>

#include "Exceptions.h"
#include "FileBytes.h"

using namespace Pepper;

FileBytes::FileBytes(const std::string &path)
{
    // open the file
    std::ifstream in(path, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open()) {
        throw FailedOpen("Unable to open the file");
    }

    // get file size
    m_fsize = getFileSize(in);
    if (m_fsize > MAX_FSIZE) {
        in.close();
        throw OversizedFile("This file exceeds the maximum file size of 512 MB");
    }

    // allocate memory, copy file bytes into memory
    m_bytes.reserve(m_fsize);
    in >> std::noskipws;
    std::istream_iterator<char> insit(in);
    std::copy(insit, std::istream_iterator<char>(), m_bytes.begin());
    in.close();

    if (in.fail() && (!in.eof())) {
        throw FailedRead("Unable to read the file");
    }
}

/* Copy `bufLen` file bytes starting from `pos`
 * into the memory space pointed to by `buf`
 */
void FileBytes::readBytes(const size_t pos, char *buf, const size_t bufLen) const
{
    if (pos < m_fsize) {
        const size_t min = (pos + bufLen <= m_fsize)
            ? bufLen
            : m_fsize - pos;
        memcpy(buf, &bytes()[pos], min);
    } else {
        memset(buf, 0, bufLen);
    }
}

size_t FileBytes::getFileSize(std::ifstream &in)
{
    in.seekg(0, in.end);
    const size_t size = static_cast<size_t>(in.tellg());
    in.seekg(0);
    return size;
}

