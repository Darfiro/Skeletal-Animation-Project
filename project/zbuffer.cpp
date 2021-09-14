#include "zbuffer.h"

#include <stdio.h>

ZBuffer::ZBuffer()
{
    sizeX = 0;
    sizeY = 0;
    if (buffer)
        FreeBuffer();
    else
        buffer = NULL;
}

ZBuffer::ZBuffer(int width, int height)
{
    sizeX = width;
    sizeY = height;
    buffer = (Pixel**)calloc(sizeX, sizeof(Pixel*));
    for(int i = 0; i < sizeX; i++)
    {
        buffer[i] = (Pixel*)calloc(sizeY, sizeof(Pixel));
    }
}

void ZBuffer::Initialize(int width, int height)
{
    sizeX = width;
    sizeY = height;
    buffer = (Pixel**)calloc(sizeX, sizeof(Pixel*));
    for(int i = 0; i < sizeX; i++)
    {
        buffer[i] = (Pixel*)calloc(sizeY, sizeof(Pixel));
    }
}

void ZBuffer::FreeBuffer()
{
    if (buffer)
    {
        for ( int i = 0; i < sizeX; i++)
        {
            if (buffer[i])
                free(buffer[i]);
        }
        if (buffer)
            free(buffer);
    }
}

void ZBuffer::ProcessTriangle(Triangle &currTriangle)
{
    Intersaction currIntersaction;
    int yStart, yFinish;
    double A, C;
    int x1, x2;
    double z1, z2, zCurr, dz = 0;
    double x[VERTEXCOUNT], y[VERTEXCOUNT], z[VERTEXCOUNT];

    currTriangle.Sort();
    for (int i = 0; i < VERTEXCOUNT; i++ )
    {
        x[i] = currTriangle.GetTriangle()[i]->GetX();
        y[i] = currTriangle.GetTriangle()[i]->GetY();
        z[i] = currTriangle.GetTriangle()[i]->GetZ();
    }

    currTriangle.ClearRast();
    currTriangle.LineRast(x[1], y[1], z[1], x[0], y[0], z[0]);
    currTriangle.LineRast(x[2], y[2], z[2], x[0], y[0], z[0]);
    currTriangle.LineRast(x[2], y[2], z[2], x[1], y[1], z[1]);

    yStart = int(y[0]);
    yFinish = int(y[2]);

    A = (y[1] - y[0])*(z[2] - z[0]) - (y[2] - y[0])*(z[1] - z[0]);
    C = (x[1] - x[0])*(y[2] - y[0]) - (x[2] - x[0])*(y[1] - y[0]);

    for (int i = yStart; i <= yFinish; i++)
    {
        currIntersaction = currTriangle.GetIntersaction(i);


        x1 = currIntersaction.start.first;
        z1 = currIntersaction.start.second;
        x2 = currIntersaction.finish.first;
        z2 = currIntersaction.finish.second;
        zCurr = z1;
        dz = A/C;

        for (int j = x1; j < x2; j++)
        {
            if (i <= sizeY && i >= 0 && j <= sizeX && j >= 0)
            {
                if (zCurr > buffer[j][i].GetZ())
                {
                    buffer[j][i].SetZ(zCurr);
                    buffer[j][i].SetColor(currTriangle.GetColor());
                }
            }
            zCurr -= dz;
        }        
    }
}

void ZBuffer::Draw(QImage *img)
{
    for (int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            img->setPixel(i, j, buffer[i][j].GetColor().rgb());
        }
    }
}

void ZBuffer::Clear()
{
    for (int i = 0;i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            buffer[i][j].SetZ(-MAXDIST*2);
            QColor color = Qt::white;
            buffer[i][j].SetColor(color);
        }
    }
}

Pixel **ZBuffer::GetBuffer()
{
    return buffer;
}

ZBuffer::~ZBuffer ( )
{
    if (buffer)
        FreeBuffer();
}
