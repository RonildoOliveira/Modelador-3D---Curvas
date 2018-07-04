#include "pontocontrole.h"

PontoControle::PontoControle(Vetor3D valoresTranslacao)
{
    this->tx = valoresTranslacao.x;
    this->ty = valoresTranslacao.y;
    this->tz = valoresTranslacao.z;
}

void PontoControle::desenha(){
    glPushMatrix();
    glTranslated(tx, ty, tz);
    if(!selecionado)glColor3f(1, 0, 0);
    else glColor3f(1, 1, 0);
    glutSolidSphere(.1, 36, 36);
    glPopMatrix();
}

std::string PontoControle::identificador(){
    return "pontoControle";
}

Vetor3D PontoControle::toVetor3D(){
    return Vetor3D(tx, ty, tz);
}

Vetor3D PontoControle::getValoresTranslacao()
{
    return Vetor3D(tx, ty, tz);
}

float PontoControle::getAX(){ return this->ax; }
float PontoControle::getAY(){ return this->ay; }
float PontoControle::getAZ(){ return this->az; }

void PontoControle::addAX(float ax){ this->ax = ax; }
void PontoControle::addAY(float ay){ this->ay = ay; }
void PontoControle::addAZ(float az){ this->az = az; }

void PontoControle::setAX(float ax) { this->ax = ax; }
void PontoControle::setAY(float ay) { this->ay = ay; }
void PontoControle::setAZ(float az) { this->az = az; }

/* Não altero a escala */
void PontoControle::addSY(float sy) { this->sy = 1.0; }
void PontoControle::addSX(float sx) { this->sx = 1.0; }
void PontoControle::addSZ(float sz) { this->sz = 1.0; }

void PontoControle::setSX(float sx) { this->sx = 1.0; }
void PontoControle::setSY(float sy) { this->sy = 1.0; }
void PontoControle::setSZ(float sz) { this->sz = 1.0; }

float PontoControle::getSX() { return this->sx; }
float PontoControle::getSY() { return this->sy; }
float PontoControle::getSZ() { return this->sz; }

void PontoControle::addTX(float tx) { this->tx+=tx; }
void PontoControle::addTY(float ty) { this->ty+=ty; }
void PontoControle::addTZ(float tz) { this->tz+=tz; }

void PontoControle::setTX(float tx) { this->tx = tx; }
void PontoControle::setTY(float ty) { this->ty = ty; }
void PontoControle::setTZ(float tz) { this->tz = tz; }

float PontoControle::getTX() { return this->tx; }
float PontoControle::getTY() { return this->ty; }
float PontoControle::getTZ() { return this->tz; }

void PontoControle::addSlices() { this->slices++; }
void PontoControle::decSlices() { this->slices--; }

void PontoControle::addStacks() { this->stacks++; }
void PontoControle::decStacks() { this->stacks--; }

int PontoControle::getSlices() { return this->slices; }
int PontoControle::getStacks() { return this->stacks; }

float PontoControle::getInnerRadius() { return this->innerRadius; }
void PontoControle::setInnerRadius(float innerRadius) { this->innerRadius = innerRadius; }

float PontoControle::getOutterRadius() { return this->outterRadius; }
void PontoControle::setOutterRadius(float outterRadius) { this->outterRadius = outterRadius; }

bool PontoControle::isSelecionado() { return this->selecionado; }

void PontoControle::setSombra(bool sombra) { this->sombra = sombra; }
bool PontoControle::isSombra() { return this->sombra; }

void PontoControle::setSelecionado(bool selecionado) { this->selecionado = selecionado; }

void PontoControle::setEixo(bool eixo) { this->eixo = eixo; }
bool PontoControle::isEixo() { return this->eixo; }

std::string PontoControle::getNome(){ return this->nome; }
