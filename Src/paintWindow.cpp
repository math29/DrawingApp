#include "enums.h"
#include "paintWindow.h"
#include <ctime>
#include <QFileDialog>

//--------------------------------------------------------------------------------
PaintWindow::PaintWindow(QWidget *parent) : QMainWindow(parent) {
   qDebug() << "PaintWindow::PaintWindow(QWidget *parent)";
   qDebug() << "Date: " << QDate::currentDate();

   _createMenus();
   _createActions();
   _createToolBars();
   _connectActions();

  _area = new PaintArea(this);
   setCentralWidget(_area);

  _signalMapper = new QSignalMapper(this);
  _signalMapper->setMapping(_freehandAct, TOOLS_ID_FREEHAND);
  _signalMapper->setMapping(_lineAct, TOOLS_ID_LINE);
  _signalMapper->setMapping(_rectAct, TOOLS_ID_RECTANGLE);
  _signalMapper->setMapping(_polyAct, TOOLS_ID_POLYGON);

  _connectSignals();
}
//--------------------------------------------------------------------------------
void PaintWindow::_createMenus(void) {
 QMenuBar* menubar = menuBar();
 statusBar()->showMessage("Ready");
 _fileMenu = menubar->addMenu( tr("&File") );
 _toolMenu = menubar->addMenu("&tool");
 _helpMenu = menubar->addMenu( tr("&Help") );
}
//--------------------------------------------------------------------------------
void PaintWindow::_createToolBars(void) {
 _toolBar=addToolBar( tr("File") ); 
}
//--------------------------------------------------------------------------------
void PaintWindow::_createActions(void) {

  _newAct = new QAction(QIcon(":/Images/new.png"), tr("&New..."), this);
  _newAct->setShortcut(tr("Ctrl+N"));
  _newAct->setToolTip(tr("New file tooltip"));
  _newAct->setStatusTip(tr("New file status"));
  _newAct->setData(QVariant("_newAct data"));

  _openAct = new QAction(QIcon(":/Images/open.png"), tr("&Open..."), this);
  _openAct->setShortcut(tr("Ctrl+O"));
  _openAct->setToolTip(tr("Open File"));
  _openAct->setStatusTip(tr("Open File from your repository"));
  _openAct->setData(QVariant("_openAct data"));

  _saveAct = new QAction(QIcon(":/Images/save.png"), tr("&Save..."), this);
  _saveAct->setShortcut(tr("Ctrl+S"));
  _saveAct->setToolTip(tr("Saving Work Tooltip"));
  _saveAct->setStatusTip(tr("Saved a file"));
  _saveAct->setData(QVariant("_saveAct data"));

  _saveAsAct = new QAction(QIcon(":/Images/save_as.png"), tr("&Save As..."), this);
  _saveAsAct->setShortcut(tr("Ctrl+D"));
  _saveAsAct->setToolTip(tr("Saving as Work Tooltip"));
  _saveAsAct->setStatusTip(tr("Saved as a file"));
  _saveAsAct->setData(QVariant("_saveAsAct data"));

  _exitAct = new QAction(tr("&Exit..."), this);
  _exitAct->setShortcut(tr("Ctrl+X"));
  _exitAct->setToolTip(tr("Exit"));
  _exitAct->setStatusTip(tr("Exit"));
  _exitAct->setIcon(QIcon(":/Images/exit.png"));
  _exitAct->setData(QVariant("_quitAct data"));

  _aboutAct    = new QAction(tr("&About Us.."), this);

  _toolsQag = new QActionGroup( this );
  _freehandAct = new QAction(tr("&Freehand"),  this);
  _lineAct = new QAction(tr("&Line"), this);
  _rectAct = new QAction(tr("&Rectangle"), this);
  _polyAct = new QAction(tr("&Polygon"), this);
  _freehandAct->setCheckable(true);
  _lineAct->setCheckable(true);
  _rectAct->setCheckable(true);
  _polyAct->setCheckable(true);
}
//--------------------------------------------------------------------------------
void PaintWindow::_connectActions(void) {
    _fileMenu->addAction(_newAct);
    _fileMenu->addAction(_openAct);
    _fileMenu->addAction(_saveAct);
    _fileMenu->addAction(_saveAsAct);
    _fileMenu->addAction(_exitAct);

    _toolBar->addAction(_newAct);
    _toolBar->addAction(_openAct);
    _toolBar->addAction(_saveAct);
    _toolBar->addAction(_exitAct);

    _toolsQag->addAction(_freehandAct);
    _toolsQag->addAction(_lineAct);
    _toolsQag->addAction(_rectAct);
    _toolsQag->addAction(_polyAct);

    _toolMenu->addAction(_freehandAct);
    _toolMenu->addAction(_lineAct);
    _toolMenu->addAction(_rectAct);
    _toolMenu->addAction(_polyAct);

    _helpMenu->addAction(_aboutAct);
}

//--------------------------------------------------------------------------------
void PaintWindow::_connectSignals(void) {

    connect(_newAct, SIGNAL(triggered()), this,SLOT(_newFile( )) );
    connect(_openAct, SIGNAL(triggered()), this,SLOT(_openFile( )) );
    connect(_saveAct, SIGNAL(triggered()), this,SLOT(_saveFile( )) );
    connect(_saveAsAct, SIGNAL(triggered()), this,SLOT(_saveAsFile( )) );
    connect(_exitAct,SIGNAL(activated()), this, SLOT(quit()));

    connect(_freehandAct,SIGNAL(activated()),_signalMapper, SLOT(map()));
    connect(_lineAct,SIGNAL(activated()),_signalMapper, SLOT(map()));
    connect(_rectAct,SIGNAL(activated()),_signalMapper, SLOT(map()));
    connect(_polyAct,SIGNAL(activated()),_signalMapper, SLOT(map()));

    connect(_aboutAct, SIGNAL(triggered()),this, SLOT(_about()));

    connect(_signalMapper,SIGNAL(mapped(int)), this, SIGNAL(toolMapped(int)));
    connect(this, SIGNAL(toolMapped(int)), _area, SLOT(setCurrentTool(int)) );
}
//--------------------------------------------------------------------------------
void PaintWindow::_about(void) {
    QMessageBox::information( this,"About Us","Developpeur : Mathieu ALLAIN \n Email : maaath29@gmail.com","Au boulot !");
}
//--------------------------------------------------------------------------------
void PaintWindow::_newFile(void)  {
  msgBox.setText("Si vous continuez vous allez perdre toutes les modifications non sauvegardees.");
  msgBox.setInformativeText("Voulez vous sauvegarder?");
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  int ret = msgBox.exec();

  switch (ret) {
    case QMessageBox::Save:
        // Save was clicked
        _area->update();
        _saveFile();
        _area->update();
        qDebug() << "Switch : NewFile : On a fini la sauvegarde !";
    case QMessageBox::Discard:
        // Don't Save was clicked
        _area->resetBuffer();
        _fileName = "";
        qDebug() << "Switch : NewFile : On a fini le reset extc ... !";
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
  }
    qDebug() << "PaintWindow::_newFile(void)";
}

void PaintWindow::_openFile(void) {
    _fileName = QFileDialog::getOpenFileName (this, tr("Choose a File"), "./Saves", tr("Images (*.png *.xpm *.jpg)"));
    _area->setBuffer(_fileName);
    qDebug() << "PaintWindow::_openFile(void)";
}

void PaintWindow::_saveFile(void)  {
    if(_fileName != NULL) {
        _area->getBuffer().save(_fileName, "PNG");
    }else {
        _fileName = QFileDialog::getSaveFileName (this, tr("Save File"), "./Saves/Sauvegarde.png", tr("Images (*.png *.xpm *.jpg)"));
        _area->getBuffer().save(_fileName, "PNG");
    }
    qDebug() << "PaintWindow::_saveFile(void)";
}

void PaintWindow::_saveAsFile(void)  {
    _fileName = QFileDialog::getSaveFileName (this, tr("Save File"), "./Saves/Sauvegarde.png", tr("Images (*.png *.xpm *.jpg)"));
    _area->getBuffer().save(_fileName, "PNG");
    qDebug() << "PaintWindow::_saveAsFile(void)";
}

void PaintWindow::quit(void)  {
  msgBox.setText("Etes vous sur de vouloir quitter ?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();

  switch (ret) {
    case QMessageBox::Yes:
        exit(0);
        break;
    case QMessageBox::No:
        break;
    default:
        break;
  }
}
