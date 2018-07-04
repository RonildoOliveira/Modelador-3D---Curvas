#include "tree.h"

using namespace std;

Tree::Tree() { this->nome = "Arvore"; }
int numberCups = 2;
Tree::Tree(
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

    this->nome = "Arvore";
}

float Tree::getAX(){ return this->ax; }
float Tree::getAY(){ return this->ay; }
float Tree::getAZ(){ return this->az; }

void Tree::addAX(float ax){ this->ax += ax; }
void Tree::addAY(float ay){ this->ay += ay; }
void Tree::addAZ(float az){ this->az += az; }

void Tree::setAX(float ax) { this->ax = ax; }
void Tree::setAY(float ay) { this->ay = ay; }
void Tree::setAZ(float az) { this->az = az; }

void Tree::addSY(float sy) { this->sy+=sy; }
void Tree::addSX(float sx) { this->sx+=sx; }
void Tree::addSZ(float sz) { this->sz+=sz; }

void Tree::setSX(float sx) { this->sx = sx; }
void Tree::setSY(float sy) { this->sy = sy; }
void Tree::setSZ(float sz) { this->sz = sz; }

void Tree::addTX(float tx) { this->tx+=tx; }
void Tree::addTY(float ty) { this->ty+=ty; }
void Tree::addTZ(float tz) { this->tz+=tz; }

float Tree::getSX() { return this->sx; }
float Tree::getSY() { return this->sy; }
float Tree::getSZ() { return this->sz; }

void Tree::setTX(float tx) { this->tx = tx; }
void Tree::setTY(float ty) { this->ty = ty; }
void Tree::setTZ(float tz) { this->tz = tz; }

float Tree::getTX() { return this->tx; }
float Tree::getTY() { return this->ty; }
float Tree::getTZ() { return this->tz; }

void Tree::addSlices() { this->slices++; }
void Tree::decSlices() { this->slices--; }

void Tree::addStacks() { this->stacks++; }
void Tree::decStacks() { this->stacks--; }

int Tree::getSlices() { return this->slices; }
int Tree::getStacks() { return this->stacks; }

void Tree::desenha(){
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
        glColor3f(0.5,0.5,0.5);
    }

    glRotated(-90,1,0,0);

    GLdouble i = 1.0; //>= 1.0 <= 2.0
    GLdouble green_base = 0.5; //Representa a relacao da largura da base e a cor

    GLfloat mult = 0.5;

    //Tronco
    glColor3d(0.6, 0.1, 0.1);
    gluCylinder(gluNewQuadric(),
                mult*(GLdouble)numberCups/10,
                mult*(GLdouble)numberCups/10,
                (1.0), slices, stacks);



    GLfloat angle = 0.0;
    //Copas
    while (i <= numberCups) {

        glColor3d(0,green_base/i,0);

        glTranslated(0,0,1.0); //Poe uma copa sobre a outra
        gluCylinder(gluNewQuadric(),
                    mult*((numberCups+green_base)-(i)),
                    mult*(numberCups-i),
                    (i), slices, stacks);
        i+=0.5;
    }

    glPopMatrix();
}

bool Tree::isSelecionado(){
    return this->selecionado;
}

void Tree::setEixo(bool eixo)
{
    this->eixo = eixo;
}

bool Tree::isEixo()
{
    return this->eixo;
}

void Tree::setSombra(bool sombra) { this->sombra = sombra; }
bool Tree::isSombra() { return this->sombra; }

string Tree::getNome()
{
    return this->nome;
}


void Tree::setSelecionado(bool selecionado){
    this->selecionado = selecionado;
}

