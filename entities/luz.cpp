#include "luz.h"

#include <iostream>

using namespace std;

Luz::Luz()
{
    this->indexLight ++;
    this->nome = "Luz";
}

Luz::Luz(
        float tx, float ty, float tz,
        float ax, float ay, float az,
        float sx, float sy, float sz)
{
    this->indexLight ++;

    this->tx = tx;
    this->ty = ty;
    this->tz = tz;

    this->ax = ax;
    this->ay = ay;
    this->az = az;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->nome = "Luz";
}

float Luz::getAX(){ return this->ax; }
float Luz::getAY(){ return this->ay; }
float Luz::getAZ(){ return this->az; }

void Luz::addAX(float ax){ this->ax += ax; }
void Luz::addAY(float ay){ this->ay += ay; }
void Luz::addAZ(float az){ this->az += az; }

void Luz::setAX(float ax) { this->ax = ax; }
void Luz::setAY(float ay) { this->ay = ay; }
void Luz::setAZ(float az) { this->az = az; }

/* Não altero a escala */
void Luz::addSY(float sy) { this->sy = 1.0; }
void Luz::addSX(float sx) { this->sx = 1.0; }
void Luz::addSZ(float sz) { this->sz = 1.0; }

void Luz::setSX(float sx) { this->sx = 1.0; }
void Luz::setSY(float sy) { this->sy = 1.0; }
void Luz::setSZ(float sz) { this->sz = 1.0; }

void Luz::addTX(float tx) { this->tx+=tx; }
void Luz::addTY(float ty) { this->ty+=ty; }
void Luz::addTZ(float tz) { this->tz+=tz; }

float Luz::getSX() { return this->sx; }
float Luz::getSY() { return this->sy; }
float Luz::getSZ() { return this->sz; }

void Luz::setTX(float tx) { this->tx = tx; }
void Luz::setTY(float ty) { this->ty = ty; }
void Luz::setTZ(float tz) { this->tz = tz; }

float Luz::getTX() { return this->tx; }
float Luz::getTY() { return this->ty; }
float Luz::getTZ() { return this->tz; }

void Luz::addSlices() { this->slices++; }
void Luz::decSlices() { this->slices--; }

void Luz::addStacks() { this->stacks++; }
void Luz::decStacks() { this->stacks--; }

int Luz::getSlices() { return this->slices; }
int Luz::getStacks() { return this->stacks; }

void Luz::desenha(){
    //habilitando transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //sistema local
    glPushMatrix();

        //composicao de transformacoes
        glTranslated(this->getTX(),this->getTY(),this->getTZ());

        glRotated(this->getAX(),0,0,1);
        glRotated(this->getAY(),0,1,0);
        glRotated(this->getAZ(),1,0,0);

        glScaled(this->getSX(),this->getSX(),this->getSZ());

        //desenhando objeto
        if(isSelecionado()){
            glColor4f(0.22,1.0,0.07,1);
        }
        else{
            glColor4f(1,1,1,0);
        }

        //glTranslatef(this->getTX(),this->getTY(),this->getTZ());
        gluSphere(gluNewQuadric(),this->innerRadius, this->slices, this->stacks);
        glutGUI::trans_luz = trans_luz;
        GUI::setLight(indexLight, 0, 0, 0, false, false);

        //GUI::setLight(0,posicao_luz[0],posicao_luz[1],posicao_luz[2],false,false);
        GLfloat posicao_luz[] = {this->getTX(),this->getTY(),this->getTZ(),1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    glPopMatrix();
}

float Luz::getInnerRadius() { return this->innerRadius; }
void Luz::setInnerRadius(float innerRadius) { this->innerRadius = innerRadius; }

float Luz::getOutterRadius() { return this->outterRadius; }
void Luz::setOutterRadius(float outterRadius) { this->outterRadius = outterRadius; }

bool Luz::isSelecionado() { return this->selecionado; }

void Luz::setSombra(bool sombra) { this->sombra = sombra; }
bool Luz::isSombra() { return this->sombra; }

void Luz::setSelecionado(bool selecionado) { this->selecionado = selecionado; }

void Luz::setEixo(bool eixo) { this->eixo = eixo; }
bool Luz::isEixo() { return this->eixo; }

string Luz::getNome(){ return this->nome; }
