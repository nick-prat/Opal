#ifndef _ASSIMPMODEL_H
#define _ASSIMPMODEL_H

class AssimpModel
{
public:
    enum ModelData
    {
            Vertices,
            Normals
    };

    AssimpModel();
    ~AssimpModel();

    void* RequestData(ModelData type, int& count) const;

private:
    void* RequestNormals() const;
    void* RequestVertices() const;
};

#endif // _ASSIMPMODEL_H
