#ifndef SPLINE_H
#define SPLINE_H

#include "bib/Vetor3D.h"
#include "pontocontrole.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include "entities/models/objmodelloader.h"

using namespace std;

enum SplineMode{
    INTERPOLADORA, BEZIER, HERMITE, BSPLINE, CATMULLROM
};

class Spline
{

private:
    ObjModelLoader *aviao;
    float M[4][4];
    Vetor3D* p;
    vector<Vetor3D> pn;
    SplineMode mode;

    float u = 0.0;
    float delta = 0.01;

    CameraDistante * cam;
public:
    Spline();

    CameraDistante *getCameraDistante();

    ObjModelLoader *getAviao();

    void setPontosControle(vector<Vetor3D> pontosControle);

    void removePontosControle();

    void addPontoControle(Vetor3D ponto);

    void setMode(SplineMode);

    SplineMode getMode();

    Vetor3D getPoint(float u);

    Vetor3D getD1(float u);

    Vetor3D getD2(float u);

    vector<Vetor3D> getPontosControle();

    int getQtdPontosControle();

    void desenharCurva();

    void desenhaCaminho();
    void desenharComSpline(bool animar);
};

#endif // SPLINE_H
