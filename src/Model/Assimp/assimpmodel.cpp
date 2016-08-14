#include "assimpmodel.h"

AssimpModel::AssimpModel()
{

}

AssimpModel::~AssimpModel()
{

}

void* AssimpModel::RequestData(ModelData type, int& count) const
{
    switch(type)
    {
    case (Vertices):

        return RequestVertices();
    case (Normals):


        return RequestNormals();
    default:
        return nullptr;
    }
}

void* AssimpModel::RequestNormals() const
{
    return nullptr;
}

void* AssimpModel::RequestVertices() const
{

}
