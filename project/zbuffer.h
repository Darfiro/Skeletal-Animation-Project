#ifndef ZBUFFER_H
#define ZBUFFER_H

#include <QImage>
#include "triangle.h"
#include "pixel.h"

#define MAXDIST 10000000

class ZBuffer
{
    private:
        Pixel **buffer;
        int sizeX, sizeY;

    public:
        ZBuffer();
        ZBuffer(int width, int height);

        void Initialize(int width, int height);

        void FreeBuffer();

        void ProcessTriangle(Triangle &currTriangle);
        void Draw(QImage *img);
        void Clear();

        Pixel** GetBuffer();

        ~ZBuffer ();
};


#endif // ZBUFFER_H
