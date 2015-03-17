#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <iostream>
#include <vector>
using namespace std;

class PaintArea : public QWidget
{
  Q_OBJECT

  public:
    PaintArea(QWidget *parent = 0);
    QPixmap getBuffer();
    void setBuffer(QString);
    void resetBuffer();
  public slots:
    void setCurrentTool(int);
  protected :
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void paintEvent(QPaintEvent*);
  private :
   QPoint  _startPoint,_endPoint, _beginPoint;
   //vector<QPoint> _points;
   QPolygon polygon;
   
   QPixmap *_buffer;
   int _currentTool;
   bool _release, _releaseDoubleClic, _enter, _esc;
};
#endif

