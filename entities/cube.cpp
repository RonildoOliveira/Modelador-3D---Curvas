#include "cube.h"

#include <iostream>

using namespace std;

Cube::Cube(){ this->nome = "Cube"; }
Cube::Cube(float tx, float ty, float tz, float ax, float ay, float az, float sx, float sy, float sz){
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;

    this->ax = ax;
    this->ay = ay;
    this->az = az;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->nome = "Cube";
}

float Cube::getAX() { return this->ax; }
float Cube::getAY() { return this->ay; }
float Cube::getAZ() { return this->az; }

void Cube::addAX(float ax) { this->ax += ax; }
void Cube::addAY(float ay) { this->ay += ay; }
void Cube::addAZ(float az) { this->az += az; }

void Cube::setAX(float ax) { this->ax = ax; }
void Cube::setAY(float ay) { this->ay = ay; }
void Cube::setAZ(float az) { this->az = az; }

float Cube::getSize() { return this->size; }
void Cube::setSize(float size) { this->size = size; }

void Cube::addSX(float sx) { this->sx+=sx; }
void Cube::addSY(float sy) { this->sy+=sy; }
void Cube::addSZ(float sz) { this->sz+=sz; }

void Cube::setSX(float sx) { this->sx = sx; }
void Cube::setSY(float sy) { this->sy = sy; }
void Cube::setSZ(float sz) { this->sz = sz; }

float Cube::getTX() { return this->tx; }
float Cube::getTY() { return this->ty; }
float Cube::getTZ() { return this->tz; }

void Cube::setTX(float tx) { this->tx = tx; }
void Cube::setTY(float ty) { this->ty = ty; }
void Cube::setTZ(float tz) { this->tz = tz; }

void Cube::addTX(float tx) { this->tx+=tx; }
void Cube::addTY(float ty) { this->ty+=ty; }
void Cube::addTZ(float tz) { this->tz+=tz; }

float Cube::getSX() { return this->sx; }
float Cube::getSY() { return this->sy; }
float Cube::getSZ() { return this->sz; }

bool Cube::isSelecionado(){
    return this->selecionado;
}

void Cube::setEixo(bool eixo) { this->eixo = eixo; }
bool Cube::isEixo() { return this->eixo; }

void Cube::setSombra(bool sombra) { this->sombra = sombra; }
bool Cube::isSombra() { return this->sombra; }

string Cube::getNome()
{
    return this->nome;
}

void Cube::setSelecionado(bool selecionado){ this->selecionado = selecionado; }

void Cube::addSlices() {}
void Cube::decSlices() {}
void Cube::addStacks() {}
void Cube::decStacks() {}

int Cube::getSlices() { return 0; }
int Cube::getStacks() { return 0; }

void Cube::desenha()
{
    //sistema local
    glPushMatrix();
        //composicao de transformacoes
        glTranslated(this->getTX(),this->getTY(),this->getTZ());

        glRotated(this->getAX(),0,0,1);
        glRotated(this->getAY(),0,1,0);
        glRotated(this->getAZ(),1,0,0);

        glScaled(this->getSX(),this->getSY(),this->getSZ());

        //desenhando eixos do sistema de coordenadas local 1
        if(isEixo()){
            Desenha::drawEixos( 0.5 );
        }

        //desenhando objeto
        if(isSelecionado()){
            glColor3f(0.22,1.0,0.07);
        }
        else if (isSombra()){
            glColor3f(0.0,0.0,0.0);
        }
        else{
            glColor3f(0.3,0.8,0.9);
        }

        glutSolidCube(this->getSize());

    glPopMatrix();
}
