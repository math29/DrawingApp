#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <iostream>
#include <vector>
#include <QColor>

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
    void changeColor(int);
    void changeColoration(QColor);
  signals:
    void popUpAsked(QPoint value);
  protected :
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent* evt);
    void paintEvent(QPaintEvent*);
    void contextMenuEvent(QContextMenuEvent *evt);
  private :
   QPoint  _startPoint,_endPoint, _beginPoint;
   QColor _currentQColor;
   vector<QPoint> _points;
   QPixmap *_buffer;
   int _currentTool, _currentColor;
   bool _release, _releaseDoubleClic;
};
#endif

