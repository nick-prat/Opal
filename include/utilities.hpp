#ifndef _UTILITIES_H
#define _UTILITIES_H

template<typename T>
void SafeDelete(T* &object)
{
	if(object != nullptr)
	{
		delete object;
		object = nullptr;
	}
}

#endif