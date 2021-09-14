#include "pixel.h"

Pixel::Pixel()
{
   z = 0;
   color = Qt::white;
}

void Pixel::SetZ(double depth)
{
    z = depth;
}

void Pixel::SetColor(QColor &pixelColor)
{
    color = pixelColor;
}

double Pixel::GetZ()
{
    return z;
}

QColor Pixel::GetColor()
{
    return color;
}

