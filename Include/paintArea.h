#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <iostream>
#include <vector>
#include <QColor>
#include <QColorDialog>
#include <QBrush>

using namespace std;

class PaintArea : public QWidget
{
  Q_OBJECT

  public:
    PaintArea(QWidget *parent = 0);
    QPixmap getBuffer();
    void setBuffer(QString);
    void resetBuffer();
    void _brushChooseColor();
  public slots:
    void setCurrentTool(int);
    void changeColor(int);
    void changeColoration(QColor);
    void changeWidth(int);
    void changeLine(int);
    void changePattern(int);
  signals:
    void popUpAsked(QPoint value);
  protected :
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void paintEvent(QPaintEvent*);
    void contextMenuEvent(QContextMenuEvent *evt);
  private :
   QPoint  _startPoint,_endPoint, _beginPoint;
   QColor _currentQColor, _currentBrushColor;
   Qt::BrushStyle _currentPattern;
   QPixmap *_buffer;
   int _currentTool, _currentColor, _currentWidth, _currentLine;
   //vector<QPoint> _points;
   QPolygon polygon;
   
   bool _release, _releaseDoubleClic, _enter, _esc;
};
#endif

