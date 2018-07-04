#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QtOpenGL>
#include <QOpenGLWidget>

#include <GL/glut.h>

#include <vector>
#include<entities/model.h>

#include <bib/CameraDistante.h>
#include <bib/Desenha.h>

#include "curvas/spline.h"

using namespace  std;

class OGLWidget : public QGLWidget
{
public:
    OGLWidget(QWidget *parent = 0);
    void initializeGL();
    void paintGL();
        void renderizaCena();
        void displayInit();
        void displayPerspective();
        void displayOrtho();
        void displayEnd();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void mouseButton(int button, int state, int x, int y);

    void addTorusListaModelos();
    void addTeapotListaModelos();
    void addCubeListaModelos();
    void addArvoreListaModelos();

    void addLuzListaModelos();

    void addKratosListaModelos();
    void addBoyListaModelos();
    void addMarioListaModelos();
    void addShelfListaModelos();
    void addAviaoListaModelos();

    void addEsqueletoListaModelos();
    void addCachorroListaModelos();
    void addLoboListaModelos();

    void increaseCont();
    void decreaseCont();

    void mudancasEixoX(char tipo, float valor);
    void mudancasEixoY(char tipo, float valor);
    void mudancasEixoZ(char tipo, float valor);

    void mudancasAngulo(float x, float y, float z);
    void mudancasTranslacao(float x, float y, float z);
    void mudancasEscala(float x, float y, float z);

    void mudaCamera(int numeroCamera);

    void carregarEstado();
    void salvarEstado();
    void carregarModelo3DOBJ(string caminho, string nome);
    void carregarModelo3D3DS(string caminho, string nome);

    void iniciaLuz();
    void carregaCamera();
    void salvaCamera();
    void iniciaCurva();

    int picking(GLint cursorX, GLint cursorY, int w, int h );

    //Camera* cam = new CameraDistante(19.5323,9.36482,24.4429,-0.0726667,0.927333,0,-0.162686,0.965604,-0.202832);
    Camera* cam = new CameraDistante(0,0,0, 0,0,0, 0,0,0);
    float savedCamera[9];

    bool islistaVazia();

    void mudarCurva(SplineMode tipoCurva);
    void mostrarPontosCtrl();
    void mostrarCaminho();

    void percorrer();

    ~OGLWidget();

public slots:
    void buttonPressEvent();

private:
    vector<Model*> listaModelos;

    int cont = 0;

    int width = 800;
    int height = 600;

    int slices = 20;
    int stacks = 20;

    float trans_obj = false;

    /*

    float tx = 0.0;
    float ty = 0.0;
    float tz = 0.0;

    float ax = 0.0;
    float ay = 0.0;
    float az = 0.0;

    */
    float delta = 5.0;

    float sx = 1.0;
    float sy = 1.0;
    float sz = 1.0;

    QTimer timer;
    QPoint lastPos;

protected:

};


#endif // OGLWIDGET_H
