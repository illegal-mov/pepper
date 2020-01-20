#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <vector>

#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_EXCEPTION_ENTRY32 {
    int32_t BeginAddress;     // AVA to start of exception-handling code
    int32_t EndAddress;       // AVA to end of exception-handling code
    int32_t ExceptionHandler; // RVA to exception handler to execute
    int32_t HandlerData;      // RVA to extra exception handler data
    int32_t PrologEndAddress; // AVA to end of function prolog
} IMAGE_EXCEPTION_ENTRY32, *PIMAGE_EXCEPTION_ENTRY32;

typedef struct _IMAGE_EXCEPTION_ENTRY64 {
    int32_t BeginAddress;      // RVA to start of exception-handling code
    int32_t EndAddress;        // RVA to end of exception-handling code
    int32_t UnwindInformation; // RVA to unwind information
} IMAGE_EXCEPTION_ENTRY64, *PIMAGE_EXCEPTION_ENTRY64;

typedef struct _IMAGE_EXCEPTION_ENTRY_ARM {
    int32_t BeginAddress;        // AVA to start of exception-handling code
    int32_t PrologLength   : 8;  // Number of instructions in prolog
    int32_t FunctionLength : 22; // number of instructions in function
    int32_t Is32BitCode    : 1;  // (==1) ? 32bit insns : 16bit insn
    int32_t ExceptionFlag  : 1;  // (==1) ? handler exists : no handler
} IMAGE_EXCEPTION_ENTRY_ARM, *PIMAGE_EXCEPTION_ENTRY_ARM;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;
template <typename T>
class FunctionTableEntry;

typedef FunctionTableEntry<IMAGE_EXCEPTION_ENTRY32>   FunctionTableEntry32;
typedef FunctionTableEntry<IMAGE_EXCEPTION_ENTRY64>   FunctionTableEntry64;
typedef FunctionTableEntry<IMAGE_EXCEPTION_ENTRY_ARM> FunctionTableEntryArm;

/* An element of the variable-length array of any IMAGE_EXCEPTION_ENTRY data structure.
 */
template <typename T>
class FunctionTableEntry final : public IHeader {
private:
    static size_t *s_pCodeDiff;
public:
    // defined in template specializations
    enum Fields : int {};

    FunctionTableEntry(const FileBytes &fbytes, const size_t raw)
    : IHeader(fbytes, raw)
    {}

    // member functions
    const T* entry() const { return (T*)hdr(); }
    const void* getFieldPtr(const int index) const override;

    uint32_t     beginRaw() const { return entry()->BeginAddress - *s_pCodeDiff; }
    uint32_t     endRaw()   const { return entry()->EndAddress   - *s_pCodeDiff; }
    uint32_t     codeLen()  const { return entry()->EndAddress   -  entry()->BeginAddress; }
    const void*  codePtr()  const { return &mem()[beginRaw()]; }

    // static functions
    static const char* getFieldName(const int index);
};

/* Variable-length array of architecture-specific exception structures.
 * Array length is given by (totalDirectorySize / sizeof(exceptionStruct))
 */
class ExceptionDir final : public IDirectory {
private:
    static size_t s_codeDiff; // RVAs in .pdata point to .text
    union {
        std::vector<FunctionTableEntry32>  m_entries32{};
        std::vector<FunctionTableEntry64>  m_entries64;
        std::vector<FunctionTableEntryArm> m_entriesArm;
    };

    // Append entries to the vector. Works for any type of FunctionTable because
    // entrySize is chosen by the caller when it knows the architecture.
    template <typename T>
    void appendEntries(const FileBytes &fbytes, uint32_t totalSize);
public:
    ExceptionDir()
    : IDirectory()
    {}

    ExceptionDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    ~ExceptionDir() {}

    // overloaded operators
    const ExceptionDir& operator=(const ExceptionDir &ed)
    {
        IDirectory::operator=(ed);
        m_entries32 = ed.m_entries32;
        return *this;
    }

    // member functions
    const std::vector<FunctionTableEntry32>& table32() const
    {
        return m_entries32;
    }

    const std::vector<FunctionTableEntry64>& table64() const
    {
        return m_entries64;
    }

    const std::vector<FunctionTableEntryArm>& tableArm() const
    {
        return m_entriesArm;
    }

    const void* getFieldPtr(const int index) const override;
    size_t length() const { return m_entries32.size(); }

    // static functions
    static const char* getFieldName(const int index);

    // classes that need special access to s_codeDiff
    friend class FunctionTableEntry<IMAGE_EXCEPTION_ENTRY32>;
    friend class FunctionTableEntry<IMAGE_EXCEPTION_ENTRY64>;
    friend class FunctionTableEntry<IMAGE_EXCEPTION_ENTRY_ARM>;
};

template<>
enum FunctionTableEntry32::Fields : int {
    BEGIN_ADDRESS,
    END_ADDRESS,
    EXCEPTION_HANDLER,
    HANDLER_DATA,
    PROLOG_END_ADDRESS,
    _NUM_FIELDS,
};

template<>
enum FunctionTableEntry64::Fields : int {
    BEGIN_ADDRESS,
    END_ADDRESS,
    UNWIND_INFORMATION,
    _NUM_FIELDS,
};

template<>
enum FunctionTableEntryArm::Fields : int {
    BEGIN_ADDRESS,
    INFO,
    _NUM_FIELDS,
};
} // namespace Pepper

#endif
