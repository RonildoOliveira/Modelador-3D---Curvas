#ifndef TREE_H
#define TREE_H

#include <GL/glut.h>
#include <bib/CameraDistante.h>
#include <bib/Desenha.h>
#include<entities/model.h>

class Tree : public Model
{
public:
    Tree();
    Tree(float tx, float ty, float tz,
          float ax, float ay, float az,
          float sx, float sy, float sz);

    void desenha() override;

    float getInnerRadius();
    float getOutterRadius();

    void setInnerRadius(float innerRadius);
    void setOutterRadius(float outterRadius);

    float getAX();
    float getAY();
    float getAZ();

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

    void setEixo(bool eixo) override;
    bool isEixo() override;

    void setSombra(bool sombra) override;
    bool isSombra() override;

    std::string getNome() override;

private:
    std::string nome = "Arvore";

    int slices = 10;
    int stacks = 10;

    float trans_obj = false;

    float tx = 0.0;
    float ty = 0.0;
    float tz = 0.0;

    float ax = 0.0;
    float ay = 0.0;
    float az = 0.0;

    float delta = 5.0;

    float sx = 1.0;
    float sy = 1.0;
    float sz = 1.0;

    bool selecionado = false;
    bool sombra = false;
    bool eixo = false;
};

#endif // TREE_H
