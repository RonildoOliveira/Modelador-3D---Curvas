#ifndef OBJMODELLOADER_H
#define OBJMODELLOADER_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/gl.h>
#include <entities/model.h>

class ObjModelLoader : public Model
{
public:
    ObjModelLoader();
    ObjModelLoader(std::string filename, std::__cxx11::string nome);
    ObjModelLoader(std::string param, std::string nomeModelo,
            float tx, float ty, float tz,
            float ax, float ay, float az,
            float sx, float sy, float sz);

    void desenha() override;

    float getAX();
    float getAY();
    float getAZ();

    float getSize();
    void  setSize(float size);

    void addAX(float ax) override;
    void addAY(float ay) override;
    void addAZ(float az) override;

    void setAX(float ax) override;
    void setAY(float ay) override;
    void setAZ(float az) override;

    void addSX(float sx) override;
    void addSY(float sy) override;
    void addSZ(float sz) override;

    void setSX(float sx) override;
    void setSY(float sy) override;
    void setSZ(float sz) override;

    float getSX() override;
    float getSY() override;
    float getSZ() override;

    void setTX(float tx) override;
    void setTY(float ty) override;
    void setTZ(float tz) override;

    float getTX() override;
    float getTY() override;
    float getTZ() override;

    void addTX(float tx) override;
    void addTY(float ty) override;
    void addTZ(float tz) override;

    void addSlices() override;
    void addStacks() override;
    void decSlices() override;
    void decStacks() override;

    int getSlices() override;
    int getStacks() override;

    void setSelecionado(bool selecionado) override;
    bool isSelecionado() override;

    void setSombra(bool sombra) override;
    bool isSombra() override;

    void setEixo(bool eixo) override;
    bool isEixo() override;

    std::string getNome() override;

    ~ObjModelLoader();
private:
    float ax = 0.0;
    float ay = 0.0;
    float az = 0.0;

    float tx = 0.0;
    float ty = 0.0;
    float tz = 0.0;

    float sx = 0.3;
    float sy = 0.3;
    float sz = 0.3;

    bool selecionado = false;
    bool sombra = false;
    bool eixo = false;

    std::string filename = "";
    std::vector<std::vector<float>*> *vertices = new std::vector<std::vector<float>*>;
    std::vector<std::vector<int>*>   *faces    = new std::vector<std::vector<int>*>;
    std::vector<std::string>* GetSplittedStrings(std::string text, char delimeter);
    float GetFloatFromString(std::string text);
    float* GetNormal(float *coord1, float *coord2, float *coord3);
};

#endif // OBJMODELLOADER_H
