#include "../../include/headers/pe_structures/ExceptionDir.h"

#include "../../include/headers/Conversion.h"
#include "../../include/headers/Identification.h"
#include "../../include/headers/pe_structures/DataDirectoryEntry.h"

using namespace Pepper;

size_t ExceptionDir::s_codeDiff = 0; // RVAs in .pdata point to .text
template<> size_t *ExceptionTableEntry32::s_pCodeDiff  = &ExceptionDir::s_codeDiff;
template<> size_t *ExceptionTableEntry64::s_pCodeDiff  = &ExceptionDir::s_codeDiff;
template<> size_t *ExceptionTableEntryArm::s_pCodeDiff = &ExceptionDir::s_codeDiff;

template <typename EntryType>
void ExceptionDir::appendEntries(const FileBytes& fbytes, const uint32_t totalSize)
{
    size_t numEntries = totalSize / sizeof(EntryType);
    m_exceptionTableEntries32.reserve(numEntries);
    for (size_t i=0; i < numEntries; i++) {
        m_exceptionTableEntries32.emplace_back(fbytes, dirOffset() + (i * sizeof(EntryType)));
    }

    // Use the `BeginAddress` field of an exception structure to find the offset to code
    s_codeDiff = Convert::getRvaToRawDiff(*m_peFile, *static_cast<const size_t*>(m_exceptionTableEntries32[0].getFieldPtr(0)));
}

ExceptionDir::ExceptionDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        if (Ident::is32bit(*m_peFile)) {
            appendEntries<IMAGE_EXCEPTION_ENTRY32>(fbytes, dde.size());
        } else if (Ident::is64bit(*m_peFile)) {
            appendEntries<IMAGE_EXCEPTION_ENTRY64>(fbytes, dde.size());
        } else {
            appendEntries<IMAGE_EXCEPTION_ENTRY_ARM>(fbytes, dde.size());
        }
    }
}

template<>
const void* ExceptionTableEntry32::getFieldPtr(const int index) const
{
    switch (index) {
        case BEGIN_ADDRESS     : return &getStructPtr()->BeginAddress;
        case END_ADDRESS       : return &getStructPtr()->EndAddress;
        case EXCEPTION_HANDLER : return &getStructPtr()->ExceptionHandler;
        case HANDLER_DATA      : return &getStructPtr()->HandlerData;
        case PROLOG_END_ADDRESS: return &getStructPtr()->PrologEndAddress;
        default                : return nullptr;
    }
}

template<>
const char* ExceptionTableEntry32::getFieldName(const int index)
{
    switch (index) {
        case BEGIN_ADDRESS     : return "Pointer to Function Start";
        case END_ADDRESS       : return "Pointer to Function End";
        case EXCEPTION_HANDLER : return "Pointer to Handler";
        case HANDLER_DATA      : return "Pointer to Handler's Data";
        case PROLOG_END_ADDRESS: return "Pointer to Function Prolog End";
        default                : return "<UNKNOWN>";
    }
}

template<>
const void* ExceptionTableEntry64::getFieldPtr(const int index) const
{
    switch (index) {
        case BEGIN_ADDRESS     : return &getStructPtr()->BeginAddress;
        case END_ADDRESS       : return &getStructPtr()->EndAddress;
        case UNWIND_INFORMATION: return &getStructPtr()->UnwindInformation;
        default                : return nullptr;
    }
}

template<>
const char* ExceptionTableEntry64::getFieldName(const int index)
{
    switch (index) {
        case BEGIN_ADDRESS     : return "Pointer to Function Start";
        case END_ADDRESS       : return "Pointer to Function End";
        case UNWIND_INFORMATION: return "Pointer to Unwind Information";
        default                : return "<UNKNOWN>";
    }
}

template<>
const void* ExceptionTableEntryArm::getFieldPtr(const int index) const
{
    switch (index) {
        case BEGIN_ADDRESS: return   &getStructPtr()->BeginAddress;
        case INFO         : return &(&getStructPtr()->BeginAddress)[1]; // can't return address of bit fields
        default           : return nullptr;
    }
}

template<>
const char* ExceptionTableEntryArm::getFieldName(const int index)
{
    switch (index) {
        case BEGIN_ADDRESS: return "Pointer to Function Start";
        case INFO         : return "Exception Info";
        default           : return "<UNKNOWN>";
    }
}

const void* ExceptionDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

const char* ExceptionDir::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

template<>
uint32_t ExceptionTableEntryArm::endRaw() const
{
    return getStructPtr()->BeginAddress + getStructPtr()->FunctionLength - *s_pCodeDiff;
}

template<>
uint32_t ExceptionTableEntryArm::codeLen() const
{
    return getStructPtr()->FunctionLength;
}

