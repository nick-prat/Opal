#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>
#include <string>

// NOTE Are there other exceptions that I might need to implement?

namespace Opal {

    class GenericException : public std::exception {
    public:
        GenericException(const std::string &error);
        virtual ~GenericException() = default;

        virtual void printError() const;
        virtual std::string getError() const;

        virtual const char* what() const noexcept override;

    protected:
        std::string m_error;
    };

    class BadResource : public GenericException {
    public:
        BadResource(const std::string &error, const std::string &resourcename = "null");

        void printError() const override;
        std::string getResourceName() const;

    private:
        std::string m_resourcename;
    };

}

#endif // _EXCEPTIONS_H
