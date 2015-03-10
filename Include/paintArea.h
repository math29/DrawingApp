#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

class PaintArea : public QWidget
{
  Q_OBJECT  

  public:
    PaintArea(QWidget *parent = 0);
  public slots:
    void setCurrentTool(int);
  protected :
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent* evt);
    void paintEvent(QPaintEvent*);
  private :
   QPoint  _startPoint,_endPoint;
   QPixmap *_buffer;
   int _currentTool;
   bool _release;
};
#endif

