#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>
#include <string>

class generic_exception : public std::exception {
public:
    generic_exception(const std::string& error);
    virtual ~generic_exception();

    virtual void PrintError() const;
    virtual std::string GetError() const;

    virtual const char* what() const noexcept override;

protected:
    std::string m_error;
};

class bad_resource : public generic_exception {
public:
    bad_resource(const std::string& error, const std::string& resourcename = "null");
    ~bad_resource();

    void PrintError() const override;
    std::string GetResourceName() const;

private:
    std::string m_resourcename;
};

#endif // _EXCEPTIONS_H
