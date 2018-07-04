#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_slicestackSubButton_clicked();
    void on_slicestackAddButton_clicked();
    void on_removerObjButton_clicked();

    void on_nextObject_clicked();
    void on_prevObject_clicked();

    void on_sombraButton_clicked();
    void on_eixoButton_clicked();
    void on_aplicarTransformacao_clicked();

    void carregarEstado();
    void salvarEstado();
    void carregarOBJ();
    void carregar3DS();
    void sair();

    void addLuz();

    void addTorus();
    void addTeapot();
    void addCube();
    void actionArvore();

    void addKratos();
    void addMario();
    void addBoy();
    void addShelf();
    void actionAviao();

    void addEsqueleto();
    void addCachorro();
    void addLobo();

    void mudarPerspectiva();
    void mudarOrtogonal();

    //Curvas
    void mudarCurvaBezier();
    void mudarCurvaBSpline();
    void mudarCurvaHermite();
    void mudarCurvaCatmullrom();

    void mostrarPontosCtrl();
    void mostrarCaminho();

    void percorrer();

    void xValueReceived(int x);
    void yValueReceived(int y);
    void zValueReceived(int z);

    void spliter(std::string linha, std::string delimitador);

    void on_cam01PushButton_clicked();
    void on_cam02PushButton_clicked();

    void on_resetCameraPushButton_clicked();
    void on_salvarCameraPushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

