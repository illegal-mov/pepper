#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

namespace Pepper {

/* Base class for exceptions */
class PeException : public std::exception
{
protected:
    std::string m_error;
public:
    PeException(const std::string& error)
    : m_error(error) {}
    const char* getError() const { return m_error.c_str(); }
};

class BadSignature : public PeException
{
public:
    BadSignature(const std::string& error)
    : PeException(error) {}
};

class FailedOpen : public PeException
{
public:
    FailedOpen(const std::string& error)
    : PeException(error) {}
};

class FailedRead : public PeException
{
public:
    FailedRead(const std::string& error)
    : PeException(error) {}
};

class GenericError : public PeException
{
public:
    GenericError(const std::string& error)
    : PeException(error) {}
};

class OversizedFile : public PeException
{
public:
    OversizedFile(const std::string& error)
    : PeException(error) {}
};
} // namespace Pepper

#endif
