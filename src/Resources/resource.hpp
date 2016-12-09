#ifndef _RESOURCE_H
#define _RESOURCE_H

#include <string>

class IResource {
public:
    IResource();
    virtual ~IResource();

    std::string GetType();

protected:
    std::string m_type;
};

#endif // _RESOURCE_H
