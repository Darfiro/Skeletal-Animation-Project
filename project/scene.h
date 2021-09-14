#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QApplication>
#include <QFlags>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QCheckBox>
#include <QLabel>
#include <math.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include "model.h"
using namespace std;


#define NONE -6
#define NO_MODEL -3
#define MAX_ANGLE -5

class Scene: public QGraphicsScene
{
    //Q_OBJECT

    private:
        bool movePart;
        bool moveScene;
        int xPos, yPos, zPos;
        int numLine, numPoint;
        int axis;
        int width, height;
        QPointF prevPoint, rotationPoint;
        Model lizard;
        QCheckBox *chMesh, *chSkelet;
        double currentAngleX, currentAngleY, currentAngleZ;

        void FindClosest(Point3D point);

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    public:
        Scene();
        ~Scene();

        Scene(int widthCanvas, int heightCanvas, QCheckBox *chBody, QCheckBox *chBones);

        int LoadModel(const char* filename);
        bool IsLoaded();
        int DrawModel();
        int AddKeyFrame(int position, bool pushBack);
        int GetFramesCount(int type);
        int GetAnimationStatus(long duration, int frameRate);
        int ShowFrame(int index, int type);
        int DeleteFrame(int index);
        int DeleteAllFrames();
        int RenderAnimation(long duration, int frameRate, QString name, bool status);
        void ShowAnimation(int index, QString name);
};

#endif // SCENE_H
