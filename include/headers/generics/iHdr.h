#ifndef IHEADER_H
#define IHEADER_H

#include "../FileBytes.h"
#include "../pe_structures/struct.h"

namespace Pepper {

/* This class defines a header in a PE file as a pointer to a copy of
 * the PE file's content and an offset into that content as the start
 * of the header data.
 */
class IHeader {
public:
    const uint8_t* mem() const { return m_headerPtr; }

    size_t hdrOffset() const { return m_headerOffset; };

    const void* hdr() const { return &m_headerPtr[hdrOffset()]; }

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

    const uint8_t* m_headerPtr{};
    size_t m_headerOffset{};
};
} // namespace Pepper

#endif
