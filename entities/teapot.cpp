#include "teapot.h"
#include <iostream>

using namespace std;

Teapot::Teapot(){ this->nome = "Teapot"; }
Teapot::Teapot(float tx, float ty, float tz, float ax, float ay, float az, float sx, float sy, float sz){
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;

    this->ax = ax;
    this->ay = ay;
    this->az = az;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->nome = "Teapot";
}

float Teapot::getAX() { return this->ax; }
float Teapot::getAY() { return this->ay; }
float Teapot::getAZ() { return this->az; }

void Teapot::addAX(float ax) { this->ax += ax; }
void Teapot::addAY(float ay) { this->ay += ay; }
void Teapot::addAZ(float az) { this->az += az; }

void Teapot::setAX(float ax) { this->ax = ax; }
void Teapot::setAY(float ay) { this->ay = ay; }
void Teapot::setAZ(float az) { this->az = az; }

float Teapot::getSize() { return this->size; }
void Teapot::setSize(float size) { this->size = size; }

void Teapot::addSX(float sx) { this->sx+=sx; }
void Teapot::addSY(float sy) { this->sy+=sy; }
void Teapot::addSZ(float sz) { this->sz+=sz; }

void Teapot::setSX(float sx) { this->sx = sx; }
void Teapot::setSY(float sy) { this->sy = sy; }
void Teapot::setSZ(float sz) { this->sz = sz; }

float Teapot::getTX() { return this->tx; }
float Teapot::getTY() { return this->ty; }
float Teapot::getTZ() { return this->tz; }

void Teapot::setTX(float tx) { this->tx = tx; }
void Teapot::setTY(float ty) { this->ty = ty; }
void Teapot::setTZ(float tz) { this->tz = tz; }

void Teapot::addTX(float tx) { this->tx+=tx; }
void Teapot::addTY(float ty) { this->ty+=ty; }
void Teapot::addTZ(float tz) { this->tz+=tz; }

float Teapot::getSX() { return this->sx; }
float Teapot::getSY() { return this->sy; }
float Teapot::getSZ() { return this->sz; }

bool Teapot::isSelecionado(){
    return this->selecionado;
}

void Teapot::setEixo(bool eixo) { this->eixo = eixo; }
bool Teapot::isEixo() { return this->eixo; }

void Teapot::setSombra(bool sombra) { this->sombra = sombra; }
bool Teapot::isSombra() { return this->sombra; }

string Teapot::getNome()
{
    return this->nome;
}

void Teapot::setSelecionado(bool selecionado){ this->selecionado = selecionado; }

void Teapot::addSlices() {}
void Teapot::decSlices() {}
void Teapot::addStacks() {}
void Teapot::decStacks() {}

int Teapot::getSlices() { return 0; }
int Teapot::getStacks() { return 0; }

void Teapot::desenha()
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
            glColor3f(0.8,0.5,0.5);
        }

        glutSolidTeapot(this->getSize());
    glPopMatrix();
}
