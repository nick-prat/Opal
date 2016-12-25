#ifndef _RESOURCE_H
#define _RESOURCE_H

#include <string>

class IResource {
public:
    virtual ~IResource();
    
    std::string getType();

protected:
    IResource(const std::string& type);
    std::string m_type;
};

#endif // _RESOURCE_H
