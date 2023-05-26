#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <QColorDialog>
#include <iostream>
#include <algorithm>

using namespace std;

Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    im=QImage(1000, 1000, QImage::Format_RGB32);
    im.fill(0);
    kolor=QColor::fromRgb(255,255,255);
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,im);
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    if(tryb==1&&id!=-1)
    {
        p2.x=e->pos().x();
        p2.y=e->pos().y();
        punkty[id].x=p2.x;
        punkty[id].y=p2.y;
        if(punkty.size()>=3)
        {
            scanLineDraw();
        }
    }
}

void Ekran::mousePressEvent(QMouseEvent *e)
{

    if(e->button()==Qt::LeftButton)
    {
        tryb=1;
        p1.x=e->pos().x();
        p1.y=e->pos().y();
        id=-1;
        for(int i=0;i<punkty.size();i++)
        {
            if(sqrt(pow((punkty[i].x-p1.x),2)+pow((punkty[i].y-p1.y),2))<=8)
            {
                id=i;
                break;
            }
        }

    }
    else if(e->button()==Qt::RightButton)
    {
        tryb=0;
        p1.x=e->pos().x();
        p1.y=e->pos().y();
        drawPunkt(p1.x,p1.y);
        punkty.push_back({p1.x,p1.y});
        if(punkty.size()>=3)
        {
            scanLineDraw();
        }

    }
}

void Ekran::drawPixel(int x,int y)
{
    if((x<im.width()&&x>=0)&&(y<im.height()&&y>=0))
    {
        uchar *pix=im.scanLine(y)+4*x;
        pix[0]=colorDraw.blue;
        pix[1]=colorDraw.green;
        pix[2]=colorDraw.red;
    }
}

void Ekran::drawPunkt(int x,int y)
{
    color rgb;
    rgb.red=colorDraw.red;
    rgb.green=colorDraw.green;
    rgb.blue=colorDraw.blue;
    colorDraw.red-=100;
    colorDraw.green-=100;
    colorDraw.blue-=100;
    for(int i=y-2;i<=y+2;i++)
    {
        for(int j=x-2;j<=x+2;j++)
        {
          drawPixel(j,i);
        }
    }
    colorDraw.red=rgb.red;
    colorDraw.green=rgb.green;
    colorDraw.blue=rgb.blue;
    update();
}

void Ekran::linia()
{
    if(abs(p2.y-p1.y)<=abs(p2.x-p1.x))
    {
            float y=p1.y;
            float a=(float)(p2.y-p1.y)/(p2.x-p1.x);
            if(p1.x<p2.x)
            {
                for(int i=p1.x;i<=p2.x;i++)
                {
                    drawPixel(i+0.5,y+0.5);
                    y+=a;
                }
            }
            else
            {
                for(int i=p1.x;i>=p2.x;i--)
                {
                    drawPixel(i+0.5,y+0.5);
                    y-=a;
                }
            }
        }
        else
        {
            float x=p1.x;
            float a=(float)(p1.x-p2.x)/(p1.y-p2.y);
            if(p1.y<p2.y)
            {
                for(int i=p1.y;i<=p2.y;i++)
                {
                    drawPixel(x+0.5,i+0.5);
                    x+=a;
                }
            }
            else
            {
                for(int i=p1.y;i>=p2.y;i--)
                {
                    drawPixel(x+0.5,i+0.5);
                    x-=a;
                }
            }
        }
    update();
}

void Ekran::colorChange()
{
    colorDraw.red=kolor.red();
    colorDraw.green=kolor.green();
    colorDraw.blue=kolor.blue();
}

void Ekran::setColor(QColor k)
{
    kolor=k;
}

QColor Ekran::getColor()
{
    return kolor;
}

void Ekran::scanLineDraw()
{
    int ymax=punkty[0].y,ymin=punkty[0].y;
    int a1,a2,b;
    im.fill(0);
    QVector<wsp> wspl;
    QVector<int> tabX;
    for(coord i:punkty)
    {
        if(i.y>ymax)
        {
            ymax=i.y;
        }
        if(i.y<ymin)
        {
            ymin=i.y;
        }
    }
    for(int i=1;i<punkty.size();i++)
    {
        a1=punkty[i].y-punkty[i-1].y;
        a2=punkty[i].x-punkty[i-1].x;
        b=punkty[i].y*a2-punkty[i].x*a1;
        wspl.push_back({a1,a2,b});
        drawPunkt(punkty[i].x,punkty[i].y);
    }
    a1=punkty[0].y-punkty[punkty.size()-1].y;
    a2=punkty[0].x-punkty[punkty.size()-1].x;
    b=punkty[0].y*a2-punkty[0].x*a1;
    wspl.push_back({a1,a2,b});
    drawPunkt(punkty[0].x,punkty[0].y);
    for(int i=ymin;i<=ymax;i++)
    {
        tabX.clear();
        for(int j=1;j<punkty.size();j++)
        {
            check(tabX,wspl,j-1,j,i);
        }
        check(tabX,wspl,punkty.size()-1,0,i);
        sort(tabX.begin(),tabX.end());
        p1.y=i;
        p2.y=i;
        for(int j=1;j<tabX.size();j+=2)
        {
            p1.x=tabX[j-1];
            p2.x=tabX[j];
            linia();
        }
    }
}

void Ekran::maxMin(int &l1,int &l2)
{
    int pom;
    if(l1<l2)
    {
       pom=l1;
       l1=l2;
       l2=pom;
    }
}


void Ekran::check(QVector<int> &tabX,QVector <wsp> wspl,int index1,int index2,int i)
{
    int x;
    int point1,point2;
    point1=punkty[index1].y;
    point2=punkty[index2].y;
    maxMin(point1,point2);
    if(i<point1&&i>=point2)
    {
        if(wspl[index1].a1!=0)
        {
            x=(i*wspl[index1].a2-wspl[index1].b)/(float)wspl[index1].a1;
            point1=punkty[index1].x;
            point2=punkty[index2].x;
            maxMin(point1,point2);
            if(x<=point1&&x>=point2)
            {
                tabX.push_back(x);
            }
        }
    }
}
