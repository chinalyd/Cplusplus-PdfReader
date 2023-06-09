#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include<QMenu>
#include<QMenuBar>
#include<QAction>
#include<QMdiArea>

#include<QToolBar>
#include<QLineEdit>  //Text Edit support
#include<QLabel> //Label Support

#include<QFileDialog>
#include<QString>
#include<QDesktopServices>
#include<QMdiSubWindow>

#include"imainviewer.h"
#include"childviewer.h"
#include"multiviewer.h"


class MainView : public QMainWindow, public IMainViewer
{
    Q_OBJECT

public:
    MainView(QWidget *parent = 0);
    ~MainView();

private:
    QMenu* m_FileMenu;
    QMenu* m_EditMenu;
    QMenu* m_WindowMenu;
    QMenu* m_HelpMenu;

    QAction* m_OpenAction;
    QAction* m_SaveAction;
    QAction* m_QuitAction;
    
    QAction* m_ZoomIn;
    QAction* m_ZoomOut;
    QAction* m_ZoomReset;
    
    QAction* m_PreviousPage;
    QAction* m_NextPage;
    QLineEdit* m_PageNumLineEdit;
    QLabel* m_PageCountLabel;
    
    QToolBar* m_FileTool;
    QToolBar* m_ZoomTool;
    QToolBar* m_PageTool;

public:
    void CreateToolBar();
    void CreateActions();
    void CreateMenus();
    
public slots:
    void UpdatePageNum();
    void GotoPage();

    void OpenFile();
    void SaveFile();
    void CloseFile();
    void NewFile();
    void ZoomIn();
    void ZoomOut();
    void ZoomReset();
    void PreviousPage();
    void NextPage();

public:
    virtual QString getFileName(){
        return m_strFileName;
    }
    virtual QLineEdit* getPageNumLineEdit(){
        return m_PageNumLineEdit;
    }
public:
    MultiViewer* m_MainMdiArea;
    ChildViewer* CreateChildViewer();
private:
    QString m_strFileName;
    void RefreshWindow();
    ChildViewer* getCurChildViewer();
};

#endif // MAINVIEW_H
