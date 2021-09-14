#ifndef __TRANSFORMVALUES_H__
#define __TRANSFORMVALUES_H__

#define NO_TRANSFORM 4
#define ROTATE 1
#define MOVE 2
#define SCALE 3

// Структура преобразования
struct TransformValues
{
    double paramOne;    // первый параметр: для поворота - угол по оси X, для переноса - dx
    double paramTwo;    // первый параметр: для поворота - угол по оси Y, для переноса - dy
    double paramThree;  // первый параметр: для поворота - угол по оси Z, для переноса - dz
    int type;           // типа преобразования: поворот или перенос
};

#endif
