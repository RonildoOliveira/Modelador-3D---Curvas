#include "entities/models/objmodelloader.h"

using namespace std;

ObjModelLoader::ObjModelLoader()
{

}

ObjModelLoader::ObjModelLoader(string filename, string nomeModelo,
        float tx, float ty, float tz, float ax, float ay, float az, float sx, float sy, float sz){
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;

    this->ax = ax;
    this->ay = ay;
    this->az = az;

    this->sx = sx;
    this->sy = sy;
    this->sz = sz;

    this->filename = filename;
    this->nome = nomeModelo;

    fstream *objFile = new fstream;
    objFile->open(filename, ios_base::in);

    if (!objFile->is_open())
    {
        cout << "O arquivo [" << filename << "] não pode ser lido." << endl;
        exit(EXIT_FAILURE);
    }

    string currentLine;

    while (!objFile->eof())
    {
        getline(*objFile, currentLine);

        // Split
        vector<string> *parameters = this->GetSplittedStrings(currentLine, ' ');

        //CASO V (Vertices)
        if (parameters->at(0) == "v")
        {
            // Remove 'v'
            parameters->erase(parameters->begin());

            vector<float> *currentPoint = new vector<float>;

            for (int index = 0; index < parameters->size(); index++)
            {
                // String (x, y, z) to Float (x, y, z)
                currentPoint->push_back(this->GetFloatFromString(parameters->at(index)));
            }
            vertices->push_back(currentPoint);
        }

        //CASO F (Faces)
        else if (parameters->at(0) == "f")
        {
            // Remove 'f'
            parameters->erase(parameters->begin());

            vector<int> *vertexIndexes = new vector<int>;

            for (int index = 0; index < parameters->size(); index++)
            {
                // String (face_index) to Int (face_index)
                int faceIndex = this->GetFloatFromString(parameters->at(index));

                // Obj possui index a partir de 1
                // A origem deve ser 0
                vertexIndexes->push_back(--faceIndex);
            }

            faces->push_back(vertexIndexes);
        }

        delete parameters;
    }

    objFile->close();
}

ObjModelLoader::ObjModelLoader(string filename, string nome)
{
    this->filename = filename;
    this->nome = nome;

    fstream *objFile = new fstream;
    objFile->open(filename, ios_base::in);

    if (!objFile->is_open())
    {
        cout << "O arquivo [" << filename << "] não pode ser lido." << endl;
        exit(EXIT_FAILURE);
    }

    string currentLine;

    while (!objFile->eof())
    {
        getline(*objFile, currentLine);

        // Split
        vector<string> *parameters = this->GetSplittedStrings(currentLine, ' ');

        //CASO V (Vertices)
        if (parameters->at(0) == "v")
        {
            // Remove 'v'
            parameters->erase(parameters->begin());

            vector<float> *currentPoint = new vector<float>;

            for (int index = 0; index < parameters->size(); index++)
            {
                // String (x, y, z) to Float (x, y, z)
                currentPoint->push_back(this->GetFloatFromString(parameters->at(index)));
            }
            vertices->push_back(currentPoint);
        }

        //CASO F (Faces)
        else if (parameters->at(0) == "f")
        {
            // Remove 'f'
            parameters->erase(parameters->begin());

            vector<int> *vertexIndexes = new vector<int>;

            for (int index = 0; index < parameters->size(); index++)
            {
                // String (face_index) to Int (face_index)
                int faceIndex = this->GetFloatFromString(parameters->at(index));

                // Obj possui index a partir de 1
                // A origem deve ser 0
                vertexIndexes->push_back(--faceIndex);
            }

            faces->push_back(vertexIndexes);
        }

        delete parameters;
    }

    objFile->close();
}

void ObjModelLoader::desenha()
{
    glPushMatrix();

    //composicao de transformacoes
    glTranslated(this->getTX(),this->getTY(),this->getTZ());

    glRotated(this->getAX(),0,0,1);
    glRotated(this->getAY(),0,1,0);
    glRotated(this->getAZ(),1,0,0);

    glScaled(this->getSX(),this->getSY(),this->getSZ());

    //desenhando eixos do sistema de coordenadas local 1
    if(isEixo()){
        Desenha::drawEixos( 0.5 );
    }

    //desenhando objeto

    if(isSelecionado()){
        glColor3f(0.22,1.0,0.07);
    }
    else if (isSombra()){
        glColor3f(0.0,0.0,0.0);
    }
    else{
        glColor3f(0.5,0.5,0.0);
    }


    // Triangulacao
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < faces->size(); i++)
    {
        float *normal = NULL;

        float coord1[3],
                coord2[3],
                coord3[3];

        for (int j = 0; j < (faces->at(i))->size(); j++)
        {
            int index = (faces->at(i))->at(j);

            switch(j)
            {
            // vertice 1
            case 0:
                coord1[0] = (vertices->at(index))->at(0);
                coord1[1] = (vertices->at(index))->at(1);
                coord1[2] = (vertices->at(index))->at(2);
                break;

                // vertice 2
            case 1:
                coord2[0] = (vertices->at(index))->at(0);
                coord2[1] = (vertices->at(index))->at(1);
                coord2[2] = (vertices->at(index))->at(2);
                break;

                // vertice 3
            case 2:
                coord3[0] = (vertices->at(index))->at(0);
                coord3[1] = (vertices->at(index))->at(1);
                coord3[2] = (vertices->at(index))->at(2);
                break;
            }

            // Calculate normal
            if (j == 2)
            {
                //NORMAL mente isso nao funciona com objetos complexos
                normal = this->GetNormal(coord1, coord2, coord3);

                if(normal != NULL){
                    // Aplica normal ao trianulo
                    glNormal3f(normal[0], normal[2], normal[2]);
                }

                // Face triangula a partir dos vertices
                glVertex3f(coord1[0], coord1[1], coord1[2]);
                glVertex3f(coord2[0], coord2[1], coord2[2]);
                glVertex3f(coord3[0], coord3[1], coord3[2]);
            }
        }
    }

    glEnd();
    glPopMatrix();
}

float ObjModelLoader::getAX() { return this->ax; }
float ObjModelLoader::getAY() { return this->ay; }
float ObjModelLoader::getAZ() { return this->az; }

float ObjModelLoader::getSize() {}
void ObjModelLoader::setSize(float size){}

void ObjModelLoader::addAX(float ax) { this->ax+=ax; }
void ObjModelLoader::addAY(float ay) { this->ay+=ay; }
void ObjModelLoader::addAZ(float az) { this->az+=az; }

void ObjModelLoader::setAX(float ax) { this->ax = ax; }
void ObjModelLoader::setAY(float ay) { this->ay = ay; }
void ObjModelLoader::setAZ(float az) { this->az = az; }

void ObjModelLoader::addSX(float sx) { this->sx+=sx; }
void ObjModelLoader::addSY(float sy) { this->sy+=sy; }
void ObjModelLoader::addSZ(float sz) { this->sz+=sz; }

void ObjModelLoader::setSX(float sx) { this->sx = sx; }
void ObjModelLoader::setSY(float sy) { this->sy = sy; }
void ObjModelLoader::setSZ(float sz) { this->sz = sz; }

float ObjModelLoader::getSX() { return this->sx; }
float ObjModelLoader::getSY() { return this->sy; }
float ObjModelLoader::getSZ() { return this->sz; }

void ObjModelLoader::setTX(float tx) { this->tx = tx; }
void ObjModelLoader::setTY(float ty) { this->ty = ty; }
void ObjModelLoader::setTZ(float tz) { this->tz = tz; }

float ObjModelLoader::getTX() { return this->tx; }
float ObjModelLoader::getTY() { return this->ty; }
float ObjModelLoader::getTZ() { return this->tz; }

void ObjModelLoader::addTX(float tx) { this->tx+=tx; }
void ObjModelLoader::addTY(float ty) { this->ty+=ty; }
void ObjModelLoader::addTZ(float tz) { this->tz+=tz; }

void ObjModelLoader::addSlices(){}
void ObjModelLoader::addStacks(){}

void ObjModelLoader::decSlices(){}
void ObjModelLoader::decStacks(){}
int ObjModelLoader::getSlices(){}
int ObjModelLoader::getStacks(){}

void ObjModelLoader::setSelecionado(bool selecionado) { this->selecionado = selecionado; }
bool ObjModelLoader::isSelecionado() { return this->selecionado; }

void ObjModelLoader::setSombra(bool sombra) { this->sombra = sombra; }
bool ObjModelLoader::isSombra() { return this->sombra; }

void ObjModelLoader::setEixo(bool eixo) { this->eixo = eixo; }
bool ObjModelLoader::isEixo() { return this->eixo; }

string ObjModelLoader::getNome() { return this->nome; }

vector<string>* ObjModelLoader::GetSplittedStrings(string text, char delimeter)
{
    text += delimeter;

    string word = "";
    vector<string> *words = new vector<string>;

    for (int character = 0; character < text.length(); character++)
    {
        if (text[character] != delimeter)
        {
            word += text[character];
        }
        else
        {
            words->push_back(word);
            word = "";
        }
    }

    return words;
}

float ObjModelLoader::GetFloatFromString(string text)
{
    float value = 0.0f;

    istringstream buffer(text);
    buffer >> value;

    return value;
}

float* ObjModelLoader::GetNormal(float *coord1, float *coord2, float *coord3)
{
    // GEt Vector1 Vector2
    float va[3], vb[3], vr[3];

    va[0] = coord1[0] - coord2[0];
    va[1] = coord1[1] - coord2[1];
    va[2] = coord1[2] - coord2[2];

    vb[0] = coord1[0] - coord3[0];
    vb[1] = coord1[1] - coord3[1];
    vb[2] = coord1[2] - coord3[2];

    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];

    // Fator Normal
    float val = sqrt(pow(vr[0], 2) + pow(vr[1], 2) + pow(vr[2], 2));

    float norm[3];

    norm[0] = vr[0] / val;
    norm[1] = vr[1] / val;
    norm[2] = vr[2] / val;

    //Gamby :v
    glNormal3f(norm[0], norm[1], norm[2]);

    return norm;
}

ObjModelLoader::~ObjModelLoader()
{
    delete this->faces;
    delete this->vertices;
}
