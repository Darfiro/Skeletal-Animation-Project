#include "transform.h"

Transform::Transform(Point3D *vertexLink)
{
    linkedVertex = vertexLink;
}

void Transform::AddLink(double weightLink, Joint* jointLink)
{
    linkedJoint.push_back(jointLink);
    weight.push_back(weightLink);
}

void Transform::Rotate()
{
    double x = 0, y = 0, z = 0;
    Point3D point;
    for (size_t i = 0; i < weight.size(); i++)
    {
        point.SetCoordinats(linkedVertex->GetX(), linkedVertex->GetY(), linkedVertex->GetZ());
        point.RotateDotX(linkedJoint[i]->GetTransform().paramOne);
        point.RotateDotY(linkedJoint[i]->GetTransform().paramTwo);
        point.RotateDotZ(linkedJoint[i]->GetTransform().paramThree);
        x += (point.GetX() * weight[i]);
        y += (point.GetY() * weight[i]);
        z += (point.GetZ() * weight[i]);
    }
    linkedVertex->SetCoordinats(x, y, z);
}

void Transform::Move()
{
    double x = 0, y = 0, z = 0;
    Point3D point;
    for (size_t i = 0; i < weight.size(); i++)
    {
        point.SetCoordinats(linkedVertex->GetX(), linkedVertex->GetY(), linkedVertex->GetZ());
        point.MoveDot(linkedJoint[i]->GetTransform().paramOne, linkedJoint[i]->GetTransform().paramTwo, linkedJoint[i]->GetTransform().paramThree);
        x += (point.GetX() * weight[i]);
        y += (point.GetY() * weight[i]);
        z += (point.GetZ() * weight[i]);
    }
    linkedVertex->SetCoordinats(x, y, z);
}

bool Transform::IsLinked(int jointKey)
{
    bool linked = false;
    for (size_t i = 0; i < linkedJoint.size() && !linked; i++)
        if (linkedJoint[i]->GetJointKey() == jointKey)
            linked = true;
    return linked;
}

