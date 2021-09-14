#include "model.h"

void Model::MovePart(double dx, double dy, double dz, Joint *startJoint)
{
    rootJoint->ClearTransform();
    startJoint->MoveJoint(dx, dy, dz);
    std::vector<int> vertexLinks;
    startJoint->GetLinks(&vertexLinks);
    for (size_t i = 0; i < vertexLinks.size(); i++)
    {
        transformVector[vertexLinks[i]].Move();
    }
    startJoint->ClearTransform();
    vertexLinks.clear();
}

void Model::RotatePart(double angleX, double angleY, double angleZ, Joint *startJoint, bool rotateStart)
{
    std::vector<int> vertexLinks;
    rootJoint->ClearTransform();
    startJoint->RotateJoint(angleX, angleY, angleZ);
    startJoint->GetLinks(&vertexLinks);
    for (size_t i = 0; i < vertexLinks.size(); i++)
    {
        transformVector[vertexLinks[i]].Rotate();
    }
    startJoint->ClearTransform();
    vertexLinks.clear();
}

void Model::RenderFrame(double widthD, double heightD, Joint *frame)
{
    std::vector<Joint*> jointVector;
    rootJoint->TransformToBindPose();
    rootJoint->NewPositionRotation(frame);
    rootJoint->CreateVector(&jointVector);
    for (int i = 0; i < pointList.size(); i++)
            pointList[i].SetCoordinats(bindPose[i].GetX(), bindPose[i].GetY(), bindPose[i].GetZ());
    Joint* rot, *par;
    Point3D point;
    for (int i = 1; i < jointVector.size(); i++)
    {
        rot = jointVector[i];
        par = rot->GetParent();
        point.SetCoordinats(par->GetJointPosition().GetX(), par->GetJointPosition().GetY(), par->GetJointPosition().GetZ());
        double angleX = jointVector[i]->GetRotation().paramOne;
        double angleY = jointVector[i]->GetRotation().paramTwo;
        double angleZ = jointVector[i]->GetRotation().paramThree;//-rot->GetRotation().paramOne, angleY = -rot->GetRotation().paramTwo, angleZ = -rot->GetRotation().paramThree;
        MovePart(-point.GetX(), -point.GetY(), -point.GetZ(), par);
        RotatePart(angleX, angleY, angleZ, rot, false);
        MovePart(point.GetX(), point.GetY(), point.GetZ(), par);
    }
    if (rotation.size())
    {
        MovePart(-widthD, -heightD, 0, rootJoint);
        for (std::list<TransformValues>::iterator it = rotation.begin(); it != rotation.end(); ++it)//int i = 0; i < rotation.size(); i++)
        {
            rootJoint->RotateJoint((*it).paramOne, (*it).paramTwo, (*it).paramThree);
            for(int i = 0; i < transformVector.size(); i++)
                transformVector[i].Rotate();
        }
        MovePart(widthD, heightD, 0, rootJoint);
    }
    jointVector.clear();
}

Model::Model()
{
    rootJoint = NULL;
    rotationJoint = NULL;
    currentAnimationPose = NULL;
    keyFramesChanged = false;
    needPop = false;
}

void Model::Clear()
{
    if (pointList.size())
        pointList.clear();
    if (triangleList.size())
        triangleList.clear();
    if (rootJoint)
        rootJoint->Clear();
    rootJoint = NULL;
    if (rotationJoint)
        rotationJoint = NULL;;
    if (currentAnimationPose)
        currentAnimationPose->Clear();
    currentAnimationPose = NULL;
    if (keyFrames.size())
    {
        for (size_t i = 0; i < keyFrames.size(); i++)
        {
            keyFrames[i]->Clear();
            keyFrames[i] = NULL;
        }
        keyFrames.clear();
        creator.Clear();
    }
    if (rotation.size())
        rotation.clear();
    if (inverse.size())
        inverse.clear();
}

bool Model::IsLoaded()
{
    if (pointList.size() && triangleList.size() && rootJoint)
        return true;
    else
        return false;
}

void Model::Load(std::ifstream *fileIn)
{
    char buff[500];
    bool flag = true;
    StringComparator *comparator = new StringComparator();
    std::vector<Joint*> joints;
    std::vector<double> weights;
    std::vector<int> numJointWeight;
    Loader *load = new Loader();

    while(flag)
    {
        *fileIn >> buff;
        if (comparator->Compare(buff, "<mesh>"))
        {
            load->LoadMesh(&pointList,fileIn);
        }
        else if (comparator->Compare(buff, "<triangles"))
        {
            load->LoadTriangles(&triangleList, fileIn, &pointList);
        }
        else if (comparator->Compare(buff, "<Name_array"))
        {
            load->LoadJointsNames(&joints, fileIn);
            while(!(comparator->Compare(buff, "<float_array")))
                *fileIn >> buff;
            load->LoadJointInverse(&joints, fileIn);
            while(!(comparator->Compare(buff, "<source")))
                *fileIn >> buff;
            load->LoadJointsWeight(&weights, fileIn);
        }
        else if (comparator->Compare(buff, "<vcount>"))
        {            
            for (size_t i = 0; i < pointList.size(); i++)
            {
                int num;
                *fileIn >> num;
                numJointWeight.push_back(num);
            }
        }
        else if (comparator->Compare(buff, "<v>"))
        {
            load->LoadWeightByJoint(&joints, &transformVector, weights, &pointList, numJointWeight, fileIn);
        }
        else if (comparator->Compare(buff, "<node"))
        {
            *fileIn >> buff;
            while (!(comparator->Compare(buff, "<node")))
                *fileIn >> buff;
            load->LoadJoints(&rootJoint, fileIn, joints);
            flag = false;
        }
    }
}

void Model::Draw(QImage *img, bool withMesh, bool withSkelet)
{
    if (withMesh)
    {
        ZBuffer *buffer = new ZBuffer(img->width(), img->height());
        buffer->Clear();
        for (size_t i = 0; i < triangleList.size(); i++)
        {
            buffer->ProcessTriangle(triangleList[i]);
        }

        buffer->Draw(img);
        buffer->FreeBuffer();
    }

    if (withSkelet)
        rootJoint->Draw(img);
}

void Model::Initial()
{
    for (int i = 0; i < pointList.size(); i++)
        bindPose.push_back(pointList[i]);
    rootJoint->SetInitial();
    rootJoint->SetBindPose();
}

void Model::Resize(double kx, double ky, double kz)
{
    for (size_t i = 0; i < pointList.size(); i++)
        pointList[i].ResizeDot(kx, ky, kz);
    rootJoint->ResizeJoint(kx, ky, kz);
}

void Model::Move(double dx, double dy, double dz)
{
    MovePart(dx, dy, dz, rootJoint);
}

void Model::Rotate(double angleX, double angleY, double angleZ)
{
    RotatePart(angleX, angleY, angleZ, rootJoint, true);
}

void Model::AddRotation(double angleX, double angleY, double angleZ)
{
    TransformValues tmp, tmpMin;
    tmp.paramOne = angleX;
    tmp.paramTwo = angleY;
    tmp.paramThree = angleZ;
    if (!angleX)
        tmpMin.paramOne = angleX;
    else
        tmpMin.paramOne = -angleX;
    if (!angleY)
        tmpMin.paramTwo = angleY;
    else
        tmpMin.paramTwo = -angleY;
    if (!angleZ)
        tmpMin.paramThree = angleZ;
    else
        tmpMin.paramThree = -angleZ;
    rotation.push_back(tmp);
    inverse.push_front(tmpMin);
}

void Model::SetRotationJoint(int x, int y)
{
    double minimum = MAXDIST;
    rootJoint->FindJoint(x, y, &rotationJoint, &minimum);
    if (rotationJoint->GetParent())
        rotationJoint->GetParent()->SetColor(Qt::red);
    else
        rotationJoint->SetColor(Qt::red);
}

void Model::RotateJoint(double xCurr, double yCurr, double xPrev, double yPrev, int width, int height, int axis)//double angle, int axis, int width, int height)
{
    rootJoint->ClearTransform();
    Point3D point;
    Joint* rot;
    bool rotateStart = false;
    std::pair<double, double> aVector, bVector;
    double sp, cp, aLength, bLength, cosinus = 1, sinus = 0, angle = 0;
    int sign = 1;
    if (rotationJoint->GetParent())
    {
        point = rotationJoint->GetParent()->GetInitial();
        rot = rotationJoint->GetParent();

        aVector.first = point.GetX() - xCurr;
        aVector.second = point.GetY() - yCurr;
        bVector.first = point.GetX() - xPrev;
        bVector.second = point.GetY() - yPrev;
        aLength = sqrt(aVector.first*aVector.first + aVector.second*aVector.second);
        bLength = sqrt(bVector.first*bVector.first + bVector.second*bVector.second);
        sp = aVector.first*bVector.first + aVector.second*bVector.second;
        cp = bVector.first*aVector.second - bVector.second*aVector.first;
        if (aLength*bLength)
        {
            cosinus = sp/(aLength*bLength);
            sinus = cp/(aLength*bLength);
        }
        int cosInt = (int)(cosinus * 1000000);

        cosinus = cosInt/1000000.0;

        if (sinus > 0)
            sign = -1;

        cosinus = cosInt/1000000.0;

        if (cosinus < 1 && cosinus > -1)
            angle = (-1)*(sign)*acos(cosinus);
        else if (cosinus <= -1)
            angle = -M_PI;

        double angleX = 0, angleY = 0, angleZ = 0;
        if (axis == X_AXIS)
            angleX = angle;
        else if (axis == Y_AXIS)
            angleY = angle;
        else
            angleZ = angle;

        if (inverse.size())
        {
            MovePart(-width/2, -height/2, 0, rootJoint);
            for (std::list<TransformValues>::iterator it = inverse.begin(); it != inverse.end(); ++it)
            {
                rootJoint->RotateJoint((*it).paramOne, (*it).paramTwo, (*it).paramThree);
                for(int i = 0; i < transformVector.size(); i++)
                    transformVector[i].Rotate();
            }
            MovePart(width/2, height/2, 0, rootJoint);
        }

        MovePart(-point.GetX(), -point.GetY(), -point.GetZ(), rot);

        RotatePart(angleX, angleY, angleZ, rotationJoint, rotateStart);

        MovePart(point.GetX(), point.GetY(), point.GetZ(), rot);

        rotationJoint->AddRotation(angleX, angleY, angleZ);

        rootJoint->SetInitial();
        if (rotation.size())
        {
            MovePart(-width/2, -height/2, 0, rootJoint);
            for (std::list<TransformValues>::iterator it = rotation.begin(); it != rotation.end(); ++it)//int i = 0; i < rotation.size(); i++)
            {
                rootJoint->RotateJoint((*it).paramOne, (*it).paramTwo, (*it).paramThree);
                for(int i = 0; i < transformVector.size(); i++)
                    transformVector[i].Rotate();
            }
            MovePart(width/2, height/2, 0, rootJoint);
        }
    }
}

bool Model::RotationJointSet()
{
    if (rotationJoint)
        return true;
    else
        return false;
}

void Model::AddFrame(int position, bool pushBack)
{
    keyFramesChanged = true;
    Joint* joint = new Joint();
    joint = rootJoint->CopySkelet();
    if (pushBack)
        keyFrames.push_back(joint);
    else
        keyFrames.insert(keyFrames.begin() + position + 1, joint);
}

void Model::DeleteFrame(int index)
{
    if (keyFrames.size() == 1)
    {
        keyFrames[keyFrames.size() - 1]->Clear();
        keyFrames[keyFrames.size() - 1] = NULL;
        keyFrames.clear();
    }
    else
    {
        keyFramesChanged = true;
        keyFrames[index]->Clear();
        keyFrames[index] = NULL;
        for (size_t i = index; i < keyFrames.size() - 1; i++)
            keyFrames[i] = keyFrames[i + 1];
        keyFrames.pop_back();
    }
}

void Model::DeleteAllFrames()
{    
    if (keyFrames.size())
    {
        keyFramesChanged = true;
        int size = keyFrames.size();
        for (size_t i = 0; i < size; i++)
        {
            keyFrames[i]->Clear();
            keyFrames[i] = NULL;
        }
        keyFrames.clear();
    }
}

int Model::GetKeyFrameCount(int type)
{
    if (type == KEY_FRAME)
        return keyFrames.size();
}

std::vector<Joint *> Model::GetKeyFrames()
{
    return keyFrames;
}

void Model::Render(int pos, double widthD, double heightD, int type)
{
    if (type == KEY_FRAME)
    {
        RenderFrame(widthD, heightD, keyFrames[pos]);
    }
    else if (type == ANIMATION)
    {
        RenderFrame(widthD, heightD, currentAnimationPose);
    }
}

void Model::CompileAnimation(long duration, int frameRate)
{
    if (keyFramesChanged)
    {
        keyFramesChanged = false;
        creator.Clear();
        creator.SetInitialKeyFrames(keyFrames);
        needPop = true;
    }
    if (creator.GetDuration() != duration)
        creator.SetDuration(duration);
    if (creator.GetFrameRate() != frameRate)
        creator.SetFrameRate(frameRate);
    creator.FillSpline();
}

void Model::CreateFrame(long moment)
{
    if (currentAnimationPose)
        currentAnimationPose->Clear();
    currentAnimationPose = NULL;
    currentAnimationPose = creator.CreateFrame(moment);
}

int Model::GetAnimationStatus(long duration, int frameRate)
{
    if (keyFramesChanged || creator.GetFrameRate() != frameRate || creator.GetDuration() != duration)
        return NEW_ANIMATION;
    else
        return OLD_ANIMATION;
}

