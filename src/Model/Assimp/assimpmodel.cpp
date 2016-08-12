#include "assimpmodel.h"

AssimpModel::AssimpModel()
{

}

AssimpModel::~AssimpModel()
{

}

void* AssimpModel::RequestData(ModelData type, int& count)
{
    switch(type)
    {
    case (ModelData_Vertices):

        return RequestVertices();
    case (ModelData_Normals):


        return RequestNormals();
    default:
        return nullptr;
    }
}

void* AssimpModel::RequestNormals()
{
    return nullptr;
}

void* AssimpModel::RequestVertices()
{
    return nullptr;
}
