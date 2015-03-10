#include <QDesktopWidget>
#include <QApplication>

#include "paintWindow.h"

void center(QWidget &widget)
{
  int x, y;
  int screenWidth;
  int screenHeight;

  int WIDTH = 350;
  int HEIGHT = 250;

  QDesktopWidget *desktop = QApplication::desktop();

  screenWidth = desktop->width();
  screenHeight = desktop->height();

  x = (screenWidth - WIDTH) / 2;
  y = (screenHeight - HEIGHT) / 2;

  widget.setGeometry(x, y, WIDTH, HEIGHT);
}


int main(int argc, char *argv[])
{
  QApplication app(argc, argv);  

  PaintWindow window;

  window.setWindowTitle("Application skeleton");
  window.show();
  center(window);

  return app.exec();
}

