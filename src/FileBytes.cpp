#include "../include/headers/FileBytes.h"

#include <cstring>
#include <fstream>
#include <iterator>

#include "../include/headers/Exceptions.h"
#include "../include/headers/Types.h"

using namespace Pepper;

FileBytes::FileBytes(const std::string& path)
{
    std::ifstream in(path, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open()) {
        throw FailedOpen("Unable to open the file");
    }

    m_fileSize = getFileSize(in);
    if (m_fileSize > MAX_FILE_SIZE) {
        in.close();
        throw OversizedFile("This file exceeds the maximum file size of 512 MB");
    }

    m_fileContent.reserve(m_fileSize);
    in >> std::noskipws;
    std::istream_iterator<char> insit(in);
    std::copy(insit, std::istream_iterator<char>(), m_fileContent.begin());
    in.close();

    if (in.fail() && (!in.eof())) {
        throw FailedRead("Unable to read the file");
    }
}

void FileBytes::readBytes(const offset_t pos, char* buf, const size_t bufLen) const
{
    if (pos < m_fileSize) {
        const size_t min = (pos + bufLen <= m_fileSize)
            ? bufLen
            : m_fileSize - pos;
        memcpy(buf, &bytes()[pos], min);
    } else {
        memset(buf, 0, bufLen);
    }
}

size_t FileBytes::getFileSize(std::ifstream& in)
{
    in.seekg(0, in.end);
    const size_t size = static_cast<size_t>(in.tellg());
    in.seekg(0);
    return size;
}

