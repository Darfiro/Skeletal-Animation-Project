#ifndef MODEL_H
#define MODEL_H

#include <QImage>
#include <stdlib.h>
#include "zbuffer.h"
#include "loader.h"
#include "animation.h"

#define NO_JOINT -4
#define NO_FRAMES -7
#define KEY_FRAME 5
#define ANIMATION 6
#define NEW_ANIMATION 7
#define OLD_ANIMATION 8

class Model
{
    private:
        std::vector<Point3D> pointList;  // мэш
        std::vector<Point3D> bindPose;

        std::vector<Triangle> triangleList;  // треугольники
        Joint* rootJoint;  // сустав        
        std::vector<Transform> transformVector;
        std::vector<Joint*> keyFrames;
        bool keyFramesChanged;
        bool needPop;
        Animation creator;
        Joint* currentAnimationPose;

        Joint *rotationJoint;  // текущий сустав, который вращает пользователь
        std::list<TransformValues> rotation;
        std::list<TransformValues> inverse;

        void MovePart(double dx, double dy, double dz, Joint *startJoint);
        void RotatePart(double angleX, double angleY, double angleZ, Joint *startJoint, bool rotateStart);
        void RenderFrame(double widthD, double heightD, Joint* frame);

    public:
        Model();

        void Clear();

        bool IsLoaded();

        void Load(std::ifstream *fileIn);
        void Draw(QImage *img, bool withMesh, bool withSkelet);

        void Initial();
        void Resize(double kx, double ky, double kz);
        void Move(double dx, double dy, double dz);
        void Rotate(double angleX, double angleY, double angleZ);
        void AddRotation(double angleX, double angleY, double angleZ);

        void SetRotationJoint(int x, int y);
        void RotateJoint(double xCurr, double yCurr, double xPrev, double yPrev, int width, int height, int axis);

        bool RotationJointSet();

        void AddFrame(int position, bool pushBack);
        void DeleteFrame(int index);
        void DeleteAllFrames();
        int GetKeyFrameCount(int type);
        std::vector<Joint*> GetKeyFrames();
        void Render(int pos, double widthD, double heightD, int type);

        void CompileAnimation(long duration, int frameRate);
        void CreateFrame(long moment);
        int GetAnimationStatus(long duration, int frameRate);
};

#endif // MODEL_H
