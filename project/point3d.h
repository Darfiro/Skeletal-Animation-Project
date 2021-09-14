#ifndef POINT3D_H
#define POINT3D_H

#include <math.h>
class Point3D
{
    private:
        double x;
        double y;
        double z;
    public:
        Point3D();
        Point3D(double xAxis, double yAxis, double zAxis);

        void SetPoint(Point3D &point);
        void SetCoordinats(double xAxis, double yAxis, double zAxis);
        void SetX(double xAxis);
        void SetY(double yAxis);
        void SetZ(double zAxis);
        void SetClear();

        double& GetX();
        double& GetY();
        double& GetZ();

        void RotateDotX(double angle);
        void RotateDotY(double angle);
        void RotateDotZ(double angle);

        void ResizeDot(double kx, double ky, double kz);
        void MoveDot(double dx, double dy, double dz);

        ~Point3D();
};

#endif // POINT3D_H
