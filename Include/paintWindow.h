#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QActionGroup>
#include <QIcon>
#include <QMessageBox>
#include <QScrollArea>
#include <QSignalMapper>

#include <QDebug>
#include <QDate>
#include <QMessageBox>

#include "paintArea.h"

class PaintWindow : public QMainWindow
{
  Q_OBJECT  
  public:
    PaintWindow(QWidget *parent = 0);
  signals:
   void toolMapped(int digit);
   void toolMappedColor(int digit);
   void toolMappedWidth(int digit);
  public slots :
    void quit(void);
    void showPopUp(QPoint value);
  private slots :
    void _newFile(void);
    void _openFile(void);
    void _saveFile(void);
    void _saveAsFile(void);
    void _about(void);
  private :
   void _createMenus(void);
   void _createToolBars(void);
   void _createActions(void);
   void _connectActions(void);
   void _connectSignals(void);

   // client
   PaintArea *_area;
   // menus
   QToolBar *_toolBar;
   QMenu *_fileMenu, *_toolMenu, *_styleMenu, *_helpMenu, *_colorSubMenu, *_popUpMenu, *_penMenu, *_widthSubMenu;
   // actions
   QAction *_newAct, *_saveAct, *_saveAsAct, *_openAct, *_exitAct, *_aboutAct;
   QActionGroup *_toolsQag, *_toolsColor, *_toolsWidth;
   QAction *_freehandAct, *_lineAct, *_rectAct, *_polyAct, *_circleAct, *_textAct;
   QAction *_otherColor, *_green, *_red, *_yellow, *_black, *_blue;
   QAction *_littleWidth, *_middleWidth, *_hightWidth;
   // mapping 
   QSignalMapper *_signalMapper, *_signalMapperColor, *_signalMapperWidth;
   //Dialog Box
   QMessageBox msgBox;
   //FileName
   QString _fileName;
};
#endif

