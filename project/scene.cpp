#include "scene.h"

#include <stdio.h>

Scene::Scene()
{
    movePart = false;
    movePart = false;
    moveScene = false;
    movePart = false;
}

Scene::~Scene()
{
    clear();
}

Scene::Scene(int widthCanvas, int heightCanvas, QCheckBox *chBody, QCheckBox *chBones)
{
    width = widthCanvas;
    height = heightCanvas;
    movePart = false;
    moveScene = false;
    chMesh = chBody;
    chSkelet = chBones;
}

int Scene::LoadModel(const char *filename)
{
    int ret = OK;
    ifstream fileIn(filename);
    if (fileIn.is_open())
    {
        lizard.Clear();
        lizard.Load(&fileIn);
        lizard.Resize(width/15, height/15, width/15);
        lizard.Move(width/2, height/2, 0);
        lizard.Initial();
        fileIn.close();
    }
    else
        ret = NO_FILE_ERR;
    return ret;
}

bool Scene::IsLoaded()
{
    if (lizard.IsLoaded())
        return true;
    else {
        return false;
    }
}

int Scene::DrawModel()
{
    bool withMesh = false;
    bool withSkelet = false;
    int ret = OK;

    clear();
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    render( &painter);
    painter.end();
    pixmap.save("image.png","PNG");
    QImage img = pixmap.toImage();
    img.fill(Qt::white);

    if (chMesh->isChecked())
        withMesh = true;
    if (chSkelet->isChecked())
        withSkelet = true;

    if (lizard.IsLoaded())
    {
        lizard.Draw(&img, withMesh, withSkelet);
    }
    else
    {
        ret = NO_MODEL;
    }
    addPixmap(QPixmap::fromImage(img));
    return ret;
}

int Scene::AddKeyFrame(int position, bool pushBack)
{
    int ret = OK;
    if (lizard.IsLoaded())
    {
        lizard.AddFrame(position, pushBack);
    }
    else
        ret = NO_MODEL;
    return ret;
}

int Scene::GetFramesCount(int type)
{
    return lizard.GetKeyFrameCount(type);
}

int Scene::GetAnimationStatus(long duration, int frameRate)
{
    if (lizard.IsLoaded())
        return lizard.GetAnimationStatus(duration, frameRate);
    else
        return NO_MODEL;
}

int Scene::ShowFrame(int index, int type)
{
    bool withMesh = false;
    bool withSkelet = false;
    int ret = OK;

    clear();
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    render( &painter);
    painter.end();
    pixmap.save("image.png","PNG");
    QImage img = pixmap.toImage();
    img.fill(Qt::white);

    if (chMesh->isChecked())
        withMesh = true;
    if (chSkelet->isChecked())
        withSkelet = true;

    if (lizard.IsLoaded())
    {
        lizard.Render(index, width/2, height/2, type);
        lizard.Draw(&img, withMesh, withSkelet);
    }
    else
        ret = NO_MODEL;
    addPixmap(QPixmap::fromImage(img));
    return ret;
}

int Scene::DeleteFrame(int index)
{
    int ret = OK;
    if (lizard.IsLoaded())
    {
        if (lizard.GetKeyFrameCount(KEY_FRAME))
            lizard.DeleteFrame(index);
        else
            ret = NO_FRAMES;
    }
    else
        ret = NO_MODEL;
    return ret;
}

int Scene::DeleteAllFrames()
{
    int ret = OK;
    if (lizard.IsLoaded())
    {
        if (lizard.GetKeyFrameCount(KEY_FRAME))
            lizard.DeleteAllFrames();
        else
            ret = NO_FRAMES;
    }
    else
        ret = NO_MODEL;
    return ret;
}

int Scene::RenderAnimation(long duration, int frameRate, QString name, bool status)
{
    int ret = OK;
    bool withMesh = false;
    bool withSkelet = false;

    if (chMesh->isChecked())
        withMesh = true;
    if (chSkelet->isChecked())
        withSkelet = true;

    if (lizard.IsLoaded() && lizard.GetKeyFrameCount(KEY_FRAME))
    {
        long msPerFrame = 1000/frameRate;
        if (!status)
            lizard.CompileAnimation(duration, frameRate);
        int index = 0;
        for (long currentTime = 0; currentTime < duration; currentTime += msPerFrame)
        {
            clear();
            QPixmap pixmap(width, height);
            QPainter painter(&pixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            render(&painter);
            painter.end();
            QString string = name;
            string.append(QString::number(index));
            string.append(".png");
            pixmap.save(string,"PNG");
            QImage img = pixmap.toImage();
            img.fill(Qt::white);

            lizard.CreateFrame(currentTime);
            lizard.Render(index, width/2, height/2, ANIMATION);
            lizard.Draw(&img, withMesh, withSkelet);
            pixmap = QPixmap::fromImage(img);
            pixmap.save(string, "PNG");
            index++;
        }
    }
    else if (!lizard.IsLoaded())
        ret = NO_MODEL;
    else
        ret = NO_FRAMES;
    return ret;
}

void Scene::ShowAnimation(int index, QString name)
{
    //if (lizard.IsLoaded())
    //{
    clear();
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    render( &painter);
    painter.end();
    QString string = name;
    string.append(QString::number(index));
    string.append(".png");
    pixmap.load(string);
    addPixmap(pixmap);
    //}
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (lizard.IsLoaded())
    {
        if (event->button() == Qt::LeftButton)
        {
            movePart = true;

            lizard.SetRotationJoint(event->scenePos().x(), event->scenePos().y());

            DrawModel();

            prevPoint = event->scenePos();

            moveScene = false;

            currentAngleX = 0;
            currentAngleY = 0;
            currentAngleZ = 0;
        }
        else if (event->button() == Qt::RightButton)
        {
            moveScene = true;

            prevPoint = event->scenePos();

            movePart = false;

            currentAngleX = 0;
            currentAngleY = 0;
            currentAngleZ = 0;

        }
    }
    event->accept();
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (lizard.IsLoaded())
    {
        movePart = false;
        moveScene = false;
        if (currentAngleX || currentAngleY || currentAngleZ)
            lizard.AddRotation(currentAngleX, currentAngleY, currentAngleZ);
    }
    event->accept();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (moveScene)
    {
        double x = event->scenePos().x(), y = event->scenePos().y();
        std::pair<double, double> aVector, bVector;
        double sp, cp, aLength, bLength, angle = 0;
        aVector.first = width/2 - x;
        aVector.second = height/2 - y;
        bVector.first = width/2 - prevPoint.x();
        bVector.second = height/2 - prevPoint.y();
        aLength = sqrt(aVector.first*aVector.first + aVector.second*aVector.second);
        bLength = sqrt(bVector.first*bVector.first + bVector.second*bVector.second);
        sp = aVector.first*bVector.first + aVector.second*bVector.second;
        cp = bVector.first*aVector.second - bVector.second*aVector.first;
        double cosinus = 1, sinus = 0;
        int sign = 1;
        if (aLength*bLength)
        {
            cosinus = sp/(aLength*bLength);
            sinus = cp/(aLength*bLength);
        }
        int cosInt = (int)(cosinus * 1000000);

        if (sinus > 0)
            sign = -1;

        cosinus = cosInt/1000000.0;

        if (cosinus < 1 && cosinus > -1)
            angle = (-1)*(sign)*acos(cosinus);
        else if (cosinus <= -1)
            angle = -M_PI;

        if (angle)
        {
            lizard.Move(-width/2, -height/2, 0);
            if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
            {
                if (currentAngleY || currentAngleZ)
                {
                    lizard.AddRotation(currentAngleX, currentAngleY, currentAngleZ);
                    currentAngleY = 0;
                    currentAngleZ = 0;
                }
                lizard.Rotate(angle,0,0);
                currentAngleX += angle;
            }
            else if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
            {
                if (currentAngleX || currentAngleZ)
                {
                    lizard.AddRotation(currentAngleX, currentAngleY, currentAngleZ);
                    currentAngleX = 0;
                    currentAngleZ = 0;
                }
                lizard.Rotate(0,angle,0);
                currentAngleY += angle;
            }
            else
            {
                if (currentAngleX || currentAngleY)
                {
                    lizard.AddRotation(currentAngleX, currentAngleY, currentAngleZ);
                    currentAngleX = 0;
                    currentAngleY = 0;
                }
                lizard.Rotate(0,0,angle);
                currentAngleZ += angle;
            }
            lizard.Move(width/2, height/2, 0);

            prevPoint = event->scenePos();

            DrawModel();
        }

    }
    else if (movePart)
    {
        double x = event->scenePos().x(), y = event->scenePos().y();
        int axis = NONE;
        if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
        {
            axis = X_AXIS;
        }
        else if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
        {
            axis = Y_AXIS;
        }
        else
            axis = Z_AXIS;
        lizard.RotateJoint(x, y, prevPoint.x(), prevPoint.y(), width, height, axis);
        prevPoint = event->scenePos();
        DrawModel();
    }
    event->accept();
}


