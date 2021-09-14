#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new Scene(ui->graphicsView->width(), ui->graphicsView->height(), ui->chBody, ui->chSkelet);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0, 0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);

    scene->DrawModel();

    QMessageBox* pmbx = new QMessageBox();
    pmbx->setText("Для поворота модели используйте правую кнопку мыши\nДля поворота части модели используйте левую кнопку мыши\nДля поворота вокруг оси X зажмите Shift\nДля поворота вокруг оси Y зажмите Ctrl\nДля поворота вокруг оси Z никаких дополнительных клавиш зажимать не нужно");
    int n = pmbx->exec();
    delete pmbx;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnInput_clicked()
{
    int ret = OK;

    const char *fileName = ui->edtInput->text().toStdString().c_str();
    ret = scene->LoadModel(fileName);

    if (!ret)
    {
        ui->cmbFrames->clear();
        ret = scene->DrawModel();
        if (ret == NO_MODEL)
        {
            QString error = "Модель не найдена. Невозможно нарисовать";
            show_Message(error);
        }
    }
    else if (ret == NO_FILE_ERR)
    {
        QString error = "Файл не найден";
        show_Message(error);
    }

}

void MainWindow::on_chSkelet_stateChanged(int arg1)
{
    if (!arg1 && !(ui->chBody->isChecked()))
    {
        ui->chSkelet->setCheckState(Qt::Checked);
        QString error = "На сцене должно быть что-то изображено";
        show_Message(error);
        showChanged = false;
    }
    else if (ui->chBody->isChecked())
    {
        showChanged = true;
        int ret = OK;
        ret = scene->DrawModel();
        if (ret == NO_MODEL)
        {
            QString error = "Модель не найдена. Невозможно нарисовать";
            show_Message(error);
        }
    }
}

void MainWindow::on_chBody_stateChanged(int arg1)
{
    if (!arg1 && !(ui->chSkelet->isChecked()))
    {
        ui->chBody->setCheckState(Qt::Checked);        
        QString error = "На сцене должно быть что-то изображено";
        show_Message(error);
        showChanged = false;
    }
    else if (ui->chSkelet->isChecked())
    {
        showChanged = true;
        int ret = OK;
        ret = scene->DrawModel();
        if (ret == NO_MODEL)
        {
            QString error = "Модель не найдена. Невозможно нарисовать";
            show_Message(error);
        }
    }
}

void MainWindow::on_btnAdd_clicked()
{
    int position = ui->cmbFrames->currentIndex();
    int size = ui->cmbFrames->count();
    bool pushBack = true;
    if (ui->btnCurrent->isChecked())
        pushBack = false;
    int ret = scene->AddKeyFrame(position, pushBack);

    if (ret == NO_MODEL)
    {
        QString error = "Перед добавлением нужно загрузить модель";
        show_Message(error);
    }
    else
    {
        justAdded = true;
        ui->cmbFrames->addItem(QString::number(size + 1), Qt::DisplayRole);
        ui->cmbFrames->setCurrentIndex(ui->cmbFrames->count() - 1);
    }
}

void MainWindow::on_btnView_clicked()
{
    int count = scene->GetFramesCount(KEY_FRAME);
    for (int i = 0; i < count; i++)
    {
        scene->ShowFrame(i, KEY_FRAME);
        QTime end = QTime::currentTime().addMSecs(100);
        while(QTime::currentTime() < end)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::on_cmbFrames_currentIndexChanged(int index)
{
    if (index > -1 && !justAdded)
        scene->ShowFrame(index, KEY_FRAME);
    justAdded = false;
}

void MainWindow::on_btnDeleteCurr_clicked()
{
    int index = ui->cmbFrames->currentIndex();
    int ret = scene->DeleteFrame(index);
    if (ret == NO_MODEL)
    {
        QString error = "Перед удалением нужно загрузить модель";
        show_Message(error);
    }
    else if (ret == NO_FRAMES)
    {
        QString error = "Перед удалением нужно задать кадры";
        show_Message(error);
    }
    else
    {
        ui->cmbFrames->removeItem(index);
    }
}

void MainWindow::on_btnDeleteAll_clicked()
{
    int ret = scene->DeleteAllFrames();
    if (ret == NO_MODEL)
    {
        QString error = "Перед удалением нужно загрузить модель";
        show_Message(error);
    }
    else if (ret == NO_FRAMES)
    {
        QString error = "Перед удалением нужно задать кадры";
        show_Message(error);
    }
    else
    {
        ui->cmbFrames->clear();
    }
}

void MainWindow::deleteFiles(QString name, const char *filename)
{
    FILE* fileIn = fopen(filename, "r");
    if (fileIn)
    {
        long duration;
        int frameRate;
        fscanf(fileIn, "%lu %d", &duration, &frameRate);
        fclose(fileIn);
        int count = frameRate*duration/1000;
        for (int i = 0; i < count; i++)
        {
            QString string = name;
            string.append(QString::number(i));
            string.append(".png");
            QFile file (string);
            file.remove();
        }
    }
}



void MainWindow::on_btnPlay_clicked()
{
    long duration = ui->Duration->value()*1000;
    int frameRate = ui->Speed->value();
    bool animationPossible = true;
    if (scene->GetFramesCount(KEY_FRAME) < 2)
    {
        QString error = "Для анимации необходимо минимум 2 кадра";
        show_Message(error);
        animationPossible = false;
    }
    if (animationPossible)
    {
        QString animation = "image";
        if (!QDir(animation).exists())
            QDir().mkdir(animation);
        QString filepath = QDir::currentPath();
        filepath.append("/");
        filepath.append(animation);
        filepath.append("/");
        filepath.append(animation);
        animation = filepath;
        int ret = OK;
        if (scene->GetAnimationStatus(duration, frameRate) == NEW_ANIMATION)
        {
            deleteFiles(animation, "image.txt");
            QString error = "Ваша анимация загружается, это может занять некоторое время";
            show_Message(error);
            ret = scene->RenderAnimation(duration, frameRate, animation, false);
            FILE* fileOut = fopen("image.txt", "w");
            fprintf(fileOut, "%lu %d", duration, frameRate);
            fclose(fileOut);
        }
        else if (showChanged)
        {
            deleteFiles(animation, "image.txt");
            QString error = "Ваша анимация загружается, это может занять некоторое время";
            show_Message(error);
            ret = scene->RenderAnimation(duration, frameRate, animation, true);
            FILE* fileOut = fopen("image.txt", "w");
            fprintf(fileOut, "%lu %d", duration, frameRate);
            fclose(fileOut);
            showChanged = false;
        }

        if (ret == NO_MODEL)
        {
            QString error = "Перед запуском нужно загрузить модель";
            show_Message(error);
        }
        else if (ret == NO_FRAMES)
        {
            QString error = "Перед запуском нужно задать кадры";
            show_Message(error);
        }
        else
        {
            int count = frameRate*duration/1000;
            long wait = duration/count;
            isAnimated = true;
            while(isAnimated)
            {
                for (int i = 0; i < count && isAnimated; i++)
                {
                    scene->ShowAnimation(i, animation);
                    QTime end = QTime::currentTime().addMSecs(wait);
                    while(QTime::currentTime() < end)
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
                }
            }
            scene->DrawModel();
        }
    }
}

void MainWindow::on_Duration_valueChanged(int value)
{
    QString string = QString::number(value);
    string.append(" сек");
    ui->lblDuration->setText(string);
}

void MainWindow::on_btnStop_clicked()
{
    if (isAnimated)
        isAnimated = false;
}

void MainWindow::on_Speed_valueChanged(int value)
{
    QString string = QString::number(value);
    string.append(" кадр/сек");
    ui->lblFrameRate->setText(string);
}

void MainWindow::show_Message(QString message)
{
    QMessageBox* pmbx = new QMessageBox();
    pmbx->setText(message);
    int n = pmbx->exec();
    delete pmbx;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox* pmbx = new QMessageBox();
    pmbx->setText("Для поворота модели используйте правую кнопку мыши\nДля поворота части модели используйте левую кнопку мыши\nДля поворота вокруг оси X зажмите Shift\nДля поворота вокруг оси Y зажмите Ctrl\nДля поворота вокруг оси Z никаких дополнительных клавиш зажимать не нужно");
    int n = pmbx->exec();
    delete pmbx;
}

void MainWindow::saveResult(QString name, const char *filename)
{
    int ret = OK;
    FILE* fileIn = fopen(filename, "r");
    if (fileIn)
    {
        long duration;
        int frameRate;
        fscanf(fileIn, "%lu %d", &duration, &frameRate);
        fclose(fileIn);
        int count = frameRate*duration/1000;
        for (int i = 0; i < count; i++)
        {
            QString animation = "image";
            QString filepath = QDir::currentPath();
            filepath.append("/");
            filepath.append(animation);
            filepath.append("/");
            filepath.append(animation);
            animation = filepath;
            QString newName = name;
            newName.append(QString::number(i));
            newName.append(".png");
            animation.append(QString::number(i));
            animation.append(".png");
            QFile::copy(animation, newName);
        }
        QFile file("image.txt");
        name.append(".txt");
        file.rename(name);
    }    
}

void MainWindow::on_btnSafe_clicked()
{
    long duration = ui->Duration->value()*1000;
    int frameRate = ui->Speed->value();
    QString animation = ui->edtAnimName->text();
    if (animation.length())
    {
        if (!QDir(animation).exists())
            QDir().mkdir(animation);
        QString filepath = QDir::currentPath();
        filepath.append("/");
        filepath.append(animation);
        filepath.append("/");
        filepath.append(animation);
        animation = filepath;
        const char *filename = "image.txt";
        if (scene->GetFramesCount(KEY_FRAME) < 2)
        {
            QString error = "Для анимации необходимо минимум 2 кадра";
            show_Message(error);
        }
        else
        {
            int ret = OK;
            QString error = "Анимация будет сохранена в виде последовательности кадров";
            show_Message(error);
            if (scene->GetAnimationStatus(duration, frameRate) == OLD_ANIMATION)
            {
                deleteFiles(animation, filename);
                saveResult(animation, filename);
            }
            else {
                deleteFiles(animation, filename);
                ret = scene->RenderAnimation(duration, frameRate, animation, false);
                QString fileN = animation;
                fileN.append(".txt");
                FILE* fileOut = fopen(fileN.toStdString().c_str(), "w");
                fprintf(fileOut, "%lu %d", duration, frameRate);
                fclose(fileOut);
                scene->DrawModel();
            }
            if (ret == NO_MODEL)
            {
                QString error = "Перед сохранением нужно загрузить модель";
                show_Message(error);
            }
            else
            {
                QString error = "Анимация успешно сохранена";
                show_Message(error);
                scene->DrawModel();
            }
        }
    }
    else {
        QString error = "Задайте имя файла";
        show_Message(error);
    }
}

void MainWindow::shut_Down(QCloseEvent *event)
{
    if (isAnimated)
        isAnimated = false;
    QString string = "image";
    deleteFiles(string, "image.txt");
    QString error = "Завершение работы программы";
    show_Message(error);
    event->accept();
}

void MainWindow::on_btnShowExample_clicked()
{
    QString animation = ui->edtAnimName->text();
    QString filepath = QDir::currentPath();
    filepath.append("/");
    filepath.append(animation);
    filepath.append("/");
    filepath.append(animation);
    animation = filepath;
    QString fileN = animation;
    fileN.append(".txt");
    FILE* file = fopen(fileN.toStdString().c_str(), "r");
    if (file)
    {        
        int frameRate;
        long duration;
        fscanf(file, "%lu %d", &duration, &frameRate);
        fclose(file);
        int count = frameRate*duration/1000;
        long wait = duration/count;
        isAnimated = true;
        while(isAnimated)
        {
            for (int i = 0; i < count && isAnimated; i++)
            {
                scene->ShowAnimation(i, animation);
                QTime end = QTime::currentTime().addMSecs(wait);
                while(QTime::currentTime() < end)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
        }
        if (scene->IsLoaded())
        {
            scene->DrawModel();
        }
        else {
            QString error = "Показ завершен. Загрузите модель для работы с ней";
            show_Message(error);
            scene->clear();
        }
    }
    else {
        QString error = "Не найдено сохраненной анимации с этим именем";
        show_Message(error);
    }
}
