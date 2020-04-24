#ifndef IHEADER_H
#define IHEADER_H

#include "../FileBytes.h"
#include "../headers/struct.h"

namespace Pepper {

/* This class defines a header in a PE file as a pointer to a copy of
 * the PE file's content and an offset into that content as the start
 * of the header data.
 */
class IHeader {
public:
    /* get char pointer to start of PE file content */
    const char* mem() const { return m_headerPtr; }

    /* get offset into PE file content where header is located */
    size_t hdrOffset() const { return m_headerOffset; };

    /* pointer to base of header */
    const void* hdr() const { return &m_headerPtr[hdrOffset()]; }

    /* only derived classes know how to return pointers to given header fields */
    virtual const void* getFieldPtr(const int fieldIndex) const = 0;

protected:
    IHeader() = default;

    IHeader(const FileBytes& fbytes, const size_t offset)
    : m_headerPtr(fbytes.bytes())
    , m_headerOffset(offset)
    {}

    IHeader(const IHeader& ih)
    : m_headerPtr(ih.m_headerPtr)
    , m_headerOffset(ih.m_headerOffset)
    {}

    IHeader& operator=(const IHeader& ih)
    {
        m_headerPtr = ih.m_headerPtr;
        m_headerOffset = ih.m_headerOffset;
        return *this;
    }

    virtual ~IHeader()
    {
        m_headerPtr = nullptr;
        m_headerOffset = 0;
    }

    const char* m_headerPtr{};
    size_t m_headerOffset{};
};
} // namespace Pepper

#endif
