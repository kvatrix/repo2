#include "Graph_.h"

#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

int WinW = 1200;
int WinH = 600;

int Mouse_Vert = -1;                            // Узнаем на какой вершине в данный момент курсор
bool isSplashScreen = false;                    // Показываем окно загрузки
bool standView = false;                         // Построить все вершины по кругу
bool isVertMove = false;                        // Отвечает за перемещение вершин
bool isCutting = false;                         // Идет линия отрезания
bool isCreateEdge = false;                      // Идет создание пути
bool isDrawingPath = false;                     // Идет отрисовка пути

int R = 50;                                     // Радиус вершин
int Button_Flag;                                // Если курсор в зоне кнопки, принимает значение номера кнопки (подсветка кнопок при наведении)
int Moving_Vertex;                              // Номер вершины, которую перемещаем
int CursorX;                                    // Позиция курсора по X
int CursorY;                                    // Позиция курсора по Y
int CreatingEdgeId = -1;                        // Из какой вершины в какую

vertCoord CuttingPos;                           // Начальная позиция X
vertCoord vertC[maxSize + 2];                   // Массив структур (коордианты вершин), +2 чтобы не вылетало искл. при заполнении до maxSize
Graph graph;

vector<pair<int, int>> Way;
vector<int> New_Way;

/* Описание работы методов класса */

Graph::Graph() {
    Mouse_Vert = -1;
    for (int i = 0; i < maxSize; i++)
        for (int j = 0; j < maxSize; j++)
            adjMatrix[i][j] = 0;
}

bool Graph::isEmpty() {
    return (vertList.size() == 0);
}

bool Graph::isFull() {
    return (vertList.size() == maxSize);
}


int Graph::getVertText(int i) {
    return vertList[i];
}

vector<int> Graph::getVertList() {
    return vertList;
}

int Graph::getAdjMatrixElem(int i, int j) {
    return adjMatrix[i][j];
}

int Graph::getAmountVerts() {
    return vertList.size();
}

int Graph::getVertPos(const int& vertex) {
    for (int i = 0; i < vertList.size(); i++)
        if (vertList[i] == vertex)
            return i;

    return -1;
}

int Graph::getAmountEdges() {
    int numb = 0;
    for (int i = 0; i < vertList.size(); i++) {
        for (int j = 0; j < vertList.size(); j++) {
            if (adjMatrix[i][j] > 0) {
                numb++;
            }
        }
    }
    return numb;
}

void Graph::setEdgeZero(int i, int j) {
    adjMatrix[i][j] = adjMatrix[j][i] = 0;
}

void Graph::insertVertex(const int& vertex) {
    if (isFull()) {
        cerr << "Граф уже заполнен!" << endl;
        return;
    }
    New_Way.clear();
    Way.clear();
    vertList.push_back(vertex);
}

void Graph::insertEdge(const int& vertex1, const int& vertex2, int weight) {
    if (weight < 1) {
        cerr << "Вес введен неверно. Минимальный вес: 1" << endl;
        return;
    }

    if (getVertPos(vertex1) == -1 || getVertPos(vertex2) == -1) {
        cerr << "Невозможно найти обе или одну из вершин" << endl;
        return;
    }

    int vertPos1 = getVertPos(vertex1);
    int vertPos2 = getVertPos(vertex2);

    if (adjMatrix[vertPos1][vertPos2] != 0) {
        cerr << "Между этими вершинами уже есть ребро!" << endl;
        return;
    }
    New_Way.clear();
    Way.clear();
    adjMatrix[vertPos1][vertPos2] = weight;
}

void Graph::print() {
    if (isEmpty()) {
        cerr << "Граф пустой" << endl;
        return;
    }

    cout << endl << "Матрица смежности графа: " << endl;
    for (int i = 0; i < vertList.size(); i++) {
        cout << setw(4) << vertList[i] << "|   ";
        for (int j = 0; j < vertList.size(); j++)
            cout << setw(4) << adjMatrix[i][j];
        cout << endl;
    }
}

void Graph::eraseLastVert() {
    if (isEmpty()) {
        cerr << "Граф пустой" << endl;
        return;
    }

    int n = vertList.size();
    for (int j = 0; j < n; j++) {
        adjMatrix[n - 1][j] = 0;
        adjMatrix[j][n - 1] = 0;
    }
    vertList.pop_back();
    New_Way.clear();
    Way.clear();
}

void Graph::eraseEdge(const int& vertex1, const int& vertex2) {
    if (getVertPos(vertex1) == -1 || getVertPos(vertex2) == -1) {
        cerr << "Невозможно найти обе или одну из вершин" << endl;
        return;
    }

    int vertPos1 = getVertPos(vertex1);
    int vertPos2 = getVertPos(vertex2);
    if (adjMatrix[vertPos1][vertPos2] == 0) {
        cerr << "Ребра между данными вершинами нет" << endl;
        return;
    }
    adjMatrix[vertPos1][vertPos2] = 0;
    New_Way.clear();
    Way.clear();
}

void Graph::drawGraph() {
    int n = graph.getAmountVerts();
    for (int i = 0; i < n; i++) {
        if (!standView)
            setCoords(i, n);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int a = adjMatrix[i][j];
            if (a == 0)
                continue;

            drawLine(a, vertC[i].x, vertC[i].y, vertC[j].x, vertC[j].y, j, i);
        }
    }
    drawVertex(n);
    glutPostRedisplay();
}

void Graph::cuttingFinish() {
    if (!isCutting)
        return;


    vertCoord cursor;
    cursor.x = CursorX;
    cursor.y = CursorY;

    int n = graph.getAmountVerts();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int a = adjMatrix[i][j];
            if (a == 0)
                continue;

            if (intersect(vertC[i], vertC[j], CuttingPos, cursor))
                graph.eraseEdge(i + 1, j + 1);
        }
    }
}

void Graph::alignmentVerts() {
    for (int i = 0; i < graph.getAmountVerts(); i++) {
        // Выравнимаем в рамки окна
        if (vertC[i].x + R > WinW) { vertC[i].x = WinW - R; }
        if (vertC[i].x < R) { vertC[i].x = R; }
        if (vertC[i].y + R > WinH) { vertC[i].y = WinH - R; }
        if (vertC[i].y < R) { vertC[i].y = R; }
    }
}

// Все остальное

int** Change_Matrix() {
    int n = graph.getAmountVerts();
    int** matrix = new int* [n];
    for (int i = 0; i < n; i++)
        matrix[i] = new int[n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int elem = graph.getAdjMatrixElem(i, j);
            if (elem == 0 || i == j)
                matrix[i][j] = -1;
            else 
                matrix[i][j] = elem;
        }
    }
    cout << "Начальная матрица:" << endl;
    printMatrix(matrix);
    return matrix;
}
    
int* searchMinElem(int* line, int n) {
    int min = 10000000;
    for (int j = 0; j < n; j++)
        if (line[j] >= 0 && line[j] < min)
            min = line[j];

    for (int j = 0; j < n; ++j)
        if (line[j] >= 0)
            line[j] -= min;

    return line;
}

void printMatrix(int** matrix) {
    int n = graph.getAmountVerts();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }
}

int** reductMatrix(int** oldmatrix) {
    int** matrix = oldmatrix;
    int n = graph.getAmountVerts();
    for (int i = 0; i < n; i++)
        matrix[i] = searchMinElem(matrix[i], n);
    for (int i = 0; i < n; i++) {
        int min = 10000000;
        for (int j = 0; j < n; j++) {
            if (matrix[j][i] >= 0 && matrix[i][j] < min)
                min = matrix[j][i];
        }
        for (int j = 0; j < n; j++) {
            if (matrix[j][i] >= 0)
                matrix[j][i] -= min;
        }
    }
    cout << endl << "Редукцированная таблица:" << endl;
    printMatrix(matrix);
    return matrix;
}

int** highZero(int** oldmatrix) {
    int n = graph.getAmountVerts();
    int** matrix = reductMatrix(oldmatrix);
    int max = -1;
    int line = 0, column = 0;
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            if (matrix[i][j] == 0) 
            {
                int minLine = 10000000;
                int minColumn = 10000000;
                for (int k = 0; k < n; k++) {
                    if (matrix[i][k] == -1 || k == j || matrix[i][k] >= minLine)
                        break;
                        
                    minLine = matrix[i][k];
                }
                for (int k = 0; k < n; k++) {
                    if (matrix[k][j] == -1 || k == i || matrix[k][j] >= minColumn)
                        break;

                    minColumn = matrix[k][j];
                }

                if (max < minColumn + minLine) {
                    max = minColumn + minLine;
                    line = i;
                    column = j;
                }
            }
        }
    }

    pair<int, int> p;
    p.first = line + 1;
    p.second = column + 1;
    Way.push_back(p);

    matrix[line][column] = -1;
    matrix[column][line] = -1;

    for (int i = 0; i < n; i++) {
        matrix[line][i] = -1;
        matrix[i][column] = -1;
    }
    cout << endl;
    cout << "Матрица после наибольшего 0:" << endl;
    printMatrix(matrix);
    cout << "Части пути: " << endl;
    for (int i = 0; i < Way.size(); i++)
        cout << Way[i].first << " -> " << Way[i].second << "    ";
    cout << endl;
    return matrix;
}

void printResult() {
    int second = Way[0].second;
    int i = 2;
    New_Way.push_back(Way[0].first);
    New_Way.push_back(Way[0].second);
    while (i != graph.getAmountVerts() + 1) {
        for (int j = 1; j < graph.getAmountVerts(); j++)
            if (Way[j].first == second) {
                second = Way[j].second;
                New_Way.push_back(second);
                i++;
            }
    }

    cout << "Ответ: " << endl;
    cout << "Путь: ";
    for (int i = 0; i < New_Way.size(); i++) {
        cout << New_Way[i];
        if (i != New_Way.size() - 1)
            cout << " -> ";
    }
    int sum = 0;
    for (int i = 0; i < Way.size(); i++) {
        int line = Way[i].first - 1;
        int column = Way[i].second - 1;
        sum += graph.getAdjMatrixElem(line, column);
    }
    cout << endl << "Кратчайшая длина пути: " << sum << endl;
}

void drawButton1() {
    if (Button_Flag != 1) { glColor3f(1.0f, 1.0f, 1.0f); } 
    else { glColor3f(0.3f, 1.0f, 0.3f); }


    glBegin(GL_QUADS);
    glVertex2i(50, WinH - 20);
    glVertex2i(50, WinH - WinH / 7);
    glVertex2i(WinW / 7, WinH - WinH / 7);
    glVertex2i(WinW / 7, WinH - 20);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, WinH - 20);
    glVertex2i(50, WinH - WinH / 7);
    glVertex2i(WinW / 7, WinH - WinH / 7);
    glVertex2i(WinW / 7, WinH - 20);
    glEnd();

    if (Button_Flag != 1) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Make new graph";
    glRasterPos2i(WinW / 16.5, 0.92 * WinH);
    for (int i = 0; i < name.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
    }
}

void drawButton2() {
    if (Button_Flag != 2) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }

    glBegin(GL_QUADS);
    glVertex2i(50, WinH - WinH / 7 - 20);
    glVertex2i(50, WinH - 2 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 2 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - WinH / 7 - 20);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, WinH - WinH / 7 - 20);
    glVertex2i(50, WinH - 2 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 2 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - WinH / 7 - 20);
    glEnd();

    if (Button_Flag != 2) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Salesman task";
    glRasterPos2i(WinW / 15.5, 0.77 * WinH);
    for (int i = 0; i < name.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton3() {
    if (Button_Flag != 3) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(50, WinH - 2 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 3 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 3 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 2 * (WinH / 7) - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, WinH - 2 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 3 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 3 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 2 * (WinH / 7) - 20);
    glEnd();

    string name = "Print matrix";
    string name1 = "In console";

    if (Button_Flag != 3) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    glRasterPos2i(WinW / 14.5, 0.63 * WinH);
    for (int i = 0; i < name.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
    }
    glRasterPos2i(WinW / 14, 0.63 * WinH - 15);
    for (int i = 0; i < name1.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name1[i]);
    }
}

void drawButton4() {
    if (Button_Flag != 4) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(50, WinH - 3 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 4 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 4 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 3 * (WinH / 7) - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, WinH - 3 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 4 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 4 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 3 * (WinH / 7) - 20);
    glEnd();

    if (Button_Flag != 4) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Add vertex";
    glRasterPos2i(WinW / 14, 0.48 * WinH);
    for (int i = 0; i < name.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
    }
}

void drawButton5() {
    if (Button_Flag != 5) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(50, WinH - 4 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 5 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 5 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 4 * (WinH / 7) - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, WinH - 4 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 5 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 5 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 4 * (WinH / 7) - 20);
    glEnd();

    if (Button_Flag != 5) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Delete vertex";
    glRasterPos2i(WinW / 14.5, 0.34 * WinH);
    for (int i = 0; i < name.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton6() {
    if (Button_Flag != 6) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(50, WinH - 5 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 6 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 6 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 5 * (WinH / 7) - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, WinH - 5 * (WinH / 7) - 20);
    glVertex2i(50, WinH - 6 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 6 * (WinH / 7));
    glVertex2i(WinW / 7, WinH - 5 * (WinH / 7) - 20);
    glEnd();

    if (Button_Flag != 6) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Add edge";
    glRasterPos2i(WinW / 13.3, 0.2 * WinH);
    for (int i = 0; i < name.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton7() {
    if (Button_Flag != 7) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(50, WinH - 6 * (WinH / 7) - 20);
    glVertex2i(50, 10);
    glVertex2i(WinW / 7, 10);
    glVertex2i(WinW / 7, WinH - 6 * (WinH / 7) - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, WinH - 6 * (WinH / 7) - 20);
    glVertex2i(50, 10);
    glVertex2i(WinW / 7, 10);
    glVertex2i(WinW / 7, WinH - 6 * (WinH / 7) - 20);
    glEnd();

    if (Button_Flag != 7) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Delete edge";
    glRasterPos2i(WinW / 15, 0.061 * WinH);
    for (int i = 0; i < name.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
    }
}

void drawButton8() {
    if (!graph.getAmountVerts())
        return;

    if (Button_Flag != 8) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(WinW - 50, WinH - 20);
    glVertex2i(WinW - 50, WinH - WinH / 7);
    glVertex2i(WinW - WinW / 7, WinH - WinH / 7);
    glVertex2i(WinW - WinW / 7, WinH - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(WinW - 50, WinH - 20);
    glVertex2i(WinW - 50, WinH - WinH / 7);
    glVertex2i(WinW - WinW / 7, WinH - WinH / 7);
    glVertex2i(WinW - WinW / 7, WinH - 20);
    glEnd();

    if (Button_Flag != 8) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Circular view";
    glRasterPos2i(WinW - WinW / 8, 0.92 * WinH);
    for (int i = 0; i < name.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton9() {
    if (!graph.getAmountVerts())
        return;

    if (Button_Flag != 9) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(WinW - 50, WinH - (WinH / 7) - 20);
    glVertex2i(WinW - 50, WinH - 2 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 2 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - (WinH / 7) - 20);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(WinW - 50, WinH - (WinH / 7) - 20);
    glVertex2i(WinW - 50, WinH - 2 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 2 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - (WinH / 7) - 20);
    glEnd();

    if (Button_Flag != 9) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "+R";
    glRasterPos2i(WinW - WinW / 8, 0.77 * WinH);
    for (int i = 0; i < name.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton10() {
    if (!graph.getAmountVerts())
        return;

    if (Button_Flag != 10) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.3f, 1.0f, 0.3f); }
    glBegin(GL_QUADS);
    glVertex2i(WinW - 50, WinH - 2 * (WinH / 7) - 20);
    glVertex2i(WinW - 50, WinH - 3 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 3 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 2 * (WinH / 7) - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(WinW - 50, WinH - 2 * (WinH / 7) - 20);
    glVertex2i(WinW - 50, WinH - 3 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 3 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 2 * (WinH / 7) - 20);
    glEnd();

    if (Button_Flag != 10) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "-R";
    glRasterPos2i(WinW - WinW / 8, 0.63 * WinH);
    for (int i = 0; i < name.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton11() {
    if (!New_Way.size())
        return;

    if (Button_Flag != 11) { glColor3f(1.0f, 1.0f, 1.0f); }
    else { glColor3f(0.0f, 0.0f, 0.0f); }
    glBegin(GL_QUADS);
    glVertex2i(WinW - 50, WinH - 3 * (WinH / 7) - 20);
    glVertex2i(WinW - 50, WinH - 4 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 4 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 3 * (WinH / 7) - 20);
    glEnd();


    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(WinW - 50, WinH - 3 * (WinH / 7) - 20);
    glVertex2i(WinW - 50, WinH - 4 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 4 * (WinH / 7));
    glVertex2i(WinW - WinW / 7, WinH - 3 * (WinH / 7) - 20);
    glEnd();

    if (Button_Flag != 11) { glColor3f(0.0f, 0.0f, 0.0f); }
    else { glColor3f(1.0f, 1.0f, 1.0f); }
    string name = "Draw path";
    glRasterPos2i(WinW - WinW / 8, 0.48 * WinH);
    for (int i = 0; i < name.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawCircle(int x, int y, int R, bool aim) {
    if(aim) glColor3f(0.0f, 0.0f, 0.0f);
    else glColor3f(1.0f, 1.0f, 1.0f);
    float x1, y1;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(360);
        x1 = R * sin(theta) + x;
        y1 = R * cos(theta) + y;
        glVertex2f(x1, y1);
    }
    glEnd();

    if (aim) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3f(0.0f, 0.0f, 0.0f);
    float x2, y2;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(360);
        x2 = R * sin(theta) + x;
        y2 = R * cos(theta) + y;
        glVertex2f(x2, y2);
    }
    glEnd();
}

void drawText(int text, int x1, int y1) {
    GLvoid* font = GLUT_BITMAP_HELVETICA_18;
    string s = to_string(text);
    glRasterPos2i(x1 - 5, y1 - 5);
    for (size_t j = 0; j < s.length(); j++)
        glutBitmapCharacter(font, s[j]);
}

void drawLine(int text, int x0, int y0, int x1, int y1, int start, int end) {
    if(Mouse_Vert == start || Mouse_Vert == end) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3i(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2i(x0, y0);
    glVertex2i(x1, y1);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);

    if (graph.getAdjMatrixElem(end, start) != 0 && end > start) { drawText(text, (x0 + x1) / 2 - 10, (y0 + y1) / 2 - 10);  }
    else { drawText(text, (x0 + x1) / 2 + 10, (y0 + y1) / 2 + 10); }

    float vx = x0 - x1;
    float vy = y0 - y1;
    float s = 1.0f / sqrt(vx * vx + vy * vy);
    vx *= s;
    vy *= s;
    x1 = x1 + R * vx;
    y1 = y1 + R * vy;

    if (Mouse_Vert == start || Mouse_Vert == end) glColor3f(1.0f, 0.0f, 0.0f);
    else glColor3i(0, 0, 0);

    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x1 + 10 * (vx + vy), y1 + 10 * (vy - vx));
    glVertex2f(x1 + 10 * (vx - vy), y1 + 10 * (vy + vx));
    glEnd();
}

void drawVertex(int n) {
    for (int i = 0; i < n; i++) {
        if (i == Mouse_Vert) {
            drawCircle(vertC[i].x, vertC[i].y, R, true);
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else {
            drawCircle(vertC[i].x, vertC[i].y, R, false);
            glColor3f(0.0, 0.0, 0.0);
        }
        drawText(i+1, vertC[i].x, vertC[i].y);
    }
}

void drawPath(int n) {
    if (New_Way.empty()) {
        cerr << "Сначала решите задачу Коммивояжера" << endl;
        isDrawingPath = false;
        return;
    }

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < graph.getAmountVerts(); i++) {
        drawCircle(vertC[i].x, vertC[i].y, R, false);
        drawText(i + 1, vertC[i].x, vertC[i].y);
    }

    glutSwapBuffers();
    Sleep(1000);
    glutSwapBuffers();

    for (int i = 0; i < graph.getAmountVerts(); i++) {
        for (int j = 0; j <= i; j++) {
            int n1 = New_Way[j]; n1--;
            int n2 = New_Way[j + 1]; n2--;
            drawLine(graph.getAdjMatrixElem(n1, n2), vertC[n1].x, vertC[n1].y, vertC[n2].x, vertC[n2].y, n1, n2);
        }
        for (int i = 0; i < graph.getAmountVerts(); i++) {
            drawCircle(vertC[i].x, vertC[i].y, R, false);
            drawText(i + 1, vertC[i].x, vertC[i].y);
        }

        // Очередной хак
        glutSwapBuffers();
        Sleep(1000);
        glutSwapBuffers();
    }

    Sleep(2000);
    isDrawingPath = false;
}

void setCoords(int i, int n) {
    int R_;
    int x0 = WinW / 2;
    int y0 = WinH / 2;
    if (WinW > WinH) {
        R = 5 * (WinH / 13) / n;
        R_ = WinH / 2 - R - 10;
    }
    else {
        R = 5 * (WinW / 13) / n;
        R_ = WinW / 2 - R - 10;
    }
    float theta = 2.0f * 3.1415926f * i / n;
    int x1 = R_ * sin(theta) + x0;
    int y1 = R_ * cos(theta) + y0;
    vertC[i].x = x1;
    vertC[i].y = y1;
}

void makeGraph() {
    standView = false;
    int amountVerts, amountEdges, sourceVertex, targetVertex, edgeWeigth;
    cout << "Для автозаполнения варианта (20) введите -1\nВведите количество вершин в графе: "; cin >> amountVerts;
    if (amountVerts == -1) {
        amountVerts = 6;
        amountEdges = 9;

        for (int i = 1; i <= amountVerts; i++)
            graph.insertVertex(i);

        graph.insertEdge(1, 2, 14);
        graph.insertEdge(2, 5, 42);
        graph.insertEdge(2, 6, 23);
        graph.insertEdge(5, 3, 18);
        graph.insertEdge(3, 1, 19);
        graph.insertEdge(3, 4, 9);
        graph.insertEdge(4, 6, 31);
        graph.insertEdge(6, 2, 23);
        graph.insertEdge(6, 1, 28);
    } else if (amountVerts > 0) {
        for (int i = 1; i <= amountVerts; i++)
            graph.insertVertex(i);
        
        cout << "Введите количество ребер в графе: "; cin >> amountEdges;
        if (amountEdges) {
            for (int i = 0; i < amountEdges; i++) {
                cout << "Введите начальную вершину: "; cin >> sourceVertex;
                cout << "Введите конечную вершину: "; cin >> targetVertex;
                cout << "Введите вес ребра: "; cin >> edgeWeigth;
                graph.insertEdge(sourceVertex, targetVertex, edgeWeigth);
            }
        }
    }
    cout << endl;
    graph.print();
}

bool checkSalesman(int** matrix) {
    if (graph.isEmpty())
        return false;

    for (int i = 0; i < graph.getAmountVerts(); i++)
        for (int j = 0; i < graph.getAmountVerts(); j++)
            if (matrix[i][j] > 0)
                return true;

    return false;
}

int checkCircle(int x, int y) {
    for (int i = 0; i < graph.getAmountVerts(); i++)
        if (pow(x - vertC[i].x, 2) + pow(y - vertC[i].y, 2) <= pow(R, 2))
            return i;

    return -1;
}

void checkButton(int x, int y) {
    // Ширина кнопки (начало / конец)
    if (x > 50 && x < WinW / 7) {
        // Высота кнопки (начало / конец)
        if(y < (WinH - 20) && y > (WinH - WinH / 7)) 
            Button_Flag = 1;
        else if(y > WinH - 2 * (WinH / 7) && y < (WinH - WinH / 7 - 20))
            Button_Flag = 2;
        else if(y > WinH - 3 * (WinH / 7) && y < WinH - 2 * (WinH / 7) - 20)
            Button_Flag = 3;
        else if(y > WinH - 4 * (WinH / 7) && y < WinH - 3 * (WinH / 7) - 20)
            Button_Flag = 4;
        else if(y > WinH - 5 * (WinH / 7) && y < WinH - 4 * (WinH / 7) - 20)
            Button_Flag = 5;
        else if(y > WinH - 6 * (WinH / 7) && y < WinH - 5 * (WinH / 7) - 20)
            Button_Flag = 6;
        else if(y > 10 && y < WinH - 6 * (WinH / 7) - 20)
            Button_Flag = 7;
        else
            Button_Flag = 0; // Ничего нет
    }
    else if (x > WinW - WinW / 7 && x < WinW - 50) {
        // Высота кнопки (начало / конец)
        if (y < (WinH - 20) && y >(WinH - WinH / 7))
            Button_Flag = 8;
        else if (y > WinH - 2 * (WinH / 7) && y < (WinH - WinH / 7 - 20))
            Button_Flag = 9;
        else if (y > WinH - 3 * (WinH / 7) && y < WinH - 2 * (WinH / 7) - 20)
            Button_Flag = 10;
        else if (y > WinH - 4 * (WinH / 7) && y < WinH - 3 * (WinH / 7) - 20)
            Button_Flag = 11;
        else
            Button_Flag = 0; // Ничего нет
    } else
        Button_Flag = 0; // Ничего нет
}

void mouseMove(int x, int y) {
    y = WinH - y;
    CursorX = x;
    CursorY = y;

    if (isDrawingPath)
        return;

    if (isCutting)
        return;

    Mouse_Vert = checkCircle(x, y);
    if (isVertMove) {
        // Хак для задержки, когда мы быстро перемещаем кружок
        Mouse_Vert = Moving_Vertex;
        vertC[Moving_Vertex].x = CursorX;
        vertC[Moving_Vertex].y = CursorY;


        // Рамки по X
        /*
        * БЕЗ УЧЕТА КНОПОК
        * 
        if (CursorX + R > WinW) { vertC[Moving_Vertex].x = WinW - R; }
        if (CursorX < R) { vertC[Moving_Vertex].x = R; }
        */

        /*
        * С учетом кнопок
        */
        if (CursorX + R > WinW - WinW / 7) { vertC[Moving_Vertex].x = WinW - WinW / 7 - R; }
        if (CursorX < R + WinW / 7) { vertC[Moving_Vertex].x = WinW / 7 + R; }

        if (CursorY + R > WinH) { vertC[Moving_Vertex].y = WinH - R; }
        if (CursorY < R) { vertC[Moving_Vertex].y = R; }
    }
    checkButton(x, y);
    glutPostRedisplay();
}

void drawInformation() {
    string r;
    glColor3f(0.0f, 0.0f, 0.0f);
    if (Button_Flag) {
        switch (Button_Flag)
        {
        case 1:
            r = "Create new graph (Console)";
            break;
        case 2:
            r = "Salesman task (Console)";
            break;
        case 3:
            r = "Displays the current graph as a matrix";
            break;
        case 4:
            r = "Adds a new vertex";
            break;
        case 5:
            r = "Removes the largest vertex";
            break;
        case 6:
            r = "Adds a edge between vertices";
            break;
        case 7:
            r = "Removes edge between vertices";
            break;
        case 8:
            r = "Resets the view";
            break;
        case 9:
            r = "Increase radius | RAD = " + to_string(R);
            break;
        case 10:
            r = "Decrease radius | RAD = " + to_string(R);
            break;
        case 11:
            r = "Draws a traveling salesman path";
            break;
        default:
            break;
        }
    } else {
        if (graph.getAmountVerts() != 0) r = "RAD = " + to_string(R);
        else r = "Use menu for create graph";
    }
    glRasterPos2i(WinW / 1.5, 0.061 * WinH);
    for (int i = 0; i < r.length(); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, r[i]);
}

void drawCreateEdge() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2i(vertC[CreatingEdgeId].x, vertC[CreatingEdgeId].y);
    glVertex2i(CursorX, CursorY);
    glEnd();
}

void drawCutting() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2i(CuttingPos.x, CuttingPos.y);
    glVertex2i(CursorX, CursorY);
    glEnd();
}

void mouseClick(int button, int state, int x, int y) {
    if (isDrawingPath)
        return;

    if (Button_Flag > 0) {
        switch (Button_Flag)
        {
            case 1:
                // Make new graph
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    New_Way.clear();
                    Way.clear();
                    Graph New;
                    graph = New;
                    makeGraph();
                    return;
                }
                break;
            case 2:
                // Salesman task
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    New_Way.clear();
                    Way.clear();
                    cout << "\nЗадача Коммивояжера:\n";
                    int** matrix = Change_Matrix();
                    bool checker = checkSalesman(matrix);
                    if (!checker) {
                        cout << "\nЗадача Коммивояжера для данного графа некорректна\n\n";
                        return;
                    }   
                    int n = graph.getAmountVerts();
                    while (Way.size() < n) {
                        matrix = highZero(matrix);
                    }
                    cout << endl;
                    printResult();
                    return;
                }
                break;
            case 3:
                // Print matrix in console
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    graph.print();
                    return;
                }
                break;
            case 4:
                // Add vertex
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    New_Way.clear();
                    int cur = graph.getAmountVerts();
                    graph.insertVertex(cur + 1);
                    vertC[cur].x = WinW / 2;
                    vertC[cur].y = WinH / 2;
                    return;
                }
                break;
            case 5:
                // Delete vertex
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    // standView = false;
                    graph.eraseLastVert();
                    return;
                }
                break;
            case 6:
                // Add edge
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    int sourceVertex, targetVertex, edgeWeigth;
                    cout << "Введите начальную вершину: "; cin >> sourceVertex;
                    cout << "Введите конечную вершину: "; cin >> targetVertex;
                    cout << "Введите вес ребра: "; cin >> edgeWeigth;
                    graph.insertEdge(sourceVertex, targetVertex, edgeWeigth);
                    return;
                }
                break;
            case 7:
                // Delete edge
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    int sourceVertex, targetVertex;
                    cout << "Введите начальную вершину: "; cin >> sourceVertex;
                    cout << "Введите конечную вершину: "; cin >> targetVertex;
                    graph.eraseEdge(sourceVertex, targetVertex);
                    return;
                }
                break;
            case 8:
                // Circular view
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    standView = false;
                    return;
                }
                break;
            case 9:
                // +R
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    if (R != 300) {
                        standView = true;
                        R++;
                        graph.alignmentVerts();
                    }
                    return;
                }
                if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
                    if (R <= 290) {
                        standView = true;
                        R += 10;
                        graph.alignmentVerts();
                    }
                    return;
                }
                break;
            case 10:
                // -R
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    if (R != 15) {
                        standView = true;
                        R--;
                        graph.alignmentVerts();
                    }
                    return;
                }
                if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
                    if (R >= 25) {
                        standView = true;
                        R -= 10;
                        graph.alignmentVerts();
                    }
                    return;
                }
                break;
            case 11:
                // Draw path
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    isDrawingPath = true;
                    drawPath(1);
                }
                break;
            default:
                break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            int j = checkCircle(x, WinH - y);
            if (j != -1) {
                if (isCreateEdge) {
                    // Если уже идет создание
                    if (CreatingEdgeId != j) {
                        // Если это не такая же вершина
                        int weight;
                        cout << "Введите вес ребра: "; cin >> weight;
                        graph.insertEdge(CreatingEdgeId + 1, j + 1, weight);
                    }
                } else {
                    CreatingEdgeId = j;
                    isCreateEdge = true;
                    return;
                }
            }
            CreatingEdgeId = -1;
            isCreateEdge = false;
        }

        if (state == GLUT_UP) {
            int j = checkCircle(x, WinH - y);
            if (j != -1) {
                if (isCreateEdge) {
                    // Если уже идет создание
                    if (CreatingEdgeId != j) {
                        // Если это не такая же вершина
                        int weight;
                        cout << "Введите вес ребра: "; cin >> weight;
                        graph.insertEdge(CreatingEdgeId + 1, j + 1, weight);
                    }
                }
            }
            CreatingEdgeId = -1;
            isCreateEdge = false;
        }
    }

    if (button == GLUT_LEFT_BUTTON) {
        CreatingEdgeId = -1;
        isCreateEdge = false;

        if (state == GLUT_DOWN) {
            int j = checkCircle(x, WinH - y);
            if (j != -1) {
                standView = true;
                isVertMove = true;
                Moving_Vertex = j;
                return;
            } else {
                isCutting = true;
                CuttingPos.x = x;
                CuttingPos.y = WinH - y;
            }
        } else {
            if (isVertMove) isVertMove = false;
            if (isCutting) {
                graph.cuttingFinish();
                isCutting = false;
            }
        }
    }
}

/*
    Проверить на пересечение два отрезка
*/
bool intersect(vertCoord a1, vertCoord a2, vertCoord b1, vertCoord b2) {
    float v1 = (b2.x - b1.x) * (a1.y - b1.y) - (b2.y - b1.y) * (a1.x - b1.x);
    float v2 = (b2.x - b1.x) * (a2.y - b1.y) - (b2.y - b1.y) * (a2.x - b1.x);
    float v3 = (a2.x - a1.x) * (b1.y - a1.y) - (a2.y - a1.y) * (b1.x - a1.x);
    float v4 = (a2.x - a1.x) * (b2.y - a1.y) - (a2.y - a1.y) * (b2.x - a1.x);
    return (v1 * v2 < 0) && (v3 * v4 < 0);
}

void reshape(int w, int h) {
    WinW = w;
    WinH = h;
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, (GLsizei)WinW, (GLsizei)WinH);
    gluOrtho2D(0, (GLdouble)WinW, 0, (GLdouble)WinH);
    glLoadIdentity();
    graph.alignmentVerts();
    glutPostRedisplay();
}

void display() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WinW, 0, WinH);
    glViewport(0, 0, WinW, WinH);
    glClear(GL_COLOR_BUFFER_BIT);

    if(isSplashScreen) {
        for (int i = 0, progress = 0, percent_int = 0; i <= 20; i++) {
                                                                                  // Окно загрузки
            progress = i * 10;
            glutSwapBuffers();
            glClear(GL_COLOR_BUFFER_BIT);

            glColor3f(0.0f, 0.0f, 0.0f);
            string s = "Graph - kommivoyajer";
            glRasterPos2i(50, WinH / 2);
            for (size_t j = 0; j < s.length(); j++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[j]);

            glColor3f(0.0f, 0.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2i(50, WinH - 6 * (WinH / 7) - 20);
            glVertex2i(50, 10);
            glVertex2i(WinW - 50, 10);
            glVertex2i(WinW - 50, WinH - 6 * (WinH / 7) - 20);
            glEnd();

            if (progress != 0) {
                percent_int = (WinW - 50) / 100 * progress;

                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_QUADS);
                glVertex2i(50, WinH - 6 * (WinH / 7) - 20);
                glVertex2i(50, 10);
                glVertex2i(percent_int, 10);
                glVertex2i(percent_int, WinH - 6 * (WinH / 7) - 20);
                glEnd();
            }

            if(progress > 50) glColor3f(1.0f, 1.0f, 1.0f);
            else glColor3f(0.0f, 0.0f, 0.0f);
            s = to_string(progress) + "%";
            glRasterPos2i(WinW / 2, (WinH - 6 * (WinH / 7) - 30) / 2);
            for (size_t j = 0; j < s.length(); j++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[j]);

            glutSwapBuffers();

            // time_t t = time(nullptr);
            // while (t + (100 / (i + 10)) > time(nullptr)) {}
            Sleep(500 / (i + 1));
        }

        isSplashScreen = false;
        return;
    }

    // Отрисовка графики
    if (!isDrawingPath) {
        drawButton1();
        drawButton2();
        drawButton3();
        drawButton4();
        drawButton5();
        drawButton6();
        drawButton7();
        drawButton11();
        drawInformation();

        if (isCreateEdge) {
            drawCreateEdge();
        }
        if (isCutting) {
            drawCutting();
        }

        graph.drawGraph();
    } else drawPath(graph.getAmountVerts());
    glutSwapBuffers();
}