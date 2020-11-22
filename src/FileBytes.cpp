#include "../include/headers/FileBytes.h"

#include <cstring>
#include <fstream>
#include <iterator>

#include "../include/headers/Exceptions.h"
#include "../include/headers/Types.h"

using namespace Pepper;

FileBytes::FileBytes(const std::string& path, ExceptionFlag throwFlag)
{
    std::ifstream in(path, std::ios_base::in | std::ios_base::binary);
    if (!in.is_open()) {
        m_error = Error::FailedOpen;
        if (throwFlag == ExceptionFlag::MAY_THROW) {
            throw FailedOpen("Unable to open the file");
        }
        return;
    }

    m_fileSize = getFileSize(in);
    if (m_fileSize > MAX_FILE_SIZE) {
        m_error = Error::OversizedFile;
        in.close();
        if (throwFlag == ExceptionFlag::MAY_THROW) {
            throw OversizedFile("This file exceeds the maximum file size of 512 MB");
        }
        return;
    }

    m_fileContent.reserve(m_fileSize);
    in >> std::noskipws;
    std::istream_iterator<char> insit(in);
    std::copy(insit, std::istream_iterator<char>(), m_fileContent.begin());
    in.close();

    if (in.fail() && (!in.eof())) {
        m_error = Error::FailedRead;
        if (throwFlag == ExceptionFlag::MAY_THROW) {
            throw FailedRead("Unable to read the file");
        }
        return;
    }
}

void FileBytes::readBytes(const offset_t pos, uint8_t* buf, const size_t bufLen) const
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
    const auto pos = in.tellg();
    in.seekg(0, in.end);
    const size_t size = static_cast<size_t>(in.tellg());
    in.seekg(pos);
    return size;
}

