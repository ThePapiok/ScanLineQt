#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QVector>
#include <QColor>

struct color
{
    uchar red;
    uchar green;
    uchar blue;
};

struct coord
{
    int x;
    int y;
};

struct wsp
{
    int a1;
    int a2;
    int b;
};

class Ekran : public QWidget
{
    Q_OBJECT
public:
        explicit Ekran(QWidget *parent = nullptr);
        void linia();
        void colorChange();
        void setColor(QColor k);
        QColor getColor();
        void scanLineDraw();
        void drawPixel(int x,int y);
        void drawPunkt(int x,int y);
        void maxMin(int &l1,int &l2);
        void check(QVector<int> &tabX,QVector <wsp> wspl,int index1,int index2,int i);
protected:
        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);
private:
        QImage im;
        QVector <coord> punkty;
        coord p1,p2;
        int tryb=0;
        int id;
        color colorDraw={255,255,255};
        QColor kolor;
signals:

};

#endif // EKRAN_H
