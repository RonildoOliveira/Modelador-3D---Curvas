#include "torus.h"

#include <iostream>

using namespace std;

Torus::Torus() { this->nome = "Torus"; }

Torus::Torus(float innerRadius, float outterRadius, int slices, int stacks,
             float tx, float ty, float tz, float ax, float ay, float az, float sx, float sy, float sz)
{
    this->innerRadius = innerRadius;
    this->outterRadius = outterRadius;
    if(slices < 3){ this->slices = 3; }
    if(stacks < 3){ this->stacks = 3; }

    this->slices = slices;
    this->stacks = stacks;

    this->tx = tx;
    this->ty = ty;
    this->tz = tz;

    this->ax = ax;
    this->ay = ay;
    this->az = az;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->nome = "Torus";
}

Torus::Torus(
        float tx, float ty, float tz,
        float ax, float ay, float az,
        float sx, float sy, float sz)
{
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;

    this->ax = ax;
    this->ay = ay;
    this->az = az;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->nome = "Torus";
}

float Torus::getAX(){ return this->ax; }
float Torus::getAY(){ return this->ay; }
float Torus::getAZ(){ return this->az; }

void Torus::addAX(float ax){ this->ax += ax; }
void Torus::addAY(float ay){ this->ay += ay; }
void Torus::addAZ(float az){ this->az += az; }

void Torus::setAX(float ax) { this->ax = ax; }
void Torus::setAY(float ay) { this->ay = ay; }
void Torus::setAZ(float az) { this->az = az; }

void Torus::addSY(float sy) { this->sy+=sy; }
void Torus::addSX(float sx) { this->sx+=sx; }
void Torus::addSZ(float sz) { this->sz+=sz; }

void Torus::setSX(float sx) { this->sx = sx; }
void Torus::setSY(float sy) { this->sy = sy; }
void Torus::setSZ(float sz) { this->sz = sz; }

void Torus::addTX(float tx) { this->tx+=tx; }
void Torus::addTY(float ty) { this->ty+=ty; }
void Torus::addTZ(float tz) { this->tz+=tz; }

float Torus::getSX() { return this->sx; }
float Torus::getSY() { return this->sy; }
float Torus::getSZ() { return this->sz; }

void Torus::setTX(float tx) { this->tx = tx; }
void Torus::setTY(float ty) { this->ty = ty; }
void Torus::setTZ(float tz) { this->tz = tz; }

float Torus::getTX() { return this->tx; }
float Torus::getTY() { return this->ty; }
float Torus::getTZ() { return this->tz; }

void Torus::addSlices() { this->slices++; }
void Torus::decSlices() { this->slices--; }

void Torus::addStacks() { this->stacks++; }
void Torus::decStacks() { this->stacks--; }

int Torus::getSlices() { return this->slices; }
int Torus::getStacks() { return this->stacks; }

void Torus::desenha(){
    //sistema local
    glPushMatrix();
        //composicao de transformacoes
        glTranslated(this->getTX(),this->getTY(),this->getTZ());

        glRotated(this->getAX(),0,0,1);
        glRotated(this->getAY(),0,1,0);
        glRotated(this->getAZ(),1,0,0);

        glScaled(this->getSX(),this->getSX(),this->getSZ());

        //desenhando eixos do sistema de coordenadas local 1
        if(isEixo()){
            Desenha::drawEixos( 0.5 );
        }
         //glColor3d(0,1,0);
        //desenhando objeto
          if(isSelecionado()){
              glColor3f(0.22,1.0,0.07);
          }
          else if (isSombra()){
              glColor3f(0.0,0.0,0.0);
          }
          else{
              glColor3f(0.1,0.0,0.8);
          }

        glutSolidTorus(this->getInnerRadius(),this->getOutterRadius(),
                       this->getSlices(),this->getStacks());

    glPopMatrix();
}

float Torus::getInnerRadius()
{
    return this->innerRadius;
}

float Torus::getOutterRadius()
{
    return this->outterRadius;
}

void Torus::setInnerRadius(float innerRadius)
{
    this->innerRadius = innerRadius;
}

void Torus::setOutterRadius(float outterRadius)
{
    this->outterRadius = outterRadius;
}

bool Torus::isSelecionado() { return this->selecionado; }

void Torus::setEixo(bool eixo) { this->eixo = eixo; }
bool Torus::isEixo() { return this->eixo; }

void Torus::setSombra(bool sombra) { this->sombra = sombra; }
bool Torus::isSombra() { return this->sombra; }

string Torus::getNome() { return this->nome; }


void Torus::setSelecionado(bool selecionado){
    this->selecionado = selecionado;
}
