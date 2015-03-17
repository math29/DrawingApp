#include "enums.h"
#include "paintArea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent) {
  qDebug() << "PaintArea::PaintArea(void)";
  this->setFocusPolicy(Qt::StrongFocus);
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

void PaintArea::keyPressEvent(QKeyEvent* evt) {
    qDebug() << "PaintArea::keyPressEvent(void)";
    if (evt->key() == Qt::Key_Enter) _enter = true;
    if (evt->key() == Qt::Key_Escape) _esc = true;
    update();
}

void PaintArea::keyReleaseEvent(QKeyEvent* evt) {
    qDebug() << "PaintArea::keyReleaseEvent(void)";
    if (evt->key() == Qt::Key_Enter) _enter = false;
    if (evt->key() == Qt::Key_Escape) _esc = false;
    update();
}

void PaintArea::paintEvent(QPaintEvent* evt) 
{
  qDebug() << "PaintArea::paintEvent(void)";
  //qDebug() << _currentTool;
  QPainter paintWindow(this);
  QPainter paintBuffer(_buffer);
  paintWindow.drawPixmap(0,0, *_buffer);
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
//         if (_release) {
//             paintBuffer.drawLine(_startPoint,_endPoint);
//             polygon << _endPoint;
//             paintBuffer.drawPolyline(polygon);
//         } else if (_enter) {
//             paintWindow.drawPolygon(polygon);
//             polygon.clear();
//         }
         if (_release) {
            polygon << _endPoint;
            paintWindow.drawPolygon(polygon);
         }
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
  update();
}