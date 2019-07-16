#include <QKeyEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include "entities/models/objmodelloader.h"
using namespace  std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                          Qt::AlignCenter, this->size(),
                                          qApp->desktop()->availableGeometry()));
    ui->setupUi(this);

    connect(ui->actionCarregar_Estado,&QAction::triggered, this, &MainWindow::carregarEstado);
    connect(ui->actionSalvar_Estado,&QAction::triggered, this, &MainWindow::salvarEstado);
    connect(ui->actionSair, &QAction::triggered, this, &MainWindow::sair);
    connect(ui->actionCarregarOBJ, &QAction::triggered, this, &MainWindow::carregarOBJ);
    connect(ui->actionCarregar3DS, &QAction::triggered, this, &MainWindow::carregar3DS);

    connect(ui->actionTorus, &QAction::triggered, this, &MainWindow::addTorus);
    connect(ui->actionTeapot, &QAction::triggered, this, &MainWindow::addTeapot);
    connect(ui->actionCube, &QAction::triggered, this, &MainWindow::addCube);
    connect(ui->actionArvore, &QAction::triggered, this, &MainWindow::actionArvore);

    connect(ui->actionLuz, &QAction::triggered, this, &MainWindow::addLuz);

    connect(ui->actionKratos, &QAction::triggered, this, &MainWindow::addKratos);
    connect(ui->actionMario, &QAction::triggered, this, &MainWindow::addMario);
    connect(ui->actionBoy, &QAction::triggered, this, &MainWindow::addBoy);
    connect(ui->actionShelf, &QAction::triggered, this, &MainWindow::addShelf);
    connect(ui->actionAviao, &QAction::triggered, this, &MainWindow::actionAviao);

    connect(ui->actionEsqueleto, &QAction::triggered, this, &MainWindow::addEsqueleto);
    connect(ui->actionCachorro, &QAction::triggered, this, &MainWindow::addCachorro);
    connect(ui->actionLobo, &QAction::triggered, this, &MainWindow::addLobo);

    connect(ui->actionPerspectiva, &QAction::triggered, this, &MainWindow::mudarPerspectiva);
    connect(ui->actionOrtogonal, &QAction::triggered, this, &MainWindow::mudarOrtogonal);

    connect(ui->eixoXSlider, SIGNAL(valueChanged(int)), this, SLOT(xValueReceived(int)));
    connect(ui->eixoYSlider, SIGNAL(valueChanged(int)), this, SLOT(yValueReceived(int)));
    connect(ui->eixoZSlider, SIGNAL(valueChanged(int)), this, SLOT(zValueReceived(int)));

    //Curvas
    connect(ui->actionBezier, &QAction::triggered, this, &MainWindow::mudarCurvaBezier);
    connect(ui->actionB_Spline, &QAction::triggered, this, &MainWindow::mudarCurvaBSpline);
    connect(ui->actionHermite, &QAction::triggered, this, &MainWindow::mudarCurvaHermite);
    connect(ui->actionCatmullrom, &QAction::triggered, this, &MainWindow::mudarCurvaCatmullrom);

    connect(ui->actionMostrar_Pontos_Ctrl, &QAction::triggered, this, &MainWindow::mostrarPontosCtrl);
    connect(ui->actionCaminho, &QAction::triggered, this, &MainWindow::mostrarCaminho);

    connect(ui->actionPercorrer, &QAction::triggered, this, &MainWindow::percorrer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_slicestackSubButton_clicked(){
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_N,Qt::NoModifier,"N");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::on_slicestackAddButton_clicked(){
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_M,Qt::NoModifier,"M");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::on_nextObject_clicked(){
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier,"Key_Right");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::on_prevObject_clicked(){
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier,"Key_Left");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::on_sombraButton_clicked()
{
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_S,Qt::NoModifier,"Key_S");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::on_eixoButton_clicked()
{
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_E,Qt::NoModifier,"Key_E");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::on_aplicarTransformacao_clicked()
{
    ui->campoTransformacao->setInputMask( "\00.0000,\00.0000,\00.0000" );
    QString qTexto = ui->campoTransformacao->text();
    std::string texto = qTexto.toUtf8().constData();
    spliter(texto, ",");
}

void MainWindow::carregarEstado()
{
    ui->openGLWidget->carregarEstado();
}

void MainWindow::salvarEstado()
{
    ui->openGLWidget->salvarEstado();
}

void MainWindow::carregarOBJ()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Carregar Modelo 3D"), "/home/", tr("Wavefront (*.obj)"));
    std::string utf8_text = filename.toUtf8().constData();

    ui->openGLWidget->carregarModelo3DOBJ(utf8_text,"Novo");
    //std::cout << utf8_text << "-->";
}

void MainWindow::carregar3DS()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Carregar Modelo 3D"), "/home/", tr("3D Studio (*.3ds)"));
    std::string utf8_text = filename.toUtf8().constData();

    ui->openGLWidget->carregarModelo3D3DS(utf8_text,"Novo3DS");
    //std::cout << utf8_text << "-->";
}


void MainWindow::on_removerObjButton_clicked(){
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_R,Qt::NoModifier,"R");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::addTorus(){
    ui->openGLWidget->addTorusListaModelos();
    /*
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.exec();
    */
}

void MainWindow::addLuz()
{
    ui->openGLWidget->addLuzListaModelos();
}

void MainWindow::addTeapot(){
    ui->openGLWidget->addTeapotListaModelos();
}

void MainWindow::addCube()
{
    ui->openGLWidget->addCubeListaModelos();
}

void MainWindow::actionArvore()
{
    ui->openGLWidget->addArvoreListaModelos();
}

void MainWindow::addKratos()
{
    ui->openGLWidget->addKratosListaModelos();
}

void MainWindow::addMario()
{
    ui->openGLWidget->addMarioListaModelos();
}

void MainWindow::addBoy()
{
    ui->openGLWidget->addBoyListaModelos();
}

void MainWindow::addShelf()
{
    ui->openGLWidget->addShelfListaModelos();
}

void MainWindow::actionAviao()
{
    ui->openGLWidget->addAviaoListaModelos();
}

void MainWindow::addEsqueleto()
{
    ui->openGLWidget->addEsqueletoListaModelos();
}

void MainWindow::addCachorro()
{
    ui->openGLWidget->addCachorroListaModelos();
}

void MainWindow::addLobo()
{
    ui->openGLWidget->addLoboListaModelos();
}

void MainWindow::mudarPerspectiva()
{
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_P,Qt::NoModifier,"P");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::mudarOrtogonal()
{
    QKeyEvent * eve1 = new QKeyEvent (QEvent::KeyPress,Qt::Key_O,Qt::NoModifier,"O");
    ui->openGLWidget->keyPressEvent(eve1);
}

void MainWindow::mudarCurvaBezier()
{
    ui->curvaLabel->setText("Bezier");
    ui->openGLWidget->mudarCurva(SplineMode::BEZIER);
}

void MainWindow::mudarCurvaBSpline()
{
    ui->curvaLabel->setText("B-Spline");
    ui->openGLWidget->mudarCurva(SplineMode::BSPLINE);
}

void MainWindow::mudarCurvaHermite()
{
    ui->curvaLabel->setText("Hermite");
    ui->openGLWidget->mudarCurva(SplineMode::HERMITE);
}

void MainWindow::mudarCurvaCatmullrom()
{
    ui->curvaLabel->setText("Carmull-Rom");
    ui->openGLWidget->mudarCurva(SplineMode::CATMULLROM);
}

void MainWindow::mostrarPontosCtrl()
{
    ui->openGLWidget->mostrarPontosCtrl();
}

void MainWindow::mostrarCaminho()
{
    ui->openGLWidget->mostrarCaminho();
}

void MainWindow::percorrer()
{
    ui->openGLWidget->percorrer();
}

void MainWindow::xValueReceived(int x)
{
    float fator = 10.0;

    if(!ui->openGLWidget->islistaVazia()){
        if(ui->tRadioButton->isChecked()){
            ui->eixoXSlider->setMinimum(-800);
            ui->eixoXSlider->setMaximum(800);
            ui->openGLWidget->mudancasEixoX('T', (x / (fator*10)));
        }

        if(ui->aRadioButton->isChecked()){
            ui->eixoXSlider->setMinimum(0);
            ui->eixoXSlider->setMaximum(360);
            ui->openGLWidget->mudancasEixoX('A', (x));
        }

        if(ui->sRadioButton->isChecked()){
            ui->eixoXSlider->setMinimum(0);
            ui->eixoXSlider->setMaximum(30);
            ui->openGLWidget->mudancasEixoX('S', x);
        }
    }
}

void MainWindow::yValueReceived(int y)
{
    float fator = 10.0;

    if(ui->tRadioButton->isChecked()){
        ui->eixoYSlider->setMinimum(-800);
        ui->eixoYSlider->setMaximum(800);
        ui->openGLWidget->mudancasEixoY('T', (y / (fator)));
    }

    if(ui->aRadioButton->isChecked()){
        ui->eixoYSlider->setMinimum(0);
        ui->eixoYSlider->setMaximum(360);
        ui->openGLWidget->mudancasEixoY('A', (y));
    }

    if(ui->sRadioButton->isChecked()){
        ui->eixoYSlider->setMinimum(0);
        ui->eixoYSlider->setMaximum(30);
        ui->openGLWidget->mudancasEixoY('S', y);
    }
}

void MainWindow::zValueReceived(int z)
{
    float fator = 10.0;

    if(ui->tRadioButton->isChecked()){
        ui->eixoZSlider->setMinimum(-800);
        ui->eixoZSlider->setMaximum(800);
        ui->openGLWidget->mudancasEixoZ('T', (z / (fator*10)));
    }

    if(ui->aRadioButton->isChecked()){
        ui->eixoZSlider->setMinimum(0);
        ui->eixoZSlider->setMaximum(360);
        ui->openGLWidget->mudancasEixoZ('A', (z));
    }

    if(ui->sRadioButton->isChecked()){
        ui->eixoZSlider->setMinimum(0);
        ui->eixoZSlider->setMaximum(30);
        ui->openGLWidget->mudancasEixoZ('S', z);
    }
}

void MainWindow::spliter(string linha, string delimitador)
{
    std::vector<float> vetores;

    size_t pos = 0;
    std::string token;
    while ((pos = linha.find(delimitador)) != std::string::npos) {
        token = linha.substr(0, pos);
        std::cout << token << std::endl;
        linha.erase(0, pos + delimitador.length());

        vetores.push_back(::atof(token.c_str()));
    }
    vetores.push_back(::atof(linha.c_str()));
    std::cout << linha << std::endl;

    for (int var = 0; var < vetores.size(); ++var) {
        cout << vetores.at(var) << endl;
    }

    if(ui->aRadioButton->isChecked())
        ui->openGLWidget->mudancasAngulo(vetores.at(0),vetores.at(1),vetores.at(2));

    if(ui->sRadioButton->isChecked())
        ui->openGLWidget->mudancasEscala(vetores.at(0),vetores.at(1),vetores.at(2));

    if(ui->tRadioButton->isChecked())
        ui->openGLWidget->mudancasTranslacao(vetores.at(0),vetores.at(1),vetores.at(2));
}

void MainWindow::on_cam01PushButton_clicked() { ui->openGLWidget->mudaCamera(1); }
void MainWindow::on_cam02PushButton_clicked() { ui->openGLWidget->mudaCamera(2); }
void MainWindow::on_resetCameraPushButton_clicked() { ui->openGLWidget->carregaCamera(); }

void MainWindow::on_salvarCameraPushButton_clicked()
{
    ui->openGLWidget->salvaCamera();
}

void MainWindow::sair(){
    exit(0);
}


