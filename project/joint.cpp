#include "joint.h"

void Joint::SetDot(double **matrix)
{
    double x, y, z;
    x = matrix[0][3];
    y = matrix[1][3];
    z = matrix[2][3];
    jointCoordinates.SetCoordinats(x, y, z);
}

void Joint::CopySkeletData(std::vector<Joint *> *newRootJoint)
{
    Joint *joint = new Joint();
    joint->SetNameKey(key, name);
    joint->SetColor(Qt::blue);
    for (size_t i = 0; i < pointWeights.size(); i++)
        joint->AddPointWeight(pointWeights[i]);
    joint->SetParent(NULL);
    joint->SetCoordinates(initialPose.GetX(), initialPose.GetY(), initialPose.GetZ());
    joint->AddRotation(rotation.paramOne, rotation.paramTwo, rotation.paramThree);
    (*newRootJoint).push_back(joint);
    for (size_t i = 0; i < children.size(); i++)
        children[i]->CopySkeletData(newRootJoint);
}

void Joint::CopyTreeStructure(std::vector<Joint *> *newRootJoint)
{
    int pos = 0;
    for (size_t i = 0; i < newRootJoint->size(); i++)
    {
        if ((*newRootJoint)[i]->GetJointKey() == key)
            pos = i;
    }
    for (size_t i = 0; i < children.size(); i++)
    {
        for (size_t j = 0; j < newRootJoint->size(); j++)
        {
            if ((*newRootJoint)[j]->GetJointKey() == children[i]->GetJointKey())
            {
                (*newRootJoint)[j]->SetParent((*newRootJoint)[pos]);
                (*newRootJoint)[pos]->AddChild((*newRootJoint)[j]);
            }
        }
    }
    for (size_t i = 0; i < children.size(); i++)
        children[i]->CopyTreeStructure(newRootJoint);
}

Joint::Joint()
{
    rotation.paramOne = 0;
    rotation.paramTwo = 0;
    rotation.paramThree = 0;    
    rotation.type = ROTATE;
}

void Joint::Clear()
{
    pointWeights.clear();
    name = "";
    key = -1;
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->Clear();
        children[i] = NULL;
        //free(children[i]);
    }
    parent = NULL;
    children.clear();
}

void Joint::SetInitial()
{
    initialPose.SetCoordinats(jointCoordinates.GetX(), jointCoordinates.GetY(), jointCoordinates.GetZ());
    for (size_t i = 0; i < children.size(); i++)
        children[i]->SetInitial();
}

void Joint::SetBindPose()
{
    bindPose.SetCoordinats(jointCoordinates.GetX(), jointCoordinates.GetY(), jointCoordinates.GetZ());
    for (size_t i = 0; i < children.size(); i++)
        children[i]->SetBindPose();
}

void Joint::SetColor(QColor setColor)
{
    color = setColor;
}

void Joint::SetInverse(double **matrix)
{
    inverseMatrix.CopyMatrix(matrix);
}

void Joint::SetNameKey(int number, char *jointName)
{
    StringComparator *comparator = new StringComparator();
    name = (char*)calloc(comparator->Length(jointName), sizeof(char));
    comparator->Copy(name, jointName);
    key = number;
}

void Joint::SetParent(Joint *parentJoint)
{
    parent = parentJoint;
}

void Joint::SetCoordinates(double x, double y, double z)
{
    jointCoordinates.SetCoordinats(x, y, z);
}

void Joint::AddChild(Joint *child)
{
    children.push_back(child);
}

void Joint::AddPointWeight(int link)
{
    pointWeights.push_back(link);
}

void Joint::FindCoordinates()
{
    double** bindPose = inverseMatrix.Inverse();
    SetDot(bindPose);
    for(size_t i = 0; i < children.size(); i++)
        children[i]->FindCoordinates();
}

void Joint::CreateVector(std::vector<Joint *> *vector)
{
    vector->push_back(this);
    for (int i = 0; i < children.size(); i++)
        children[i]->CreateVector(vector);
}

Point3D Joint::GetJointPosition()
{
    return jointCoordinates;
}

char *Joint::GetJointName()
{
    return name;
}

int Joint::GetJointKey()
{
    return key;
}

Joint *Joint::GetParent()
{
    return parent;
}

std::vector<Joint *> Joint::GetChildren()
{
    return children;
}

Quaternion& Joint::GetInverse()
{
    return inverseMatrix;
}

TransformValues Joint::GetTransform()
{
    return transform;
}

Point3D Joint::GetInitial()
{
    return initialPose;
}

void Joint::TransformToBindPose()
{
    jointCoordinates = bindPose;

    for(size_t i = 0; i < children.size(); i++)
        children[i]->TransformToBindPose();
}

void Joint::SetRotation(double angleX, double angleY, double angleZ)
{
    rotation.paramOne = angleX;
    rotation.paramTwo = angleY;
    rotation.paramThree = angleZ;
}

void Joint::ResizeJoint(double kx, double ky, double kz)
{
    jointCoordinates.ResizeDot(kx, ky, kz);

    for(size_t i = 0; i < children.size(); i++)
    {
        children[i]->ResizeJoint(kx, ky, kz);
    }
}

void Joint::MoveJoint(double dx, double dy, double dz)
{
    jointCoordinates.MoveDot(dx, dy, dz);

    transform.paramOne = dx;
    transform.paramTwo = dy;
    transform.paramThree = dz;
    transform.type = MOVE;

    for(size_t i = 0; i < children.size(); i++)
        children[i]->MoveJoint(dx, dy, dz);
}

void Joint::RotateJoint(double angleX, double angleY, double angleZ)
{
    jointCoordinates.RotateDotX(angleX);
    jointCoordinates.RotateDotY(angleY);
    jointCoordinates.RotateDotZ(angleZ);

    transform.paramOne = angleX;
    transform.paramTwo = angleY;
    transform.paramThree = angleZ;
    transform.type = ROTATE;

    for(size_t i = 0; i < children.size(); i++)
        children[i]->RotateJoint(angleX, angleY, angleZ);
}

void Joint::GetLinks(std::vector<int> *links)
{
    int inLinks;
    for (size_t i = 0; i < pointWeights.size(); i++)
    {
        inLinks = 0;
        (*links).push_back(pointWeights[i]);
        for (size_t j = 0; j < (*links).size() - 1 && !inLinks; j++)
        {
            if ((*links)[j] == pointWeights[i])
                inLinks++;
        }
        if (inLinks)
            (*links).pop_back();
    }
    for(size_t i = 0; i < children.size(); i++)
        children[i]->GetLinks(links);
}

void Joint::ClearTransform()
{
    transform.paramOne = 0;
    transform.paramTwo = 0;
    transform.paramThree = 0;
    transform.type = NO_TRANSFORM;
    for (size_t i = 0; i < children.size(); i++)
        children[i]->ClearTransform();
}

void Joint::AddRotation(double angleX, double angleY, double angleZ)
{
    rotation.paramOne += angleX;
    rotation.paramTwo += angleY;
    rotation.paramThree += angleZ;
}

TransformValues Joint::GetRotation()
{
    return rotation;
}

void Joint::Draw(QImage *img)
{
    double x1, x2, y1, y2;
    x1 = jointCoordinates.GetX();
    y1 = jointCoordinates.GetY();
    for (size_t i = 0; i < children.size(); i++)
    {
        x2 = children[i]->GetJointPosition().GetX();
        y2 = children[i]->GetJointPosition().GetY();
        double l = 0, dx = 0, dy = 0;
        double x = x1, y = y1;

        if (fabs(x2 - x1) > fabs(y2 - y1))
            l = fabs(x2 - x1);
        else
            l = fabs(y2 - y1);
        dx = (x2 - x1)/l;
        dy = (y2 - y1)/l;
        while (fabs(x - x1) < fabs(x2 - x1) || fabs(y - y1) < fabs(y2 - y1))
        {
            x += dx;
            y += dy;
            img->setPixel(x, y, color.rgb());
        }
        children[i]->Draw(img);
    }
}

void Joint::FindJoint(int x, int y, Joint **rotJoint, double *minimum)
{
    double xJoint = jointCoordinates.GetX(), yJoint = jointCoordinates.GetY();
    double distance = sqrt((xJoint - x)*(xJoint - x) + (yJoint - y)*(yJoint - y));
    this->SetColor(Qt::blue);
    if (*minimum > distance)
    {
        *minimum = distance;
        (*rotJoint) = this;
    }
    for (size_t i = 0; i < children.size(); i++)
        children[i]->FindJoint(x, y, rotJoint, minimum);
}

Joint *Joint::CopySkelet()
{
    Joint *joint = new Joint();
    std::vector<Joint*> newRootJoint;
    CopySkeletData(&newRootJoint);
    CopyTreeStructure(&newRootJoint);
    joint = newRootJoint[0];
    return joint;
}



void Joint::NewPositionRotation(Joint *newRoot)
{
    rotation.paramOne = newRoot->GetRotation().paramOne;
    rotation.paramTwo = newRoot->GetRotation().paramTwo;
    rotation.paramThree =  newRoot->GetRotation().paramThree;
    for (size_t i = 0; i < children.size(); i++)
        children[i]->NewPositionRotation(newRoot->GetChildren()[i]);
}

