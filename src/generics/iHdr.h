#ifndef IHEADER_H
#define IHEADER_H

#include <memory>
#include "../FileBytes.h"

namespace Pepper {

/* This class defines a header in a PE file as a pointer to a copy of
 * the PE file's content and an offset into that content as the start
 * of the header data.
 */
class IHeader {
protected:
    std::shared_ptr<const char> m_headerPtr{};
    int m_baseOffset{};

    IHeader()
    {}

    IHeader(const FileBytes &fbytes, int offset)
    : m_headerPtr(fbytes.bytes())
    , m_baseOffset(offset)
    {}

    virtual ~IHeader()
    {
        m_headerPtr = nullptr;
        m_baseOffset = 0;
    }
public:
    // get pointer to start of PE file content
    const char* mem() const { return m_headerPtr.get(); }

    // get offset into PE file content where header is located
    int hdrOffset() const { return m_baseOffset; };

    // pointer to base of header as char*
    const char* hdr() const { return &m_headerPtr.get()[hdrOffset()]; }

    // only derived classes know how to return pointers to given header fields
    virtual const void* getFieldPtr(int fieldIndex) const = 0;
};
} // namespace Pepper

#endif
