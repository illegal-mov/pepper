#ifndef FILEBYTES_H
#define FILEBYTES_H

#include <fstream>
#include <string>
#include <vector>

#include "Types.h"
#include "Exceptions.h"

namespace Pepper {

static constexpr offset_t MAX_FILE_SIZE = 512*1024*1024; // 512 MB

class FileBytes final {
public:
    FileBytes(const FileBytes& fb) = delete;
    FileBytes& operator=(const FileBytes& fb) = delete;

    FileBytes(const std::string& path, ExceptionFlag throwFlag = ExceptionFlag::MAY_THROW);

    size_t size() const { return m_fileSize; }
    const uint8_t* bytes() const { return m_fileContent.data(); }
    Error getError() const { return m_error; }
    void readBytes(const offset_t pos, uint8_t* buf, const size_t bufLen) const;

    static size_t getFileSize(std::ifstream& in);

private:
    size_t m_fileSize{};
    std::vector<uint8_t> m_fileContent{};
    Error m_error = Error::None;
};
} // namespace Pepper

#endif
