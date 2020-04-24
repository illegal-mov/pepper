#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <vector>

#include "../Types.h"
#include "../generics/iDir.h"

/*
typedef struct _IMAGE_EXCEPTION_ENTRY32 {
    uint32_t BeginAddress;     // AVA to start of exception-handling code
    uint32_t EndAddress;       // AVA to end of exception-handling code
    uint32_t ExceptionHandler; // RVA to exception handler to execute
    uint32_t HandlerData;      // RVA to extra exception handler data
    uint32_t PrologEndAddress; // AVA to end of function prolog
} IMAGE_EXCEPTION_ENTRY32, *PIMAGE_EXCEPTION_ENTRY32;

typedef struct _IMAGE_EXCEPTION_ENTRY64 {
    uint32_t BeginAddress;      // RVA to start of exception-handling code
    uint32_t EndAddress;        // RVA to end of exception-handling code
    uint32_t UnwindInformation; // RVA to unwind information
} IMAGE_EXCEPTION_ENTRY64, *PIMAGE_EXCEPTION_ENTRY64;

typedef struct _IMAGE_EXCEPTION_ENTRY_ARM {
    uint32_t BeginAddress;        // AVA to start of exception-handling code
    uint32_t PrologLength   : 8;  // Number of instructions in prolog
    uint32_t FunctionLength : 22; // number of instructions in function
    int32_t  Is32BitCode    : 1;  // (==1) ? 32bit insns : 16bit insn
    int32_t  ExceptionFlag  : 1;  // (==1) ? handler exists : no handler
} IMAGE_EXCEPTION_ENTRY_ARM, *PIMAGE_EXCEPTION_ENTRY_ARM;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;
template <typename EntryType>
class ExceptionTableEntry;

using ExceptionTableEntry32  = ExceptionTableEntry<IMAGE_EXCEPTION_ENTRY32>;
using ExceptionTableEntry64  = ExceptionTableEntry<IMAGE_EXCEPTION_ENTRY64>;
using ExceptionTableEntryArm = ExceptionTableEntry<IMAGE_EXCEPTION_ENTRY_ARM>;

/* An element of the variable-length array of any IMAGE_EXCEPTION_ENTRY data structure.
 */
template <typename EntryType>
class ExceptionTableEntry final : public IHeader {
public:
    // defined in template specializations
    enum Fields : int {};

    ExceptionTableEntry(const FileBytes& fbytes, const offset_t raw)
    : IHeader(fbytes, raw)
    {}

    const EntryType* getStructPtr() const { return static_cast<const EntryType*>(hdr()); }
    const void* getFieldPtr(const int index) const override;

    uint32_t     beginRaw() const { return getStructPtr()->BeginAddress - *s_pCodeDiff; }
    uint32_t     endRaw()   const { return getStructPtr()->EndAddress   - *s_pCodeDiff; }
    uint32_t     codeLen()  const { return getStructPtr()->EndAddress   -  getStructPtr()->BeginAddress; }
    const void*  codePtr()  const { return &mem()[beginRaw()]; }

    static const char* getFieldName(const int index);

private:
    static size_t *s_pCodeDiff;
};

/* Variable-length array of architecture-specific exception structures.
 * Array length is given by (totalDirectorySize / sizeof(exceptionStruct))
 */
class ExceptionDir final : public IDirectory {
public:
    ExceptionDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    ~ExceptionDir() {}

    // overloaded operators
    ExceptionDir& operator=(const ExceptionDir& ed)
    {
        IDirectory::operator=(ed);
        m_exceptionTableEntries32 = ed.m_exceptionTableEntries32;
        return *this;
    }

    const std::vector<ExceptionTableEntry32>& getFunctions32() const
    {
        return m_exceptionTableEntries32;
    }

    const std::vector<ExceptionTableEntry64>& getFunctions64() const
    {
        return m_exceptionTableEntries64;
    }

    const std::vector<ExceptionTableEntryArm>& getFunctionsArm() const
    {
        return m_exceptionTableEntriesArm;
    }

    const void* getFieldPtr(const int index) const override;
    size_t length() const { return m_exceptionTableEntries32.size(); }

    static const char* getFieldName(const int index);

    // classes that need special access to s_codeDiff
    friend class ExceptionTableEntry<IMAGE_EXCEPTION_ENTRY32>;
    friend class ExceptionTableEntry<IMAGE_EXCEPTION_ENTRY64>;
    friend class ExceptionTableEntry<IMAGE_EXCEPTION_ENTRY_ARM>;

private:
    static size_t s_codeDiff; // RVAs in .pdata point to .text
    union {
        std::vector<ExceptionTableEntry32>  m_exceptionTableEntries32{};
        std::vector<ExceptionTableEntry64>  m_exceptionTableEntries64;
        std::vector<ExceptionTableEntryArm> m_exceptionTableEntriesArm;
    };

    // Append entries to the vector. Works for any type of ExceptionTable because
    // entrySize is chosen by the caller when it knows the architecture.
    template <typename EntryType>
    void appendEntries(const FileBytes& fbytes, uint32_t totalSize);
};

template<>
enum ExceptionTableEntry32::Fields : int {
    BEGIN_ADDRESS,
    END_ADDRESS,
    EXCEPTION_HANDLER,
    HANDLER_DATA,
    PROLOG_END_ADDRESS,
    _NUM_FIELDS,
};

template<>
enum ExceptionTableEntry64::Fields : int {
    BEGIN_ADDRESS,
    END_ADDRESS,
    UNWIND_INFORMATION,
    _NUM_FIELDS,
};

template<>
enum ExceptionTableEntryArm::Fields : int {
    BEGIN_ADDRESS,
    INFO,
    _NUM_FIELDS,
};
} // namespace Pepper

#endif
