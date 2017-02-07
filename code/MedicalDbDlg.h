#ifndef MEDICALDBDLG_H
#define MEDICALDBDLG_H

#include "ui_MedicalDbDlg.h"

class MedicalDbDlg : public QDialog
{
	Q_OBJECT

public:
	MedicalDbDlg(QWidget *parent = 0);
	~MedicalDbDlg();

	void setFolderManager(FolderManager *pFolderManager);
	void setTreeOpsManager(TreeOpsManager *pTreeOpsManager);
	void setDbTree(TreeItem *root);

private:
	int _getDeepLevel(TreeItem *item);
	void _updateBottomTreeEntries(TreeItem *root);
	TreeItem * _getParentOfColunmIndex(TreeItem *item,int colunm);

	void editFracPart(TreeItem *treeitem);
	void editClassInfo(TreeItem *treeitem);
	void editLevel1(TreeItem *treeitem);
	void editLevel2(TreeItem *treeitem);

signals:
	//when db Tree added ,modified or deleted ,emit this signal
	void dbTreeChanged(TreeItem *root); 

private slots:
	void addFracPartEntry();
	void addClassNameEntry();
	void addLevel1Entry();
	void addLevel2Entry();
	void deleteEntry();
	void updateDBListView(TreeItem *root);
	void editDb(QTreeWidgetItem *item, int column);
	void showContextMenu(const QPoint& pos);

	void onAddfracPartAction();
	void onAddClassNameAction();
	void onAddLevel1Action();
	void onAddLevel2Action();
	void onDeleteAction();

public:
	Ui::MedicalDbDlg m_ui;

private:
	FolderManager *m_pFolderManager;
	TreeOpsManager *m_pTreeOpsManager;
	TreeItem *m_pDbTreeRoot;
	QVector<TreeItem *> bottomTreeEntries;
	QMenu *menu;
};

#endif // MEDICALDBDLG_H