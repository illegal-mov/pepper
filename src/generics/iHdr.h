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
    const char* m_headerPtr{};
    size_t m_baseOffset{};

    IHeader() = default;

    IHeader(const FileBytes &fbytes, const size_t offset)
    : m_headerPtr(fbytes.bytes())
    , m_baseOffset(offset)
    {}

    IHeader(const IHeader& ih)
    : m_headerPtr(ih.m_headerPtr)
    , m_baseOffset(ih.m_baseOffset)
    {}

    const IHeader& operator=(const IHeader& ih)
    {
        m_headerPtr = ih.m_headerPtr;
        m_baseOffset = ih.m_baseOffset;
        return *this;
    }

    virtual ~IHeader()
    {
        m_headerPtr = nullptr;
        m_baseOffset = 0;
    }
public:
    // get pointer to start of PE file content
    const char* mem() const { return m_headerPtr; }

    // get offset into PE file content where header is located
    size_t hdrOffset() const { return m_baseOffset; };

    // pointer to base of header as char*
    const void* hdr() const { return &m_headerPtr[hdrOffset()]; }

    // only derived classes know how to return pointers to given header fields
    virtual const void* getFieldPtr(const int fieldIndex) const = 0;
};
} // namespace Pepper

#endif
