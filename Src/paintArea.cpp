#include "enums.h"
#include "paintArea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent) {
  qDebug() << "PaintArea::PaintArea(void)";
  _startPoint = _endPoint = QPoint(0,0);
  _buffer = new QPixmap(parent->size());
  _buffer->fill(Qt::white);
  _release=false;
}

void PaintArea::mousePressEvent(QMouseEvent* evt) {
  qDebug() << "PaintArea::mousePressEvent(void)";
  _release = false;
  _releaseDoubleClic = false;
  _startPoint = _endPoint = evt->pos();
}

void PaintArea::mouseMoveEvent(QMouseEvent* evt) 
{
// qDebug() << "PaintArea::mouseMoveEvent(void)";
  _endPoint = evt->pos(); 
  update();
}
void PaintArea::mouseReleaseEvent(QMouseEvent* evt) 
{
  qDebug() << "PaintArea::mouseReleaseEvent(void)";
  _release=true;
  update();

}

void PaintArea::mouseDoubleClickEvent (QMouseEvent* evt) {
  qDebug() << "PaintArea::mouseDoubleClickEvent(void)";
  _releaseDoubleClic = true;
  update();
}

void PaintArea::contextMenuEvent(QContextMenuEvent *evt) {
  emit popUpAsked(evt->globalPos());
}

void PaintArea::paintEvent(QPaintEvent* evt) 
{
  qDebug() << "PaintArea::paintEvent(void)";
  //qDebug() << _currentTool;
  QPainter paintWindow(this);
  QPainter paintBuffer(_buffer);
  paintWindow.drawPixmap(0,0, *_buffer);
  qDebug() << _currentColor;
  switch(_currentColor) {
    case COLOR_BLUE :
      paintWindow.setPen(QColor("blue"));
      paintBuffer.setPen(QColor("blue"));
      qDebug() << "on met la couleur bleu"+_currentColor;
      break;
    case COLOR_BLACK :
      paintWindow.setPen(QColor("black"));
      paintBuffer.setPen(QColor("black"));
      qDebug() << "on met la couleur noir";
      break;
    case COLOR_RED :
      paintWindow.setPen(QColor("red"));
      paintBuffer.setPen(QColor("red"));
      qDebug() << "on met la couleur red";
      break;
    case COLOR_YELLOW :
      paintWindow.setPen(QColor("yellow"));
      paintBuffer.setPen(QColor("yellow"));
      qDebug() << "on met la couleur jaune";
      break;
    case COLOR_GREEN :
      paintWindow.setPen(QColor("green"));
      paintBuffer.setPen(QColor("green"));
      qDebug() << "on met la couleur verte";
      break;
  }

  switch(_currentTool) {
    case TOOLS_ID_FREEHAND :
      paintBuffer.drawPoint(_endPoint);
      paintWindow.drawPoint(_endPoint);
      break;
    case TOOLS_ID_LINE :
      if (_release) paintBuffer.drawLine(_startPoint,_endPoint);
      paintWindow.drawLine(_startPoint,_endPoint);
      break;
    case TOOLS_ID_RECTANGLE :
      if (_release) paintBuffer.drawRect(QRect(_startPoint,_endPoint));
      paintWindow.drawRect(QRect(_startPoint,_endPoint));
      break;
    case TOOLS_ID_CIRCLE :
      if (_release) paintBuffer.drawEllipse(QRect(_startPoint,_endPoint));
      paintWindow.drawEllipse(QRect(_startPoint,_endPoint));
      break;
    case TOOLS_ID_POLYGON :
      if(_releaseDoubleClic) {
        while(_points.size() > 0){
          _points.pop_back();
        }
        break;
      }
      if(_points.size() < 1) _points.push_back(_startPoint);
      _points.push_back(_endPoint);
      if (_release) paintBuffer.drawLine(_points[_points.size()-2], _points[_points.size()-1]);
      paintWindow.drawLine(_points[_points.size()-2], _endPoint);
      break;
    default :
      break;
  }
}

void PaintArea::setCurrentTool(int tool) {
  _currentTool = tool;
}

QPixmap PaintArea::getBuffer() {
  return *_buffer;
}

void PaintArea::setBuffer(QString fileName) {
  _buffer->load(fileName);
}

void PaintArea::resetBuffer() {
  QSize size = _buffer->size();
  _startPoint = _endPoint = QPoint(0,0);
  _buffer = new QPixmap(size);
  _buffer->fill(Qt::white);
  qDebug() << "On reset le buffer";
}

void PaintArea::changeColor(int color) {
  _currentColor = color;
}