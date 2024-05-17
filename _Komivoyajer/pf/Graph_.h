#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <Windows.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

const int maxSize = 20;                                                             // ������������ ���������� ������
extern int WinW;                                                                    // ������ ����
extern int WinH;                                                                    // ������ ����

// ��������� ��������� ������
struct vertCoord {
    int x, y;
};

class Graph
{
private:
    vector<int> vertList;                                                           // ������ ������
    int adjMatrix[maxSize][maxSize];                                                // ������� ���������

public:
    Graph();                                                                        // �����������
    bool isEmpty();                                                                 // �������� �� �������
    bool isFull();                                                                  // �������� �� �������

    int getVertText(int i);                                                         // �������� �������� �������
    vector<int> getVertList();                                                      // �������� ������ ������
    int getAdjMatrixElem(int i, int j);                                             // �������� ������� ������� ���������
    int getAmountVerts();                                                           // �������� ���������� ������ �����
    int getVertPos(const int& vertex);                                              // �������� ������� ������� � ������� ������
    int getAmountEdges();                                                           // �������� ���������� ����� �����
    void setEdgeZero(int i, int j);                                                 // �������� ������� ������� (��� ������������)
    void insertVertex(const int& vertex);                                           // �������� ������� (� ����� ������� ������)
    void insertEdge(const int& vertex1, const int& vertex2, int weight);            // �������� ����� ����� weight ����� vertex1 � vertex2
    void print();                                                                   // ����� ������� ��������� � �������
    void eraseLastVert();                                                           // ������� ��������� ������� � ������� ������
    void eraseEdge(const int& vertex1, const int& vertex2);                         // ������� ����� ����� vertex1 � vertex2
    void drawGraph();                                                               // ���������� ���� � OpenGL
    void cuttingFinish();                                                           // ��������� ������
    void alignmentVerts();                                                          // ����������� �������
};

extern Graph graph;                                                                 // ��������� ������ ������ Graph

/*
// ������� ��� ������� ������ ������������
*/
void printMatrix(int** matrix);                                                     // ����� ������� �������
bool checkSalesman(int** matrix);                                                   // ��������: ����� �� ������ ������ ������������
int** Change_Matrix();                                                              // ��������� �������, ������� 0 �� -1
int* searchMinElem(int* line, int n);                                               // ����� ������������ �������� � ������ �������
int** reductMatrix(int** oldmatrix);                                                // �������� �������
int** highZero(int** oldmatrix);                                                    // ���� � ���������� �����������
void printResult();                                                                 // ����� ���������� ������� ������������

/*
// ��������� ������ OpenGL
*/
void drawButton1();                                                                 // ������ "Make new graph"
void drawButton2();                                                                 // ������ "Salesman Task"
void drawButton3();                                                                 // ������ "Print Matrix In Console"
void drawButton4();                                                                 // ������ "Add Vertex"
void drawButton5();                                                                 // ������ "Delete Vertex"
void drawButton6();                                                                 // ������ "Add Edge"
void drawButton7();                                                                 // ������ "Delete Edge"
void drawButton11();                                                                // ������ "Draw path"
void drawInformation();                                                             // �������� ������� � ���� OpenGL

/*
// ��������� ��������� ����� OpenGL
*/
void drawCircle(int x, int y, int R, bool aim);                                     // ��������� ����� (aim - �������� �� ������)
void drawText(int nom, int x1, int y1);                                             // ��������� ���� �����
void drawLine(int text, int x0, int y0, int x1, int y1, int start, int end);        // ��������� �����
void drawVertex(int n);                                                             // ��������� �������
void drawPath(int n);                                                               // ��������� ���� ������������
void drawCreateEdge();                                                              // ������ ����� ��� �����
void drawCutting();                                                                 // ������ ����� ��� "���������"
void makeGraph();                                                                   // �������� �����
void setCoords(int i, int n);                                                       // ��������� ��������� ��� ������ �����

/*
// ������� - ��������
*/
int checkCircle(int x, int y);                                                      // �������� ������� �� ������
void checkButton(int x, int y);                                                     // �������� ������� �� ������

/*
// ������� ������������ ��������� ����
*/
void mouseMove(int x, int y);                                                       // ������������ �������� �������
void mouseClick(int button, int state, int x, int y);                               // ������������ ������� ������ �� ����

/*
// �����������
*/
void reshape(int w, int h);                                                         // ��������� ������� ������
void display();                                                                     // �������� �����������

/*
// ���������������
*/
bool intersect(vertCoord a1, vertCoord a2, vertCoord b1, vertCoord b2);             // �������� �� ����������� ���� ��������

#endif // GRAPH_GRAPH_H
