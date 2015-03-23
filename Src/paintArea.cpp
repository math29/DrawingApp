#include "enums.h"
#include "paintArea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent) {
  _startPoint = _endPoint = QPoint(-10,-10);
  this->setFocusPolicy(Qt::StrongFocus);
  _buffer = new QPixmap(parent->size());
  _buffer->fill(Qt::white);
  _trigger = false;
  _release=false;
  _releaseDoubleClic = false;
  _enter = false;
  _esc = false;
  _key= "";
  _currentWidth = LITTLE_WIDTH;
  _currentLine = SOLID_LINE;
  _currentBrushColor = Qt::white;
  _currentPattern = Qt::SolidPattern;
  _currentFont = QFont();
}

void PaintArea::mousePressEvent(QMouseEvent* evt) {
  _trigger = true;
  _release = false;
  _releaseDoubleClic = false;
  _startPoint = _endPoint = evt->pos();
}

void PaintArea::mouseMoveEvent(QMouseEvent* evt) 
{
  _endPoint = evt->pos(); 
  update();
}
void PaintArea::mouseReleaseEvent(QMouseEvent* evt) 
{
  _release=true;
  update();

}

void PaintArea::mouseDoubleClickEvent (QMouseEvent* evt) {
  _releaseDoubleClic = true;
  update();
}

void PaintArea::contextMenuEvent(QContextMenuEvent *evt) {
  emit popUpAsked(evt->globalPos());
}

void PaintArea::keyPressEvent(QKeyEvent* evt) {
    _key = evt->text();
    if (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return) {
	_enter = true;
	_key = "\n";
    }
    if (evt->key() == Qt::Key_Escape) _esc = true;
    update();
}

void PaintArea::keyReleaseEvent(QKeyEvent* evt) {
    if (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return) _enter = false;
    if (evt->key() == Qt::Key_Escape) _esc = false;
    update();
}

void PaintArea::paintEvent(QPaintEvent* evt) 
{
  QPainter paintWindow(this);
  QPainter paintBuffer(_buffer);
  paintWindow.drawPixmap(0,0, *_buffer);
  QPen pen;
  QBrush brush;
  brush = QBrush(_currentBrushColor);
  brush.setStyle(_currentPattern);
  paintWindow.setFont(_currentFont);
  paintBuffer.setFont(_currentFont);

  switch(_currentColor) {
    case COLOR_BLUE :
      pen = QPen(QColor("blue"));
      break;
    case COLOR_BLACK :
      pen = QPen(QColor("black"));
      break;
    case COLOR_RED :
      pen = QPen(QColor("red"));
      break;
    case COLOR_YELLOW :
      pen = QPen(QColor("yellow"));
      break;
    case COLOR_GREEN :
      pen = QPen(QColor("green"));
      break;
    case COLOR_OTHER :
      pen = QPen(_currentQColor);
      break;
  }

  switch(_currentLine) {
    case SOLID_LINE :
      pen.setStyle(Qt::SolidLine);
      break;
    case DASH_LINE :
      pen.setStyle(Qt::DashLine);
      break;
    case DOT_LINE :
      pen.setStyle(Qt::DotLine);
      break;
    case DASH_DOT_LINE :
      pen.setStyle(Qt::DashDotLine);
      break;
    case DASH_DOT_DOT_LINE :
      pen.setStyle(Qt::DashDotDotLine);
      break;
  }

  pen.setWidth(_currentWidth);
  paintWindow.setPen(pen);
  paintBuffer.setPen(pen);
  paintWindow.setBrush(brush);
  paintBuffer.setBrush(brush);

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
        // Ajout d'un point
        if (_trigger) {
            polygon << _endPoint;
            _trigger = false; // Un seul point par mousePressEvent
        }

        // Déplacer le point nouvellement créé
        if (polygon.size() != 0)
            polygon.setPoint(polygon.size()-1, _endPoint);
        paintWindow.drawPolyline(polygon);
        
        // Terminer ou annuler le polygone
        if (_enter) {
            paintBuffer.drawPolygon(polygon);
            polygon.clear();
        } else if (_esc) {
            polygon.clear();
        }
      break;
    case TOOLS_ID_TEXT :
	// Styles zone de texte
	pen.setStyle(Qt::DotLine);
	pen.setWidth(1);
	paintWindow.setPen(pen);
	paintWindow.setBrush(QBrush());
	
	if (_trigger) { // Création zone de texte
	    paintBuffer.drawText(textWrapper, Qt::TextWordWrap, text);
	    text.clear();
	    _trigger = false;
	} else if (_esc) { // Annulation texte
	    text.clear();
	    _key.clear();
	    textWrapper = QRect();
	} else if (_release) { // Écriture du texte
	    text.append(_key);
	    _key.clear();
	    paintWindow.drawText(textWrapper, Qt::TextWordWrap, text);
	} else { // Dimensionnement zone de texte
	    textWrapper = QRect(_startPoint, _endPoint);
	}
	
	// Zone de texte
	paintWindow.drawRect(textWrapper);
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
  polygon.clear();
  text.clear();
  _buffer = new QPixmap(size);
  _buffer->fill(Qt::white);
  update();
}

void PaintArea::changeColor(int color) {
  _currentColor = color;
  if(color == COLOR_OTHER) {
    changeColoration(QColorDialog::getColor(Qt::white, this));
  }
}

void PaintArea::changeColoration(QColor color) {
  _currentQColor = color;
}

void PaintArea::changeWidth(int width) {
  switch(width) {
    case LITTLE_WIDTH :
      _currentWidth = 1;
      break;
    case MEDIUM_WIDTH :
      _currentWidth = 10;
      break;
    case HIGHT_WIDTH :
      _currentWidth = 20;
      break;
  }
}

void PaintArea::changeLine(int line) {
  _currentLine = line;
}

void PaintArea::_brushChooseColor(void) {
  _currentBrushColor = QColorDialog::getColor(Qt::white, this);
}

void PaintArea::_chooseFont(void) {
  _currentFont = QFontDialog::getFont(NULL, QFont(), this);
}

void PaintArea::changePattern(int pattern) {
  switch(pattern) {
    case SOLID_PATTERN :
      _currentPattern = Qt::SolidPattern;
      break;
    case DENSE1_PATTERN :
      _currentPattern = Qt::Dense1Pattern;
      break;
    case DENSE2_PATTERN :
      _currentPattern = Qt::Dense2Pattern;
      break;
    case DENSE3_PATTERN :
      _currentPattern = Qt::Dense3Pattern;
      break;
    case DENSE4_PATTERN :
      _currentPattern = Qt::Dense4Pattern;
      break;
    case DENSE5_PATTERN :
      _currentPattern = Qt::Dense5Pattern;
      break;
    case HOR_PATTERN :
      _currentPattern = Qt::HorPattern;
      break;
    case VER_PATTERN :
      _currentPattern = Qt::VerPattern;
      break;
    case CROSS_PATTERN :
      _currentPattern = Qt::CrossPattern;
      break;
  }
}
