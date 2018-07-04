#ifndef MODEL_H
#define MODEL_H

#include <GL/glut.h>
#include <bib/CameraDistante.h>
#include <bib/Desenha.h>
#include <string>

/** Classe Abstrata **/
class Model
{
public:
    Model();
    virtual void desenha() = 0;

    virtual void addAX(float ax) = 0;
    virtual void addAY(float ay) = 0;
    virtual void addAZ(float az) = 0;

    virtual void setAX(float ax) = 0;
    virtual void setAY(float ay) = 0;
    virtual void setAZ(float az) = 0;

    virtual float getAX() = 0;
    virtual float getAY() = 0;
    virtual float getAZ() = 0;

    virtual void setTX(float tx) = 0;
    virtual void setTY(float ty) = 0;
    virtual void setTZ(float tz) = 0;

    virtual void addTX(float tx) = 0;
    virtual void addTY(float ty) = 0;
    virtual void addTZ(float tz) = 0;

    virtual void setSX(float sx) = 0;
    virtual void setSY(float sy) = 0;
    virtual void setSZ(float sz) = 0;

    virtual void addSX(float sx) = 0;
    virtual void addSY(float sy) = 0;
    virtual void addSZ(float sz) = 0;

    virtual float getSX() = 0;
    virtual float getSY() = 0;
    virtual float getSZ() = 0;

    virtual float getTX() = 0;
    virtual float getTY() = 0;
    virtual float getTZ() = 0;

    virtual void setSelecionado(bool selecionado) = 0;
    virtual bool isSelecionado() = 0;

    virtual void setSombra(bool sombra) = 0;
    virtual bool isSombra() = 0;

    virtual void setEixo(bool eixo) = 0;
    virtual bool isEixo() = 0;

    virtual void addStacks() = 0;
    virtual void addSlices() = 0;

    virtual void decStacks() = 0;
    virtual void decSlices() = 0;

    virtual int getSlices() = 0;
    virtual int getStacks() = 0;

    virtual std::string getNome() = 0;

    std::string nome = "Model";
private:
    float ax = 0.0;
    float ay = 0.0;
    float az = 0.0;

    float tx = 0.0;
    float ty = 0.0;
    float tz = 0.0;

    float sx = 1.0;
    float sy = 1.0;
    float sz = 1.0;
};

#endif // MODEL_H
