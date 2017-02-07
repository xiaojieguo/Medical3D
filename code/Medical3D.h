#ifndef MEDICAL3D_H
#define MEDICAL3D_H

#include "ui_Medical3D.h"

class Medical3D : public QMainWindow
{
	Q_OBJECT

public:
	Medical3D(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Medical3D();

private slots:
	void UpdateDbTreeView(TreeItem *);
	void openDbDlg();
	void showAboutDlg();

private:
	Ui::Medical3DClass m_ui;

	PicGroup *m_pPicgroup;

	MedicalDbDlg *m_pMedicalDbDlg;

	FolderManager *m_pFolderManager;
	VisualManager *m_pVisualManager;
	TreeOpsManager *m_pTreeOpsManager;
	DisplayManager *m_pDisplayManager;

	TreeItem *m_pDbTreeRoot;

	void _updateDbTree(TreeItem *,QTreeWidgetItem *);

	QMap<QTreeWidgetItem *,TreeItem *> m_treeItemMap;
};

#endif // MEDICAL3D_H