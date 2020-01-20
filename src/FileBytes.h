#ifndef FILEBYTES_H
#define FILEBYTES_H

#include <fstream>
#include <memory>
#include <string>

static constexpr int MAX_FSIZE = 512*1024*1024; // 512 MB

namespace Pepper {

class FileBytes final {
private:
    size_t m_fsize;
    std::shared_ptr<char> m_bytes;
public:
    FileBytes(const FileBytes& fb) = delete;
    FileBytes& operator=(const FileBytes& fb) = delete;

    FileBytes(const std::string &path);

    // member functions
    const std::shared_ptr<char>& bytes() const { return m_bytes; }
    void readBytes(const size_t pos, char *buf, const size_t bufLen) const;

    // static functions
    static size_t getFileSize(std::ifstream &in);
};
} // namespace Pepper

#endif
