#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include <math.h>
//#include "point3d.h"
#include "joint.h"
using namespace std;

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

class Transform
{
    private:

        Point3D *linkedVertex;
        std::vector<Joint *> linkedJoint;
        std::vector<double> weight;

    public:
        Transform(Point3D *vertexLink);

        void AddLink(double weightLink, Joint *jointLink);

        void Rotate();

        void Move();

        bool IsLinked(int jointKey);
};

#endif // TRANSFORM_H
