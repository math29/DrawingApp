#include "enums.h"
#include "paintArea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent) {
  qDebug() << "PaintArea::PaintArea(void)";
  _startPoint = _endPoint = QPoint(-10,-10);
  this->setFocusPolicy(Qt::StrongFocus);
  _buffer = new QPixmap(parent->size());
  _buffer->fill(Qt::white);
  _release=false;
  _releaseDoubleClic = false;
  _enter = false;
  _esc = false;
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

void PaintArea::keyPressEvent(QKeyEvent* evt) {
    qDebug() << "PaintArea::keyPressEvent(void)";
    if (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return) _enter = true;
    if (evt->key() == Qt::Key_Escape) _esc = true;
    update();
}

void PaintArea::keyReleaseEvent(QKeyEvent* evt) {
    qDebug() << "PaintArea::keyReleaseEvent(void)";
    if (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return) _enter = false;
    if (evt->key() == Qt::Key_Escape) _esc = false;
    update();
}

void PaintArea::paintEvent(QPaintEvent* evt) 
{
  QPainter paintWindow(this);
  QPainter paintBuffer(_buffer);
  paintWindow.drawPixmap(0,0, *_buffer);
  //qDebug() << _currentColor;
  switch(_currentColor) {
    case COLOR_BLUE :
      paintWindow.setPen(QColor("blue"));
      paintBuffer.setPen(QColor("blue"));
      break;
    case COLOR_BLACK :
      paintWindow.setPen(QColor("black"));
      paintBuffer.setPen(QColor("black"));
      break;
    case COLOR_RED :
      paintWindow.setPen(QColor("red"));
      paintBuffer.setPen(QColor("red"));
      break;
    case COLOR_YELLOW :
      paintWindow.setPen(QColor("yellow"));
      paintBuffer.setPen(QColor("yellow"));
      break;
    case COLOR_GREEN :
      paintWindow.setPen(QColor("green"));
      paintBuffer.setPen(QColor("green"));
      break;
    case COLOR_OTHER :
      paintWindow.setPen(_currentQColor);
      paintBuffer.setPen(_currentQColor);
      break;
  }
  //qDebug() << "entre deux";
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
        if (_release) polygon << _startPoint;
        polygon.setPoint(polygon.size()-1, _endPoint);
        paintWindow.drawPolyline(polygon);

        if (_enter) {
            paintBuffer.drawPolygon(polygon);
            polygon.clear();
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
  _startPoint = _endPoint = QPoint(-10,-10);
  _buffer = new QPixmap(size);
  _buffer->fill(Qt::white);
  update();
}

void PaintArea::changeColor(int color) {
  _currentColor = color;
  if(color == COLOR_OTHER) {
    qDebug() << "ici on lance la coloration";
    changeColoration(QColorDialog::getColor(Qt::white, this));
  }
}

void PaintArea::changeColoration(QColor color) {
  _currentQColor = color;
}

void PaintArea::changeWidth(int width) {
  _currentWidth = width;
  qDebug() << "On change la largeur de trait";
}
