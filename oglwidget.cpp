#include <QKeyEvent>
#include <QMessageBox>

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "oglwidget.h"
#include "bib/GUI.h"

#include "entities/model.h"
#include "entities/torus.h"
#include "entities/teapot.h"
#include "entities/cube.h"
#include "entities/tree.h"
#include "entities/luz.h"

#include "entities/models/objmodelloader.h"
#include "entities/models/tdsmodelloader.h"

#include "curvas/pontocontrole.h"
#include "curvas/spline.h"

float trans_obj = false;
float trans_luz = false;

bool isPerspective = false;
bool isOrtogonal = false;

int indice_luz = -1;

Camera* cam2 = new CameraDistante(-3,2,-5, 0,0,0, 0,1,0);

/** DBDB **/
//ponto em coords locais, a ser calculado em coords de mundo
float pl[4] = { 0.0, 0.0, 0.0, 1.0 };
//ponto em coords globais, resultado da conversao a partir de pl
float pg[4] = { 0.0, 0.0, 0.0, 1.0 };

bool lbpressed = false;
bool mbpressed = false;
bool rbpressed = false;
bool desenhaEixos = true;
bool desenhaGrade = true;

float last_x = 0.0;
float last_y = 0.0;

int pontoSelecionado = 0;

vector<PontoControle*> pontosControle;
int indiceObjeto = -1;

bool mostrarPontosControle = false;
bool blMostrarCaminho = false;
bool blPercorrer = false;

Spline *spline;

OGLWidget::OGLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    connect( &timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
    timer.start(16);
}

void OGLWidget::initializeGL()
{
    carregaCamera();
    iniciaLuz();

    iniciaCurva();

    //glClearColor(1,1,1,1);
    //Cor fundo ambiente
    glClearColor(0.9,0.9,0.9,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE); //mantem a qualidade da iluminacao mesmo quando glScalef eh usada

    //glShadeModel(GL_SMOOTH);
    glShadeModel(GL_FLAT);

    glEnable(GL_DEPTH_TEST);

    //    //definindo uma luz
    glEnable(GL_LIGHT0);

    const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

void transformacao_camera_2_global(Vetor3D e, Vetor3D c, Vetor3D u, bool mostra_matriz = false){
    //matriz de transformacao
    float transform[16] = {
        1.0,    0.0,    0.0,    0.0,
        0.0,    1.0,    0.0,    0.0,
        0.0,    0.0,    1.0,    0.0,
        0.0,    0.0,    0.0,    1.0
    };
    glMultTransposeMatrixf( transform );
}

//processa as intersecoes
int processHits( GLint hits, GLuint buffer[] ) {
    //for each hit in buffer
    //Number of names in the name stack
    //Minimum depth of the object
    //Maximum depth of the object
    //List of names of the name stack

    int i;
    GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

    ptrNames = NULL;

    printf("Hits = %d\n",hits);
    printf("Buffer = ");
    for (i = 0; i < 4*hits; i++) {
        printf("%u ",buffer[i]);
    }
    printf("\n");

    ptr = (GLuint *) buffer;
    minZ = 0xffffffff;
    for (i = 0; i < hits; i++) {
        names = *ptr;
        ptr++;
        if (*ptr < minZ) {
            numberOfNames = names;
            minZ = *ptr;
            if (numberOfNames != 0)
                ptrNames = ptr+2;
        }
        //}
        ptr += names+2;
    }

    if (ptrNames == NULL)
        return 0;
    else
        return *ptrNames;
}

//picking
#define BUFSIZE 512
GLuint selectBuf[BUFSIZE];
int OGLWidget::picking( GLint cursorX, GLint cursorY, int w, int h ) {
    GLint viewport[4];

    glDisable(GL_LIGHTING);

    glSelectBuffer(BUFSIZE,selectBuf);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT,viewport);
    gluPickMatrix(cursorX,viewport[3]-cursorY,w,h,viewport);

    //de acordo com a implementacao
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float ar = height>0 ? (float) width / (float) height : 1.0;

    //lembrar de nao inicializar a matriz de projecao,
    //pois a gluPickMatrix é que redefine os planos de corte
    //do volume de visualizacao reduzido apenas na vizinhanca
    //do pixel selecionado pelo mouse
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    gluPerspective(30.,ar,0.1,1000.);

    glMatrixMode(GL_MODELVIEW);
    glInitNames();

    glLoadIdentity();


    //matrixGlobal2Cam(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);

    //sistema local 1
    glPushMatrix();
    //composicao de transformacoes
    glTranslated(0,0,0);
    glRotated(0,0,0,1);
    glRotated(0,0,1,0);
    glRotated(0,1,0,0);
    glScaled(sx,sy,sz);

    renderizaCena();
    glPopMatrix();

    //fim-de acordo com a implementacao

    glEnable(GL_LIGHTING);

    //processando as intersecoes
    int hits;

    // returning to normal rendering mode
    hits = glRenderMode(GL_RENDER);

    // if there are hits process them
    if (hits != 0) {
        return processHits(hits,selectBuf);
    } else {
        return 0;
    }
}


void OGLWidget::renderizaCena(){
    /*
        if(isPerspective){
            displayPerspective();
        }
        else if(isOrtogonal){
            displayOrtho();
        }else{
            displayInit();
        }
    */
        //displayInit();


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);

        //sistema global
        glPushMatrix();
            //desenhando eixos do sistema de coordenadas global
            Desenha::drawEixos( 0.5 );
            //chao
            glColor3d(0.3,0.3,0.3);
            Desenha::drawGrid( 15, 0, 15, 1 );
        glPopMatrix();


        float sombreamentoGlobal[16] = {
            listaModelos.at(0)->getTY(), -listaModelos.at(0)->getTX(),                0.01,                0.0,
            0.0,            0.01,                 0.0,                0.0,
            0.0, -listaModelos.at(0)->getTZ(),      listaModelos.at(0)->getTY(),                0.0,
            0.0, -listaModelos.at(0)->getTY(),                 0.0,      listaModelos.at(0)->getTY()
        };

        float k_x = -9.95;
        float sombra_x[16] = {
            -k_x,                    0.0,                    0.0,  k_x * listaModelos.at(0)->getTX(),
            -listaModelos.at(0)->getTY(), (listaModelos.at(0)->getTX() - k_x),                    0.0,  k_x * listaModelos.at(0)->getTY(),
            -listaModelos.at(0)->getTZ(),                    0.0, (listaModelos.at(0)->getTX() - k_x),  k_x * listaModelos.at(0)->getTZ(),
            -1.0,                    0.0,                    0.0,        listaModelos.at(0)->getTX()
        };

        float k_y = 0.05;
        float sombra_y[16] = {
            listaModelos.at(0)->getTY() - k_y,   -listaModelos.at(0)->getTX(),                   0.0,   k_y * listaModelos.at(0)->getTX(),
            0.0,              -k_y,                   0.0,   k_y * listaModelos.at(0)->getTY(),
            0.0,   -listaModelos.at(0)->getTZ(),  listaModelos.at(0)->getTY() - k_y,   k_y * listaModelos.at(0)->getTZ(),
            0.0,   -listaModelos.at(0)->getTZ(),                   0.0,         listaModelos.at(0)->getTY()
        };

        float k_z = -9.95;

        float sombra_z[16] = {
            listaModelos.at(0)->getTZ() - k_z,                  0.0,   -listaModelos.at(0)->getTX(),  k_z * listaModelos.at(0)->getTX(),
            0.0, listaModelos.at(0)->getTZ() - k_z,   -listaModelos.at(0)->getTY(),  k_z * listaModelos.at(0)->getTY(),
            0.0,                  0.0,              -k_z,  k_z * listaModelos.at(0)->getTZ(),
            0.0,                  0.0,              -1.0,        listaModelos.at(0)->getTZ()
        };

        //sombras
        glPushMatrix();
            //posicao da luz
            glutGUI::trans_luz = trans_luz;
            GUI::setLight(0,
                          listaModelos.at(0)->getTX(),
                          listaModelos.at(0)->getTY(),
                          listaModelos.at(0)->getTZ(),
                          false,false);
            //GUI::setLight(0,posicao_luz[0],posicao_luz[1],posicao_luz[2],false,false);
            GLfloat posicao_luz[] = {
                    listaModelos.at(0)->getTX(), listaModelos.at(0)->getTY(),
                    listaModelos.at(0)->getTZ(), listaModelos.at(0)->getTZ()
                    };
            glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
        glPopMatrix();

        glPushMatrix();
            //posicao da luz
            glutGUI::trans_luz = trans_luz;
            GUI::setLight(0,
                          listaModelos.at(0)->getTX(),
                          listaModelos.at(0)->getTY(),
                          listaModelos.at(0)->getTZ(),
                          false,false);
            //GUI::setLight(0,posicao_luz[0],posicao_luz[1],posicao_luz[2],false,false);
            GLfloat p_luz[] = {
                    listaModelos.at(0)->getTX(), listaModelos.at(0)->getTY(),
                    listaModelos.at(0)->getTZ(), listaModelos.at(0)->getTZ()
                    };
            glLightfv(GL_LIGHT0, GL_POSITION, p_luz);
        glPopMatrix();

        //sistema local da camera
        glPushMatrix();
            //definindo sistema de coordenadas atraves do glulookat (eye,center,up)
            transformacao_camera_2_global(cam2->e,cam2->c,cam2->u);
            //desenhando eixos do sistema de coordenadas local da camera
            Desenha::drawEixos( 0.5 );
            //desenhando camera
            glColor3d(0.5,0.5,0.0);
            //desenha_camera(0.2);
        glPopMatrix();

        //posicao da luz
        glPushMatrix();
            glutGUI:: trans_luz = trans_luz;
            GUI::setLight(0,
                          listaModelos.at(0)->getTX(),
                          listaModelos.at(0)->getTY(),
                          listaModelos.at(0)->getTZ(),
                          false,false);
            //GUI::setLight(0,posicao_luz[0],posicao_luz[1],posicao_luz[2],false,false);
            glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
        glPopMatrix();


       glPushMatrix();
              glMultTransposeMatrixf( sombreamentoGlobal );
              glDisable(GL_LIGHTING);
              glColor3d(0.0,0.0,0.0);
              if(listaModelos.size() > 0){
                  for (unsigned int index = 0; index < listaModelos.size(); ++index) {
                      if( listaModelos.at(index)->isSombra()){
                          listaModelos.at(index)->desenha();
                      }
                  }
              }
              glEnable(GL_LIGHTING);
        glPopMatrix();

        //X
        for (unsigned int index = 0; index < listaModelos.size(); ++index) {
            glPushMatrix();
            glMultTransposeMatrixf(sombra_x);
            glDisable(GL_LIGHTING);
            for (unsigned int index = 0; index < listaModelos.size(); ++index) {
                if( listaModelos.at(index)->isSombra()){
                    listaModelos.at(index)->desenha();
                }
            }
            glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        //Y
        for (unsigned int index = 0; index < listaModelos.size(); ++index) {
            glPushMatrix();
            glMultTransposeMatrixf(sombra_y);
            glDisable(GL_LIGHTING);
            for (unsigned int index = 0; index < listaModelos.size(); ++index) {
                if( listaModelos.at(index)->isSombra()){
                    listaModelos.at(index)->desenha();
                }
            }
            glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        //Z
        for (unsigned int index = 0; index < listaModelos.size(); ++index) {
            glPushMatrix();
            glMultTransposeMatrixf(sombra_z);
            glDisable(GL_LIGHTING);
            for (unsigned int index = 0; index < listaModelos.size(); ++index) {
                if( listaModelos.at(index)->isSombra()){
                    listaModelos.at(index)->desenha();
                }
            }
            glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        //Padrao
        glPushMatrix();
            if(listaModelos.size() > 0){
                for (unsigned int index = 0; index < listaModelos.size(); ++index) {
                    if(mostrarPontosControle){
                        glPushName(index+1);
                            listaModelos.at(index)->desenha();
                        glPopName();
                    }
                }
            }
        glPopMatrix();


        /** DESENHA VETOR DE ESFERAS **/
        /*
        for(auto p : pontosControle){
            p->desenha();
        }
        */

        vector<Vetor3D> pontos;
        /*
        for(PontoControle* p : pontosControle){
            pontos.push_back(p->getValoresTranslacao());
        }
        */
        for(auto * p : listaModelos){
            //pontos.push_back(p->getValoresTranslacao());
            float tx = p->getTX();
            float ty = p->getTY();
            float tz = p->getTZ();
            Vetor3D v = {tx,ty,tz};
            pontos.push_back(v);
        }
        spline->setPontosControle(pontos);

        if(blMostrarCaminho){
            spline->desenharCurva();
        }

        if(blPercorrer){
            spline->desenharComSpline(true);
        }

        //Escape
        glPopMatrix();

        displayEnd();
}

void OGLWidget::paintGL()
{
    //visao de duas camera (duas viewports), viewport auxiliar sobrepondo a principal
    //permitindo alterar posicionamento e dimensoes da viewport auxiliar
    //Quadro
    displayInit();
    glViewport(0, 0, width, height);
    glLoadIdentity();
    gluLookAt(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);

    renderizaCena();

    glScissor(0, 3*height/4, width/3, height/3);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    glViewport(0, 3*height/4, width/3, height/3);
    glLoadIdentity();

    CameraDistante * cd = spline->getCameraDistante();
    gluLookAt(cd->e.x,cd->e.y,cd->e.z, cd->c.x,cd->c.y,cd->c.z, cd->u.x,cd->u.y,cd->u.z);
    if(blPercorrer){
        spline->desenharComSpline(true);
    }

    glViewport(0, 0, width, height);

    displayEnd();
}

void OGLWidget::displayPerspective(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float ar = height>0 ? (float) width / (float) height : 1.0;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
    gluPerspective(50.0, ar, 1, 1000);
    void gluPerspective(	GLdouble fovy,
    GLdouble aspect,
    GLdouble zNear,
    GLdouble zFar);
    */
    float near = 1;
    float fovy = 50.0;
    float top = tan(fovy/2) * near;
    float bottom = -top;
    float right = top * ar;
    float left = -top * ar;
    float M[4][4];

    float zNeg[16] = {
                            2 * near / (right - left), 0.0, 0.0, 0.0,
                            0.0, 2 * near / (top - bottom), 0.0, 0.0,
                            (right + left) / (right - left), (top + bottom) / (top - bottom),-(fovy + near) / (fovy - near), -1.0,
                            0.0, -2 * fovy * near / (fovy - near), 0.0, 1.0
                    };

    glMultTransposeMatrixf( zNeg );

    //gluPerspective(50.0, ar, 1, 1000);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);
}

void OGLWidget::displayOrtho()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float ar = height>0 ? (float) width / (float) height : 1.0;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /*
    float zNeg[16] = {
                            1.0, 0.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0,-1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0
                    };

    glMultTransposeMatrixf( zNeg );
    */

    /*
        glOrtho(left, right, bottom, top, near, far)
        left: minimum x we see
        right: maximum x we see
        bottom: minimum y we see
        top: maximum y we see
        -near: minimum z we see. Yes, this is -1 times near. So a negative input means positive z.
        -far: maximum z we see. Also negative.
    */
    glOrtho(-4.0 * ar, 4.0 * ar, -1.0, 5.0, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);
}

void OGLWidget::displayInit()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float ar = height>0 ? (float) width / (float) height : 1.0;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.,ar,0.1,1000.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);
}

void OGLWidget::displayEnd()
{
    //glutSwapBuffers();
}

void OGLWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;
}

void OGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Right:{
        if(!listaModelos.empty()){
            if(cont == -1){
                cont++;
                (listaModelos[cont])->setSelecionado(true);
            }else if((cont >= 0) && (cont < (listaModelos.size() - 1))){
                (listaModelos[cont])->setSelecionado(false);
                cont++;
                (listaModelos[cont])->setSelecionado(true);
            }else{
                (listaModelos[cont])->setSelecionado(false);
                cont = -1;
            }
        }
    }
        break;

    case Qt::Key_E:{
        if(!listaModelos.empty()){
            if((listaModelos[cont])->isSelecionado()){
                (listaModelos[cont])->setEixo(!(listaModelos[cont])->isEixo());
            }
        }
    }
        break;

    case Qt::Key_Escape:
    case Qt::Key_Return:
    case Qt::Key_Q:
        exit(0);
        break;

    case Qt::Key_T:
        trans_obj = !trans_obj;
        break;

    case Qt::Key_Plus:
    case Qt::Key_M:
        if(!listaModelos.empty()){
            if (listaModelos.at(cont)->getSlices() > 3 && listaModelos.at(cont)->getStacks() > 3)
            {
                listaModelos.at(cont)->addSlices();
                listaModelos.at(cont)->addStacks();
            }
        }
        break;

    case Qt::Key_Minus:
    case Qt::Key_N:
        if(!listaModelos.empty()){
            if (listaModelos.at(cont)->getSlices() > 3 && listaModelos.at(cont)->getStacks() > 3)
            {
                listaModelos.at(cont)->decSlices();
                listaModelos.at(cont)->decStacks();
            }
        }
        break;

    case Qt::Key_R:
        if(!listaModelos.empty()){
            listaModelos.erase (listaModelos.begin()+cont);
        }
        break;
    case Qt::Key_S:
        if(!listaModelos.empty()){
            if((listaModelos[cont])->isSelecionado()){
                (listaModelos[cont])->setSombra(!(listaModelos[cont])->isSombra());
            }
        }
        break;
    case Qt::Key_P:
        isPerspective = !isPerspective;
        break;
    case Qt::Key_O:
        isOrtogonal = !isOrtogonal;
        break;
    }
}

void OGLWidget::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();

    // if the left button is pressed
    if (event->buttons() == Qt::LeftButton) {
        int pick = picking(x,y,5,5);
        cout << "[]" << pick << "[]";
        if (pick == 0){
            lbpressed = true;
        }
        else{
            pontoSelecionado = pick;
            for(auto item : listaModelos){
                item->setSelecionado(false);
            }
            listaModelos.at(pontoSelecionado-1)->setSelecionado(true);
            cont = pontoSelecionado-1;
        }
    }
    else {// state = GLUT_UP
        lbpressed = false;
    }

    // if the middle button is pressed
    if (event->buttons() == Qt::MiddleButton) {
        // when the button is pressed
        mbpressed = true;
    }
    else {// state = GLUT_UP
        mbpressed = false;
    }

    // if the left button is pressed
    if (event->buttons() == Qt::RightButton) {
        // when the button is pressed
        rbpressed = true;
    }
    else {// state = GLUT_UP
        rbpressed = false;
    }
    last_x = x;
    last_y = y;

    lastPos = event->pos();
}

void OGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    int last_x = lastPos.x();
    int last_y = lastPos.y();


    //TODO AMBIENTE
    float fator = 10.0;
    if (event->buttons() & Qt::LeftButton) {
        if (!trans_obj) {
            cam->rotatex(y,last_y);
            cam->rotatey(x,last_x);
        } else {
            //ax += (y - last_y)/fator;
            //ay += (x - last_x)/fator;
        }
    }

    fator = 100.0;
    if (event->buttons() & Qt::RightButton) {
        if (!trans_obj) {
            cam->translatex(x,last_x);
            cam->translatey(y,last_y);
        } else {
            listaModelos.at(pontoSelecionado)->addAX((y - last_y)/fator);
            listaModelos.at(pontoSelecionado)->addAY((x - last_x)/fator);
            //ax += (y - last_y)/fator;
            //ay += (x - last_x)/fator;
        }
    }
    fator = 100.0;
    if (event->buttons() & Qt::RightButton && !event->buttons() & Qt::MiddleButton) {
        if (!trans_obj) {
            cam->translatex(x,last_x);
            cam->translatey(y,last_y);
        } else {
            listaModelos.at(pontoSelecionado)->addTX((x - last_x)/fator);
            listaModelos.at(pontoSelecionado)->addTY(-(y - last_y)/fator);
            //tx += (x - last_x)/fator;
            //ty += -(y - last_y)/fator;
        }
    }
    if (event->buttons() & Qt::LeftButton && event->buttons() & Qt::RightButton) {
        if (!trans_obj) {
            cam->zoom(y,last_y);
        } else {
            listaModelos.at(pontoSelecionado)->addTZ((y - last_y)/fator);
            fator = 10.0;
            listaModelos.at(pontoSelecionado)->addAZ(-(x - last_x)/fator);
            //tz += (y - last_y)/fator;
            //fator = 10.0;
            //az += -(x - last_x)/fator;
        }
    }
    fator = 100.0;
    if (event->buttons() & Qt::MiddleButton) {
        if (!trans_obj) {
            cam->zoom(y,last_y);
        }
    }

    last_x = x;
    last_y = y;

    lastPos = event->pos();
}

void OGLWidget::addTorusListaModelos() { this->listaModelos.push_back(new Torus()); }
void OGLWidget::addTeapotListaModelos() { this->listaModelos.push_back(new Teapot()); }
void OGLWidget::addCubeListaModelos() { this->listaModelos.push_back(new Cube()); }
void OGLWidget::addArvoreListaModelos(){ this->listaModelos.push_back(new Tree()); }

void OGLWidget::addLuzListaModelos() { this->listaModelos.push_back(new Luz()); }

void OGLWidget::addKratosListaModelos() { this->listaModelos.push_back(new ObjModelLoader("../modelador-3d-curvas/data/obj/Kratos.obj", "Kratos")); }
void OGLWidget::addBoyListaModelos() { this->listaModelos.push_back(new ObjModelLoader("../modelador-3d-curvas/data/obj/Boy.obj", "Boy")); }
void OGLWidget::addMarioListaModelos() { this->listaModelos.push_back(new ObjModelLoader("../modelador-3d-curvas/data/obj/Mario.obj", "Mario")); }
void OGLWidget::addShelfListaModelos() { this->listaModelos.push_back(new ObjModelLoader("../modelador-3d-curvas/data/obj/Shelf.obj", "Shelf")); }
void OGLWidget::addAviaoListaModelos() { this->listaModelos.push_back(new ObjModelLoader("../modelador-3d-curvas/data/obj/Aviao.obj", "Aviao")); }

void OGLWidget::addEsqueletoListaModelos() { this->listaModelos.push_back(new TdsModelLoader("../modelador-3d-curvas/data/3ds/Esqueleto.3ds", "Esqueleto")); }
void OGLWidget::addCachorroListaModelos() { this->listaModelos.push_back(new TdsModelLoader("../modelador-3d-curvas/data/3ds/Cachorro.3ds", "Cachorro")); }
void OGLWidget::addLoboListaModelos() { this->listaModelos.push_back(new TdsModelLoader("../modelador-3d-curvas/data/3ds/Lobo.3ds", "Lobo"));}

void OGLWidget::increaseCont() { this->cont++; }
void OGLWidget::decreaseCont() { this->cont--; }

void OGLWidget::mudancasEixoX(char tipo, float valor)
{
    if(tipo == 'T'){
        listaModelos.at(cont)->setTX(valor);
    }
    if(tipo == 'A'){
        listaModelos.at(cont)->setAX(valor);
    }
    if(tipo == 'S'){
        listaModelos.at(cont)->setSX(valor);
    }
}

void OGLWidget::mudancasEixoY(char tipo, float valor)
{
    if(tipo == 'T'){
        listaModelos.at(cont)->setTY(valor);
    }
    if(tipo == 'A'){
        listaModelos.at(cont)->setAY(valor);
    }
    if(tipo == 'S'){
        listaModelos.at(cont)->setSY(valor);
    }
}

void OGLWidget::mudancasEixoZ(char tipo, float valor)
{
    if(tipo == 'T'){
        listaModelos.at(cont)->setTZ(valor);
    }
    if(tipo == 'A'){
        listaModelos.at(cont)->setAZ(valor);
    }
    if(tipo == 'S'){
        listaModelos.at(cont)->setSZ(valor);
    }
}

void OGLWidget::mudancasAngulo(float x, float y, float z)
{
    listaModelos.at(cont)->setAX(x);
    listaModelos.at(cont)->setAY(y);
    listaModelos.at(cont)->setAZ(z);
}

void OGLWidget::mudancasTranslacao(float x, float y, float z)
{
    listaModelos.at(cont)->setTX(x);
    listaModelos.at(cont)->setTY(y);
    listaModelos.at(cont)->setTZ(z);
}

void OGLWidget::mudancasEscala(float x, float y, float z)
{
    listaModelos.at(cont)->setSX(x);
    listaModelos.at(cont)->setSY(y);
    listaModelos.at(cont)->setSZ(z);
}

void OGLWidget::mudaCamera(int numeroCamera)
{
    switch (numeroCamera) {
    case 1:
        this->cam = new CameraDistante(13.9053,2.46208,7.26844,0.237068,1.11059,-0.439761,-0.0747426,0.996312,-0.0421512);
        break;
    case 2:
        this->cam = new CameraDistante(19.5323, 19.36482, 24.4429, -0.0726667, 3.927333, 0, -0.162686, 1.965604, -0.202832);
        break;
    default:
        break;
    }

}

void OGLWidget::carregarEstado(){
    //medidas desesperadas
    listaModelos.clear();

    std::ifstream file("../modelador-3d-curvas/state.txt");
    if (!file) {
        cout << "Erro de leitura";
    }

    float tx, ty, tz = 0;

    while(!file.eof()){
        file >> tx >> ty >> tz;
        cout << tx << ty << tz;
        spline->addPontoControle(Vetor3D(tx, ty, tz));
        listaModelos.push_back(new PontoControle(Vetor3D(tx, ty,  tz)));
    }
/*
    for(Vetor3D v : spline->getPontosControle()){
        listaModelos.push_back(new PontoControle(v));
    }
*/
    //listaModelos.pop_back();
    /*
    std::ifstream file("../modelador-3d-curvas/state.txt");
    if (!file) {
        cout << "Erro de leitura";
    }

    string nomeModelo;
    float tx, ty, tz = 0;
    float ax, ay, az = 0;
    float sx, sy, sz = 0;

    while(!file.eof()){

        file >> nomeModelo;

        if(nomeModelo == "Torus"){
            file >> tx >> ty >> tz;
            file >> ax >> ay >> az;
            file >> sx >> sy >> sz;

            listaModelos.push_back(new Torus(tx, ty, tz, ax, ay, az, sx, sy, sz));
        }
        else if(nomeModelo == "Teapot"){
            file >> tx >> ty >> tz;
            file >> ax >> ay >> az;
            file >> sx >> sy >> sz;

            listaModelos.push_back(new Teapot(tx,ty,tz, ax,ay,az, sx,sy,sz));
        }

        else if(nomeModelo == "Cube"){
            file >> tx >> ty >> tz;
            file >> ax >> ay >> az;
            file >> sx >> sy >> sz;

            listaModelos.push_back(new Cube(tx,ty,tz, ax,ay,az, sx,sy,sz));
        }

        else if(nomeModelo == "Arvore"){
            file >> tx >> ty >> tz;
            file >> ax >> ay >> az;
            file >> sx >> sy >> sz;

            listaModelos.push_back(new Tree(tx,ty,tz, ax,ay,az, sx,sy,sz));
        }

        else if(nomeModelo == "Luz"){
            file >> tx >> ty >> tz;
            file >> ax >> ay >> az;
            file >> sx >> sy >> sz;

            listaModelos.push_back(new Luz(tx,ty,tz, ax,ay,az, sx,sy,sz));
        }

        else if(nomeModelo == "Kratos" || nomeModelo == "Mario" || nomeModelo == "Boy"
                || nomeModelo == "Shelf" || nomeModelo == "Aviao"){
            string diretorio = "../modelador-3d-curvas/data/obj/";
            string extensao = ".obj";

            file >> tx >> ty >> tz;
            file >> ax >> ay >> az;
            file >> sx >> sy >> sz;

            string param = diretorio+nomeModelo+extensao;

            listaModelos.push_back(new ObjModelLoader(param, nomeModelo, tx,ty,tz, ax,ay,az, sx,sy,sz));
        }

        else if(nomeModelo == "Esqueleto" || nomeModelo == "Cachorro" || nomeModelo == "Lobo"){
            string diretorio = "../modelador-3d-curvas/data/3ds/";
            string extensao = ".3ds";

            file >> tx >> ty >> tz;
            file >> ax >> ay >> az;
            file >> sx >> sy >> sz;

            string param = diretorio+nomeModelo+extensao;

            listaModelos.push_back(new TdsModelLoader(param, nomeModelo, tx,ty,tz, ax,ay,az, sx,sy,sz));
        }

    }

    listaModelos.pop_back();
    */
}

void OGLWidget::carregarModelo3DOBJ(string caminho, string nome)
{
    listaModelos.push_back(new ObjModelLoader(caminho, nome));
}

void OGLWidget::carregarModelo3D3DS(string caminho, string nome)
{
    const char * c = caminho.c_str();
    listaModelos.push_back(new TdsModelLoader(c, nome));
}

void OGLWidget::iniciaLuz()
{
    Luz * luz = new Luz(5.0,5.0,5.0,
                        0.0,0.0,0.0,
                        1.0,1.0,1.0);
    this->listaModelos.push_back(luz);
}

void OGLWidget::carregaCamera()
{
    std::ifstream file("../modelador-3d-curvas/camera.txt");
    string nomeModelo;

    GLfloat ex;
    GLfloat ey;
    GLfloat ez;
    GLfloat cx;
    GLfloat cy;
    GLfloat cz;
    GLfloat ux;
    GLfloat uy;
    GLfloat uz;

    if (!file) {
        cout << "Erro de leitura";
    }

    while(!file.eof()){

        file >> nomeModelo;
        file >> ex >> ey >> ez;
        file >> cx >> cy >> cz;
        file >> ux >> uy >> uz;

        this->cam = new CameraDistante(ex,ey,ez, cx,cy,cz, ux,uy,uz);
    }
}

void OGLWidget::salvaCamera()
{
    ofstream myfile ("../modelador-3d-curvas/camera.txt");

    myfile << "CameraDistante";

    if (myfile.is_open())
    {
        //ecu
        myfile << " " << this->cam->e.x;
        myfile << " " << this->cam->e.y;
        myfile << " " << this->cam->e.z;

        myfile << " " << this->cam->c.x;
        myfile << " " << this->cam->c.y;
        myfile << " " << this->cam->c.z;

        myfile << " " << this->cam->u.x;
        myfile << " " << this->cam->u.y;
        myfile << " " << this->cam->u.z;

        myfile.close();
    }

    else cout << "Erro de leitura";
}

void OGLWidget::salvarEstado()
{
    ofstream myfile ("../modelador-3d-curvas/state.txt");
    if (myfile.is_open())
    {
        for (int index = 0; index < listaModelos.size(); ++index) {

            myfile << listaModelos.at(index)->getNome() << " ";

            myfile << listaModelos.at(index)->getTX() << " ";
            myfile << listaModelos.at(index)->getTY() << " ";
            myfile << listaModelos.at(index)->getTZ() << " ";

            myfile << listaModelos.at(index)->getAX() << " ";
            myfile << listaModelos.at(index)->getAY() << " ";
            myfile << listaModelos.at(index)->getAZ() << " ";

            myfile << listaModelos.at(index)->getSX() << " ";
            myfile << listaModelos.at(index)->getSY() << " ";
            myfile << listaModelos.at(index)->getSZ() << " ";

            myfile << "\n";
        }

        myfile.close();
    }
    else cout << "Erro de leitura";
}

bool OGLWidget::islistaVazia()
{
    if(this->listaModelos.empty())
        return true;
    return false;
}

void OGLWidget::mudarCurva(SplineMode tipoCurva)
{
    tipoCurva = tipoCurva;
    spline->setMode(tipoCurva);
}

void OGLWidget::mostrarPontosCtrl()
{
    mostrarPontosControle = !mostrarPontosControle;
}

void OGLWidget::mostrarCaminho()
{
    blMostrarCaminho = !blMostrarCaminho;
}

void OGLWidget::percorrer()
{
    blPercorrer = !blPercorrer;
}

void OGLWidget::iniciaCurva(){
    spline = new Spline();

    //spline->addPontoControle(Vetor3D( 7, 0,  6));
    //spline->addPontoControle(Vetor3D( 8, 9,  0 ));
    //spline->addPontoControle(Vetor3D( 12, 10, 7));
    //spline->addPontoControle(Vetor3D(  3, 9, -7));
    //spline->addPontoControle(Vetor3D(  0, 5, -5));
    //spline->addPontoControle(Vetor3D( -2, 8, -6));
    //spline->addPontoControle(Vetor3D(-5, 4, -10));
    //spline->addPontoControle(Vetor3D(-6, 3, -2));
    //spline->addPontoControle(Vetor3D(-8, 6,   0));
    //spline->addPontoControle(Vetor3D(-7, 1,  3));

    //spline->addPontoControle(Vetor3D(-7, 0, 7));

    /*
    for(Vetor3D v : spline->getPontosControle()){
        //pontosControle.push_back(new PontoControle(v));
        listaModelos.push_back(new PontoControle(v));
    }
    */

}

OGLWidget::~OGLWidget() { }
