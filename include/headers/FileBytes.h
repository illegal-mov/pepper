#ifndef FILEBYTES_H
#define FILEBYTES_H

#include <fstream>
#include <string>
#include <vector>

#include "Types.h"

namespace Pepper {

static constexpr offset_t MAX_FILE_SIZE = 512*1024*1024; // 512 MB

class FileBytes final {
public:
    FileBytes(const FileBytes& fb) = delete;
    FileBytes& operator=(const FileBytes& fb) = delete;

    FileBytes(const std::string& path);

    const char* bytes() const { return m_fileContent.data(); }
    void readBytes(const offset_t pos, char *buf, const size_t bufLen) const;

    static size_t getFileSize(std::ifstream& in);

private:
    size_t m_fileSize{};
    std::vector<char> m_fileContent{};
};
} // namespace Pepper

#endif