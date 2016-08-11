#include "utilities.h"

Utilities::Exception::Exception(const unsigned char code, std::string error)
{
    this->code = code;
    this->error = error;
}

Utilities::Exception::~Exception()
{}

void Utilities::Exception::PrintError()
{
    std::cout << "Error (" << code << "): " << error << std::endl;
}

void Utilities::Exception::SetCode(int code)
{
    this->code = code;
}

int Utilities::Exception::GetCode()
{
    return this->code;
}

void Utilities::Exception::SetError(std::string error)
{
    this->error = error;
}

std::string Utilities::Exception::GetError()
{
    return this->error;
}
