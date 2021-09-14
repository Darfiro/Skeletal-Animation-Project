#include "loader.h"

int Loader::FindCount(std::ifstream *fileIn)
{
    char buff[500];
    int index, count = 0;
    *fileIn >> buff;
    while (buff[0] != 'c')
    {
        *fileIn >> buff;
    }
    index = 7;

    for (size_t i = index; buff[i] != '"'; i++)
    {
        count += buff[i] - '0';
        count *= 10;
    }
    count /= 10;
    return count;
}

int Loader::GetName(std::ifstream *fileIn, std::vector<Joint *> joints)
{
    char buff[500];
    int index = 0, key = -1;
    char name[500];
    StringComparator comp;

    *fileIn >> buff;
    while(buff[0] != 's')
        *fileIn >> buff;
    index = 5;

    for (int i = index; buff[i] != '"'; i++)
    {
        name[i - index] = buff[i];
    }

    for (size_t i = 0; i < joints.size() && key == -1; i++)
    {
        if (comp.Compare(name, joints[i]->GetJointName()))
            key = i;
    }
    return key;
}

void Loader::SortTriangles(std::vector<Triangle>* triangleList)
{
    double z1, z2;
    for (size_t i = 0; i < triangleList->size(); i++)
    {
        (*triangleList)[i].GetMaxZ(&z1);
        for (size_t j = i + 1; j < triangleList->size(); j++)
        {
            (*triangleList)[j].GetMaxZ(&z2);
            if (z2 < z1)
            {
                Triangle tmpTriangle = (*triangleList)[i];
                (*triangleList)[i] = (*triangleList)[j];
                (*triangleList)[j] = tmpTriangle;
                z1 = z2;
            }
        }
    }
}

Loader::Loader()
{

}

void Loader::LoadMesh(std::vector<Point3D>* pointList, std::ifstream *fileIn)
{
    int count = FindCount(fileIn);

    count /= 3;

    double x, y, z;
    for (int i = 0; i < count; i++)
    {
        *fileIn >> x >> y >> z;
        Point3D *point = new Point3D(x, y, z);
        pointList->push_back(*point);
    }
}

void Loader::LoadTriangles(std::vector<Triangle>* triangleList, std::ifstream *fileIn, std::vector<Point3D>* pointList)
{
    char buff[500];
    int count = FindCount(fileIn);
    int vertex1, vertex2, vertex3, normal1, normal2, normal3;
    int currMax = 0, r, g, b;

    *fileIn >> buff;

    while(buff[1] != 'p')
    {
        *fileIn >> buff;
    }

    for (int i = 0; i < count; i++)
    {
        *fileIn >> vertex1 >> normal1 >> vertex2 >> normal2 >> vertex3 >> normal3;

        Triangle *newTriangle = new Triangle();
        newTriangle->SetVertices(&((*pointList)[vertex1]), &((*pointList)[vertex2]), &((*pointList)[vertex3]));

        triangleList->push_back(*(newTriangle));
    }
    currMax = count/20;
    r = 100;
    g = 100;
    b = 100;
    SortTriangles(triangleList);

    for (int i = 0; i < count; i++)
    {
        (*triangleList)[i].SetColor(QColor::fromRgb(r, g, b));
        if (i > currMax)
        {
            if (r >= 185)
                r -= 40;
            else
                r += 5;
            if (g >= 255)
                g -= 25;
            else
                g += 5;
            if (b >= 70)
                b -= 20;
            else
                b += 5;
            currMax += count/10;
        }
    }
}

void Loader::LoadJointsNames(std::vector<Joint *>* joints, std::ifstream *fileIn)
{
    char buff[500];
    int count = FindCount(fileIn);

    for (int i = 0; i < count; i++)
    {
        *fileIn >> buff;
        Joint *current = new Joint();
        current->SetNameKey(i, buff);
        current->SetColor(Qt::blue);
        joints->push_back(current);
    }
}

void Loader::LoadJointInverse(std::vector<Joint *>* joints, std::ifstream *fileIn)
{
    char buff[500];
    *fileIn >> buff;
    while(buff[0] != 'c')
        *fileIn >> buff;
    for (size_t i = 0; i < joints->size(); i++)
    {
        Quaternion *quat = new Quaternion();
        double **matrix = quat->GetMatrix();
        for (int i = 0; i < FOUR; i++)
            for (int j = 0; j < FOUR; j++)
                *fileIn >> matrix[i][j];
        (*joints)[i]->SetInverse(matrix);
    }
}

void Loader::LoadJointsWeight(std::vector<double>* weights, std::ifstream *fileIn)
{
    int count = FindCount(fileIn);
    double weight;

    for (int i = 0; i < count; i++)
    {
        *fileIn >> weight;
        weights->push_back(weight);
    }
}

void Loader::LoadWeightByJoint(std::vector<Joint *>* joints, std::vector<Transform> *transformVector, std::vector<double> weights, std::vector<Point3D>* pointList, std::vector<int> numJointWeight, std::ifstream *fileIn)
{
    int jointKey, weightNum;

    for (size_t i = 0; i < numJointWeight.size(); i++)
    {
        Transform *newPoint = new Transform(&((*pointList)[i]));
        for (int j = 0; j < numJointWeight[i]; j++)
        {
            *fileIn >> jointKey >> weightNum;
            newPoint->AddLink(weights[weightNum], (*joints)[jointKey]);
            //(*joints)[jointKey]->AddPointWeight(&((*pointList)[i]), weights[weightNum]);
        }
        (*transformVector).push_back(*newPoint);
    }
    for (size_t i = 0; i < joints->size(); i++)
    {
        for (size_t j = 0; j < (*transformVector).size(); j++)
        {
            if ((*transformVector)[j].IsLinked((*joints)[i]->GetJointKey()))
                (*joints)[i]->AddPointWeight(j);
        }
    }
}

void Loader::LoadJoints(Joint **rootJoint, std::ifstream *fileIn, std::vector<Joint* > joints)
{
    char buff[500];
    int nodeCount = 1;
    int rootName = GetName(fileIn, joints);
    StringComparator *comp = new StringComparator();
    int flag = 1;

    Joint *parentJoint = NULL;

    (*rootJoint) = joints[rootName];
    (*rootJoint)->SetParent(parentJoint);
    *fileIn >> buff;

    while(nodeCount)//!end)
    {
        if (flag)
        {
            while(!(comp->Compare(buff, "<matrix")))
                *fileIn >> buff;
            while (!comp->Compare(buff, "</matrix>"))
                *fileIn >> buff;
        }
        while (!(comp->Compare(buff, "<node")) && !(comp->Compare(buff, "</node>")))
            *fileIn >> buff;
        if (comp->Compare(buff, "<node"))
        {
            nodeCount++;
            rootName = GetName(fileIn, joints);
            (*rootJoint)->AddChild(joints[rootName]);
            parentJoint = (*rootJoint);
            (*rootJoint) = joints[rootName];
            (*rootJoint)->SetParent(parentJoint);
            flag = 1;
        }
        else if (comp->Compare(buff, "</node>"))
        {
            nodeCount--;
            if (nodeCount)
                (*rootJoint) = (*rootJoint)->GetParent();
            flag = 0;
            buff[0] = 'f';
        }
    }
    (*rootJoint)->FindCoordinates();
}


