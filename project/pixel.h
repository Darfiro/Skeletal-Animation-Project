#ifndef PIXEL_H
#define PIXEL_H

#include <QColor>

class Pixel
{
    private:
        double z;
        QColor color;
    public:
        Pixel();

        void SetZ(double depth);
        void SetColor(QColor &pixelColor);

        double GetZ();
        QColor GetColor();
};

#endif // PIXEL_H
