#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>
#include <string>

class GenericException : public std::exception {
public:
    GenericException(const std::string& error);
    virtual ~GenericException();

    virtual void PrintError() const;
    virtual std::string GetError() const;

    virtual const char* what() const noexcept override;

protected:
    std::string m_error;
};

class BadResource : public GenericException {
public:
    BadResource(const std::string& error, const std::string& filename = "null");
    ~BadResource();

    std::string GetFilename() const;

private:
    std::string m_filename;
};

#endif // _EXCEPTIONS_H
