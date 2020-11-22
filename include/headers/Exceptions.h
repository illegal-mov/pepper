#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <map>
#include <string>

namespace Pepper {

enum class ExceptionFlag {
    NO_THROW,
    MAY_THROW,
};

enum class Error {
    None,
    BadSignature,
    FailedOpen,
    FailedRead,
    GenericError,
    OversizedFile,
};

#define ENUM_TO_STR(ENUM, FIELD) ENUM::FIELD, #FIELD
std::map<Error, const char*> const kErrorStrMap {
    {ENUM_TO_STR(Error, None)},
    {ENUM_TO_STR(Error, BadSignature)},
    {ENUM_TO_STR(Error, FailedOpen)},
    {ENUM_TO_STR(Error, FailedRead)},
    {ENUM_TO_STR(Error, GenericError)},
    {ENUM_TO_STR(Error, OversizedFile)},
};

class PeException : public std::exception
{
public:
    PeException(const std::string& error)
    : m_error(error) {}
    virtual ~PeException() = default;
    const char* getError() const { return m_error.c_str(); }

protected:
    std::string m_error;
};

class BadSignature final : public PeException
{
public:
    BadSignature(const std::string& error)
    : PeException(error) {}
};

class FailedOpen final : public PeException
{
public:
    FailedOpen(const std::string& error)
    : PeException(error) {}
};

class FailedRead final : public PeException
{
public:
    FailedRead(const std::string& error)
    : PeException(error) {}
};

class GenericError final : public PeException
{
public:
    GenericError(const std::string& error)
    : PeException(error) {}
};

class OversizedFile final : public PeException
{
public:
    OversizedFile(const std::string& error)
    : PeException(error) {}
};
} // namespace Pepper

#endif
