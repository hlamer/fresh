#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gui/pMainWindow.h>

#include <QtGui>

class pActionsNodeModel;
class pActionsNodeMenuBar;

class MainWindow : public pMainWindow
{
	Q_OBJECT
	
public:
	MainWindow( QWidget* parent = 0 );
	virtual ~MainWindow();

protected:
	QPushButton* pbAddAction;
	QPushButton* pbRemoveAction;
	QPushButton* pbEditTextNode;
	QPushButton* pbEditShortcuts;
	pActionsNodeModel* mActionsModel;
	pActionsNodeMenuBar* mMenuBar;
	QTreeView* tvActions;

protected slots:
	void pbAddAction_clicked();
	void pbRemoveAction_clicked();
	void pbEditTextNode_clicked();
	void pbEditShortcuts_clicked();
};

#endif // MAINWINDOW_H