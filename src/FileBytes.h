#ifndef FILEBYTES_H
#define FILEBYTES_H

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "Types.h"

namespace Pepper {

static constexpr offset_t MAX_FSIZE = 512*1024*1024; // 512 MB

class FileBytes final {
public:
    FileBytes(const FileBytes& fb) = delete;
    FileBytes& operator=(const FileBytes& fb) = delete;

    FileBytes(const std::string &path);

    // member functions
    const char* bytes() const { return m_bytes.data(); }
    void readBytes(const offset_t pos, char *buf, const size_t bufLen) const;

    // static functions
    static size_t getFileSize(std::ifstream &in);

private:
    size_t m_fsize{};
    std::vector<char> m_bytes{};
};
} // namespace Pepper

#endif
