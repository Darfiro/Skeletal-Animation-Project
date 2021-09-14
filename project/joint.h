#ifndef JOINT_H
#define JOINT_H

#include "point3d.h"
#include "quaternion.h"
#include "stringcomparator.h"
#include "transformvalues.h"
#include <QImage>
#include <QColor>

class Joint
{
    private:

        Quaternion inverseMatrix;       // обратная матрица сустава
        Point3D jointCoordinates;       // координаты сустава
        Point3D initialPose;            // положение при нулевом вращении
        Point3D bindPose;               // исходное положение сустава
        std::vector<int> pointWeights;  // индексы точек, связанных с данным суставом
        std::vector<Joint*> children;   // потомки сустава
        Joint* parent;                  // родитель сустава
        char* name;                     // имя сустава
        int key;                        // ключ сустава
        QColor color;                   // цвет сустава: синий - не активен, красный - активен

        TransformValues transform;      // текущее преобразование сустава
        TransformValues rotation;       // суммарный поворот сустава

        // Установка координаты точки
        void SetDot(double** matrix);

        // Задание ключевых кадров: копирование данных и древовидной структуры
        void CopySkeletData(std::vector<Joint*>* vector);
        void CopyTreeStructure(std::vector<Joint *> *newRootJoint);

    public:
        Joint();

        // Отчистка дерева
        void Clear();

        // Инициализация
        void SetInitial();
        void SetBindPose();
        void SetColor(QColor setColor);
        void SetInverse(double** matrix);
        void SetNameKey(int number, char* jointName);
        void SetParent(Joint* parentJoint);
        void SetCoordinates(double x, double y, double z);
        void AddChild(Joint* child);
        void AddPointWeight(int link);
        void FindCoordinates();

        // Преобразовать дерево в вектор
        void CreateVector(std::vector<Joint*>* vector);

        // Доступ
        Point3D GetJointPosition();
        char* GetJointName();
        int GetJointKey();
        Joint* GetParent();
        std::vector<Joint*> GetChildren();
        Quaternion& GetInverse();
        TransformValues GetTransform();
        Point3D GetInitial();

        // Вернуть исходное положение и задать преобразование
        void TransformToBindPose();
        void SetRotation(double angleX, double angleY, double angleZ);

        // преобразования
        void ResizeJoint(double kx, double ky, double kz);
        void MoveJoint(double dx, double dy, double dz);
        void RotateJoint(double angleX, double angleY, double angleZ);

        // Связь сустав - точка
        void GetLinks(std::vector<int> *links);

        // Отчистить преобразования
        void ClearTransform();

        // Работа с углом поворота для данного сустава
        void AddRotation(double angleX, double angleY, double angleZ);
        TransformValues GetRotation();

        // Отрисовка
        void Draw(QImage *img);

        // Поиск нужного сустава для взаимодействия
        void FindJoint(int x, int y, Joint **rotJoint, double *minimum);

        // Копирование текущего положения
        Joint* CopySkelet();

        // Рендеринг нового положения: задание вращения для суставов
        void NewPositionRotation(Joint* newRoot);
};


#endif // JOINT_H
