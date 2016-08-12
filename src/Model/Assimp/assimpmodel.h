#ifndef _ASSIMPMODEL_H
#define _ASSIMPMODEL_H

enum ModelData
{
        ModelData_Vertices,
        ModelData_Normals
};

class AssimpModel
{
public:
    AssimpModel();
    ~AssimpModel();

    void* RequestData(ModelData type, int& count);

private:
    void* RequestNormals();
    void* RequestVertices();
};

#endif // _ASSIMPMODEL_H
