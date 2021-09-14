#include "animation.h"

void Animation::RotateCurrent()
{
    std::vector<Joint*> jointVector;
    currentPose->CreateVector(&jointVector);
    Joint* rot;
    Point3D point;
    for (int i = 0; i < jointVector.size(); i++)
    {
        rot = jointVector[i];
        point.SetCoordinats(rot->GetJointPosition().GetX(), rot->GetJointPosition().GetY(), rot->GetJointPosition().GetZ());
        double angleX = jointVector[i]->GetRotation().paramOne;
        double angleY = jointVector[i]->GetRotation().paramTwo;
        double angleZ = jointVector[i]->GetRotation().paramThree;
        rot->MoveJoint(-point.GetX(), -point.GetY(), -point.GetZ());
        rot->RotateJoint(angleX, angleY, angleZ);
        rot->MoveJoint(point.GetX(), point.GetY(), point.GetZ());
    }
}

Animation::Animation()
{
    currentPose = NULL;
    frameRate = 0;
    animationDuration = 0;
}

Animation::Animation(std::vector<Joint *> initialFrames, long duration, int rate)
{
    initialKeyFrames = initialFrames;
    animationDuration = duration;
    frameRate = rate;
    currentPose = initialFrames[0]->CopySkelet();
}

void Animation::SetInitialKeyFrames(std::vector<Joint *> initialFrames)
{
    for (int i = 0; i < initialFrames.size(); i++)
    {
        Joint *joint = initialFrames[i]->CopySkelet();
        initialKeyFrames.push_back(joint);
    }
    Joint *joint = initialKeyFrames[0]->CopySkelet();
    initialKeyFrames.push_back(joint);
    currentPose = initialFrames[0]->CopySkelet();
}

void Animation::SetDuration(long duration)
{
    animationDuration = duration;
    for(size_t i = 0; i < splinesForEach.size(); i++)
        splinesForEach[i].Clear();
    if (splinesForEach.size())
        splinesForEach.clear();
}

void Animation::SetFrameRate(int rate)
{
    frameRate = rate;
    for(size_t i = 0; i < splinesForEach.size(); i++)
        splinesForEach[i].Clear();
    if (splinesForEach.size())
        splinesForEach.clear();
}

long Animation::GetDuration()
{
    return animationDuration;
}

int Animation::GetFrameRate()
{
    return frameRate;
}

void Animation::Clear()
{
    for (size_t i = 0; i < initialKeyFrames.size(); i++)
    {
        initialKeyFrames[i]->Clear();
        initialKeyFrames[i] = NULL;
    }
    if (initialKeyFrames.size())
        initialKeyFrames.clear();
    //currentPose = NULL;
    for(size_t i = 0; i < splinesForEach.size(); i++)
        splinesForEach[i].Clear();
    if (splinesForEach.size())
        splinesForEach.clear();
}

void Animation::FillSpline()
{
    std::vector<Joint*> vector;
    double deltaTime = animationDuration/(initialKeyFrames.size()*1.0 - 1);
    double currentTime = 0;
    if (!splinesForEach.size())
    {
        initialKeyFrames[0]->CreateVector(&vector);
        for (size_t i = 0; i < vector.size(); i++)
        {
            Spline *current = new Spline();
            splinesForEach.push_back(*current);
        }
        vector.clear();
    }
    for (size_t i = 0; i < initialKeyFrames.size(); i++)
    {
        initialKeyFrames[i]->CreateVector(&vector);
        for (size_t j = 0; j < vector.size(); j++)
        {
            TransformValues rotation = vector[j]->GetRotation();
            splinesForEach[j].AddAngleTime(rotation.paramOne, rotation.paramTwo, rotation.paramThree, currentTime);
        }
        currentTime += deltaTime;
        vector.clear();
    }
    for (size_t i = 0; i < splinesForEach.size(); i++)
    {
        splinesForEach[i].MakeCoefficients();
    }
}

void Animation::CreateAnimation()
{
    long msPerFrame = 1000/frameRate;
    std::vector<Joint*> vector;
    currentPose->CreateVector(&vector);
    FillSpline();    
    for (long currentTime = 0; currentTime < animationDuration; currentTime += msPerFrame)
    {
        for (size_t i = 0; i < splinesForEach.size(); i++)
        {
            TransformValues rotation = splinesForEach[i].GetAngles(currentTime);
            vector[i]->SetRotation(rotation.paramOne, rotation.paramTwo, rotation.paramThree);
        }
        RotateCurrent();
        Joint *newPose = currentPose->CopySkelet();
        animationFrames.push_back(newPose);
    }
    splinesForEach.clear();
}

Joint* Animation::CreateFrame(long moment)
{
    std::vector<Joint*> vector;
    currentPose->CreateVector(&vector);
    for (size_t i = 0; i < splinesForEach.size(); i++)
    {
        TransformValues rotation = splinesForEach[i].GetAngles(moment);
        vector[i]->SetRotation(rotation.paramOne, rotation.paramTwo, rotation.paramThree);
    }
    RotateCurrent();
    Joint *newPose = currentPose->CopySkelet();
    vector.clear();
    return newPose;
}

std::vector<Joint *> Animation::GetAnimation()
{
    return animationFrames;
}

Animation::~Animation()
{
    for (size_t i = 0; i < initialKeyFrames.size(); i++)
        initialKeyFrames[i]->Clear();
    for(size_t i = 0; i < splinesForEach.size(); i++)
        splinesForEach[i].Clear();
    if (splinesForEach.size())
        splinesForEach.clear();
}
