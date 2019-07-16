#include"bib/Desenha.h"

#include "spline.h"

Spline::Spline(){
    this->aviao = new ObjModelLoader("../modelador-3d-curvas/data/obj/Aviao.obj", "Aviao");
    aviao->setSombra(true);
    p = new Vetor3D[4];
    this->setMode(SplineMode::BSPLINE);

    this->cam = new CameraDistante();
}

CameraDistante* Spline::getCameraDistante()
{
    return this->cam;
}

ObjModelLoader *Spline::getAviao()
{
    return this->aviao;
}

void Spline::setPontosControle(vector<Vetor3D> pontosControle){
    pn.clear();
    for(Vetor3D p : pontosControle){
        this->pn.push_back(p);
    }

    for(int i = 0; i<pn.size() && pn.size() <= 4; i++){
        p[i] = pn[i];
    }

}

void Spline::removePontosControle(){
    this->pn.clear();
    delete p;
    p = new Vetor3D[4];
}

void Spline::addPontoControle(Vetor3D ponto){
    this->pn.push_back(ponto);
}

void Spline::setMode(SplineMode mode){
    this->mode = mode;

    if(mode == SplineMode::INTERPOLADORA){
        float Maux[4][4] = {
            {-4.5, 13.5, -13.5, 4.5},
            {9, -22.5, 18, -4.5},
            {-5.5, 9, -4.5, 1},
            {1, 0, 0, 0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }
    /**
        Interpolação tn
        n = pontos;
        pk, pk+1, Dpk, Dpk+1
    **/
    if(mode == SplineMode::HERMITE){
        float Maux[][4] = {
            {2, -2, 1, 1},
            {-3, 3, -2, -1},
            {0, 0, 1, 0},
            {1, 0, 0, 0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }
    /**
        Aproximação
        c(u) = (1-p)p0-p1
        c(u) = Sum i=0 to n Bi n (u)pi
        Ṕascal
    **/
    else if(mode == SplineMode::BEZIER){
        float Maux[][4] = {
            {-1, 3, -3, 1},
            {3, -6, 3, 0},
            {-3, 3, 0, 0},
            {1, 0, 0, 0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }

    }
    /**
        Aproximação
        c(u) = Sum i=0 to n-m Bi n (u)pi
    **/
    else if(mode == SplineMode::BSPLINE){
        float Maux[][4] = {
            {-1/6.0, 3/6.0, -3/6.0, 1/6.0},
            {3/6.0, -6/6.0, 3/6.0, 0/6.0},
            {-3/6.0, 0/6.0, 3/6.0, 0/6.0},
            {1/6.0, 4/6.0, 1/6.0, 0/6.0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }
    /**
        Interpolação
        c(u) = Sum i=0 to 3 ci u^i
    **/
    else if(mode == SplineMode::CATMULLROM){
        float Maux[][4] = {
            {-1/2.0, 3/2.0, -3/2.0, 1/2.0},
            {2/2.0, -5/2.0, 4/2.0, -1/2.0},
            {-1/2.0, 0.0, 1/2.0, 0.0},
            {0.0, 2/2.0, 0.0, 0.0}
        };

        for(int i = 0; i<4; i++){
            for(int j = 0; j<4; j++){
                M[i][j] = Maux[i][j];
            }
        }
    }
}

SplineMode Spline::getMode(){
    return this->mode;
}

//Transposto
Vetor3D Spline::getPoint(float u){

    Vetor3D resultado;
    float us[] = {pow(u, 3), pow(u, 2), u, 1};

    float matriz[4];

    for(int i = 0; i<4; i++){
        matriz[i] = 0;
        for(int j = 0; j<4; j++){
            matriz[i] += us[j] * M[j][i];
        }
    }

    for(int i = 0; i<4; i++){
        resultado.x += matriz[i] * p[i].x;
        resultado.y += matriz[i] * p[i].y;
        resultado.z += matriz[i] * p[i].z;
    }

    return resultado;
}

Vetor3D Spline::getD1(float u){

    Vetor3D resultado;
    float us[] = {3*pow(u, 2), 2*u, 1, 0};

    float matriz[4];

    for(int i = 0; i<4; i++){
        matriz[i] = 0;
        for(int j = 0; j<4; j++){
            matriz[i] += us[j] * M[j][i];
        }
    }

    for(int i = 0; i<4; i++){
        resultado.x += matriz[i] * p[i].x;
        resultado.y += matriz[i] * p[i].y;
        resultado.z += matriz[i] * p[i].z;
    }

    return resultado;

}

Vetor3D Spline::getD2(float u){

    Vetor3D resultado;
    float us[] = {6*u, 2, 0, 0};

    float matriz[4];

    for(int i = 0; i<4; i++){
        matriz[i] = 0;
        for(int j = 0; j<4; j++){
            matriz[i] += us[j] * M[j][i];
        }
    }

    for(int i = 0; i<4; i++){
        resultado.x += matriz[i] * p[i].x;
        resultado.y += matriz[i] * p[i].y;
        resultado.z += matriz[i] * p[i].z;
    }

    return resultado;

}

vector<Vetor3D> Spline::getPontosControle(){
    return pn;
}

int Spline::getQtdPontosControle(){
    return pn.size();
}

void Spline::desenharCurva(){

    Vetor3D ponto;
    for(int i = 0; i<=pn.size()-4; i++){

        for(int j = 0; j<4; j++){
            ponto = pn[i+j];
            p[j] = ponto;
        }

        /**
         |    |    |    |    |
         | i' | j' | k' | o' |
         |    |    |    |    |
         [ 0    0    0     1 ]
        **/
        for(float u = 0; u<=1; u+=delta){
            Vetor3D z_ = getD1(u) * (-1); //k'
            Vetor3D y_ = getD2(u); //up
            Vetor3D x_ = y_ ^ z_; //i' = up * k'
            y_ = z_ ^ x_; // k' * i' = j'
            Vetor3D t_ = getPoint(u); //o -> translacao
            z_.normaliza();
            y_.normaliza();
            x_.normaliza();

            double T[] = {
                x_.x, y_.x, z_.x, t_.x,
                x_.y, y_.y, z_.y, t_.y,
                x_.z, y_.z, z_.z, t_.z,
                0   , 0   , 0   , 1
            };

            glPushMatrix();
                glColor3f(0, 0, 0);
                glMultTransposeMatrixd(T);
                desenhaCaminho();
            glPopMatrix();

        }
    }

}

void Spline::desenhaCaminho(){
    glPushMatrix();
        glTranslated(0.25, 0, 0);
        glRotated(90, 0, 1, 0);
        glRotated(180, 1, 0, 0);
        glPushMatrix();
            glColor3f(1,0,1);
            glutSolidCone(0.05,0.05,10,10);
        glPopMatrix();
    glPopMatrix();
}

void Spline::desenharComSpline(bool animar){

    Vetor3D ponto;
    static int i = 0;
    for(int j = 0; j<4; j++){
        ponto = pn[i+j];
        p[j] = ponto;
    }

    Vetor3D z_ = getD1(u) * (-1);
    Vetor3D y_ = getD2(u);
    Vetor3D x_ = y_ ^ z_;
    y_ = z_ ^ x_;
    Vetor3D t_ = getPoint(u);
    z_.normaliza();
    y_.normaliza();
    x_.normaliza();

    double T[] = {
        x_.x, y_.x, z_.x, t_.x,
        x_.y, y_.y, z_.y, t_.y,
        x_.z, y_.z, z_.z, t_.z,
        0   , 0   , 0   , 1
    };

    glPushMatrix();
        glColor3f(1, 0, 1);
        glMultTransposeMatrixd(T);
        glScaled(1,1,1);
        glRotated(180,0,0,1);
        aviao->desenha();
    glPopMatrix();

    if(u >= 1){
        u = 0;
        i++;
        if(i > pn.size()-4)
            i = 0;
    }

    cam->e = getPoint(u-20*delta) ;
    cam->c = getPoint(u) + getD1(u);
    cam->u = (getD2(u-20*delta) * (-1)).getUnit();
    cam->e = cam->u + cam->e;

    if(animar){
        u+=delta;
    }
}
