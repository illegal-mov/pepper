#include "../Conversion.h"
#include "../Identification.h"
#include "DataDirectoryEntry.h"
#include "ExceptionDir.h"

using namespace Pepper;

int ExceptionDir::s_codeDiff = 0; // RVAs in .pdata point to .text
template<> int *FunctionTableEntry32::s_pCodeDiff  = &ExceptionDir::s_codeDiff;
template<> int *FunctionTableEntry64::s_pCodeDiff  = &ExceptionDir::s_codeDiff;
template<> int *FunctionTableEntryArm::s_pCodeDiff = &ExceptionDir::s_codeDiff;

// Append elements to the vector. Works for any type of FunctionTable because
// entrySize is chosen by the caller when it knows the architecture.
template <typename T>
void ExceptionDir::appendEntries(const FileBytes &fbytes, int32_t totalSize)
{
    size_t numEntries = (size_t)totalSize / sizeof(T);
    m_entries32.reserve(numEntries);
    for (size_t i=0; i < numEntries; i++)
        m_entries32.emplace_back(fbytes, (size_t)dirOffset() + (i * sizeof(T)));

    // Use the `BeginAddress` field of an exception structure to find the offset to code
    s_codeDiff = Convert::getRvaToRawDiff(*m_pe, *(int32_t*)m_entries32[0].getFieldPtr(0));
}

ExceptionDir::ExceptionDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        // Fill elements vector according to architecture's exception table entry size
        if (Ident::is32bit(*m_pe))
            appendEntries<IMAGE_EXCEPTION_ENTRY32>(fbytes, dde.size());
        else if (Ident::is64bit(*m_pe))
            appendEntries<IMAGE_EXCEPTION_ENTRY64>(fbytes, dde.size());
        else
            appendEntries<IMAGE_EXCEPTION_ENTRY_ARM>(fbytes, dde.size());
    }
}

template<>
const void* FunctionTableEntry32::getFieldPtr(int index) const
{
    switch (index) {
        case BEGIN_ADDRESS     : return &entry()->BeginAddress;
        case END_ADDRESS       : return &entry()->EndAddress;
        case EXCEPTION_HANDLER : return &entry()->ExceptionHandler;
        case HANDLER_DATA      : return &entry()->HandlerData;
        case PROLOG_END_ADDRESS: return &entry()->PrologEndAddress;
        default                : return nullptr;
    }
}

template<>
const char* FunctionTableEntry32::getFieldName(int index)
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
const void* FunctionTableEntry64::getFieldPtr(int index) const
{
    switch (index) {
        case BEGIN_ADDRESS     : return &entry()->BeginAddress;
        case END_ADDRESS       : return &entry()->EndAddress;
        case UNWIND_INFORMATION: return &entry()->UnwindInformation;
        default                : return nullptr;
    }
}

template<>
const char* FunctionTableEntry64::getFieldName(int index)
{
    switch (index) {
        case BEGIN_ADDRESS     : return "Pointer to Function Start";
        case END_ADDRESS       : return "Pointer to Function End";
        case UNWIND_INFORMATION: return "Pointer to Unwind Information";
        default                : return "<UNKNOWN>";
    }
}

template<>
const void* FunctionTableEntryArm::getFieldPtr(int index) const
{
    switch (index) {
        case BEGIN_ADDRESS: return   &entry()->BeginAddress;
        case INFO         : return &(&entry()->BeginAddress)[1]; // can't return address of bit fields
        default           : return nullptr;
    }
}

template<>
const char* FunctionTableEntryArm::getFieldName(int index)
{
    switch (index) {
        case BEGIN_ADDRESS: return "Pointer to Function Start";
        case INFO         : return "Exception Info";
        default           : return "<UNKNOWN>";
    }
}

const void* ExceptionDir::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

const char* ExceptionDir::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

template<>
int32_t FunctionTableEntryArm::endRaw() const
{
    return entry()->BeginAddress + entry()->FunctionLength - *s_pCodeDiff;
}

template<>
int32_t FunctionTableEntryArm::codeLen() const
{
    return entry()->FunctionLength;
}

