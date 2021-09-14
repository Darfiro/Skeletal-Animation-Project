#ifndef LOADER_H
#define LOADER_H

#include <fstream>
#include <iostream>
#include "point3d.h"
#include "triangle.h"
#include "joint.h"
#include "transform.h"

#define OK 0
#define NO_FILE_ERR -1
#define INPUT_ERR -2

class Loader
{
    private:

        int FindCount(std::ifstream *fileIn);
        int GetName(std::ifstream *fileIn, std::vector<Joint*> joints);
        void SortTriangles(std::vector<Triangle> *triangleList);

    public:
        Loader();

        void LoadMesh(std::vector<Point3D>* pointList, std::ifstream *fileIn);
        void LoadTriangles(std::vector<Triangle>* triangleList,std::ifstream *fileIn, std::vector<Point3D>* pointList);
        void LoadJointsNames(std::vector<Joint*>* joints, std::ifstream *fileIn);
        void LoadJointInverse(std::vector<Joint*>* joints,std::ifstream *fileIn);
        void LoadJointsWeight(std::vector<double>* weights, std::ifstream *fileIn);
        void LoadWeightByJoint(std::vector<Joint*>* joints, std::vector<Transform> *transformVector, std::vector<double> weights, std::vector<Point3D>* pointList,std::vector<int> numJointWeight, std::ifstream *fileIn);
        void LoadJoints(Joint** rootJoint, std::ifstream *fileIn, std::vector<Joint *> joints);
};

#endif // LOADER_H
