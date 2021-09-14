#ifndef ANIMATION_H
#define ANIMATION_H

#include "spline.h"
#include "joint.h"

class Animation
{
    private:
        long animationDuration;                 // продолжительность анимации
        int frameRate;                          // количество кадров в секунду

        std::vector<Joint*> initialKeyFrames;   // ключевые кадры
        std::vector<Joint*> animationFrames;    // кадры анимации
        std::vector<Spline> splinesForEach;     // сплайны для каждого сустава
        Joint* currentPose;                     // текущее положение

        void RotateCurrent();

    public:

        Animation();
        Animation(std::vector<Joint*> initialFrames, long duration, int rate);
        void SetInitialKeyFrames(std::vector<Joint*> initialFrames);
        void SetDuration(long duration);
        void SetFrameRate(int rate);

        long GetDuration();
        int GetFrameRate();

        void Clear();

        void FillSpline();

        void CreateAnimation();

        Joint* CreateFrame(long moment);

        std::vector<Joint*> GetAnimation();

        ~Animation();
};

#endif // ANIMATION_H
