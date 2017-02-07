#include "StdAfx.h"
#include "MedicalDbDlg.h"

MedicalDbDlg::MedicalDbDlg(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_pFolderManager=NULL;
	m_pTreeOpsManager=NULL;
	m_pDbTreeRoot=NULL;

	menu=NULL;

	m_ui.dbListView->setContextMenuPolicy(Qt::CustomContextMenu); 
	connect(m_ui.addFracPartBtn,SIGNAL(clicked()),this,SLOT(addFracPartEntry()));
	connect(m_ui.addClassNameBtn,SIGNAL(clicked()),this,SLOT(addClassNameEntry()));
	connect(m_ui.addLevel1Btn,SIGNAL(clicked()),this,SLOT(addLevel1Entry()));
	connect(m_ui.addLevel2Btn,SIGNAL(clicked()),this,SLOT(addLevel2Entry()));
	connect(m_ui.deleteItemBtn,SIGNAL(clicked()),this,SLOT(deleteEntry()));
	connect(this,SIGNAL(dbTreeChanged(TreeItem *)),this,SLOT(updateDBListView(TreeItem *)));
	connect(m_ui.okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	connect(m_ui.dbListView,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
		this,SLOT(editDb(QTreeWidgetItem *, int)));
	connect(m_ui.dbListView, SIGNAL(customContextMenuRequested(const QPoint&)), 
		this, SLOT(showContextMenu(const QPoint&)));
}

MedicalDbDlg::~MedicalDbDlg()
{

}

void MedicalDbDlg::setFolderManager( FolderManager *pFolderManager )
{
	m_pFolderManager=pFolderManager;
}

void MedicalDbDlg::setTreeOpsManager(TreeOpsManager *pTreeOpsManager)
{
	m_pTreeOpsManager=pTreeOpsManager;
}

void MedicalDbDlg::setDbTree( TreeItem *root )
{
	m_pDbTreeRoot=root;
	emit dbTreeChanged(m_pDbTreeRoot);
}

void MedicalDbDlg::addFracPartEntry()
{
	AddCommonInfoDlg addFracPartDlg(tr("Please input the fracture site"));
	if(QDialog::Accepted==addFracPartDlg.exec())
	{
		TreeItem *newItem = new TreeItem(addFracPartDlg.getName());
		m_pDbTreeRoot->addChildren(newItem);

		m_pTreeOpsManager->updateIndexOfTree(m_pDbTreeRoot);
		m_pFolderManager->mkdirOfTreeItem(newItem);

		m_pFolderManager->savingMapFile();

		updateDBListView(m_pDbTreeRoot);
		emit dbTreeChanged(m_pDbTreeRoot);
	}
}

void MedicalDbDlg::addClassNameEntry()
{
	if (m_ui.dbListView->currentItem()==NULL)
	{
		QMessageBox::warning(this,"warning",tr("The entry is not selected !"));
		return;
	}

	if (m_ui.dbListView->currentColumn()!=0)
	{
		QMessageBox::warning(this,"warning",tr("Select the fracture site !"));
		return;
	}

	AddCommonInfoDlg addClassNameDlg(tr("Please input category name"));
	if(QDialog::Accepted==addClassNameDlg.exec())
	{
		//do something
		int rowindex=m_ui.dbListView->indexOfTopLevelItem(m_ui.dbListView->currentItem());
		TreeItem *selectedEntry=bottomTreeEntries.value(rowindex);
		TreeItem *parentAppendEntry=_getParentOfColunmIndex(selectedEntry,1); 

		TreeItem *newItem=new TreeItem(addClassNameDlg.getName());	
		parentAppendEntry->addChildren(newItem);

		m_pTreeOpsManager->updateIndexOfTree(m_pDbTreeRoot);
		m_pFolderManager->mkdirOfTreeItem(newItem);
		//save it again ,because mkdirOfTreeItem create one new map relationship
		m_pFolderManager->savingMapFile();

		updateDBListView(m_pDbTreeRoot);


		emit dbTreeChanged(m_pDbTreeRoot);
	}
}

void MedicalDbDlg::addLevel1Entry()
{
	if (m_ui.dbListView->currentItem()==NULL)
	{
		QMessageBox::warning(this,"warning",tr("The entry is not selected !"));
		return;
	}

	if (m_ui.dbListView->currentColumn()!=1)
	{
		QMessageBox::warning(this,"warning",tr("Select the category name"));
		return;
	}

	AddCommonInfoDlg addLevel1Dlg(tr("Input the first classification"));
	if(QDialog::Accepted==addLevel1Dlg.exec())
	{
		//do something
		int rowindex=m_ui.dbListView->indexOfTopLevelItem(m_ui.dbListView->currentItem());
		TreeItem *selectedEntry=bottomTreeEntries.value(rowindex);
		TreeItem *parentAppendEntry=_getParentOfColunmIndex(selectedEntry,2); 

		TreeItem *newItem=new TreeItem(addLevel1Dlg.getName());	
		parentAppendEntry->addChildren(newItem);

		m_pTreeOpsManager->updateIndexOfTree(m_pDbTreeRoot);
		m_pFolderManager->mkdirOfTreeItem(newItem);

		//save it again ,because mkdirOfTreeItem create one new map relationship
		m_pFolderManager->savingMapFile();

		updateDBListView(m_pDbTreeRoot);
		emit dbTreeChanged(m_pDbTreeRoot);
	}
}

void MedicalDbDlg::addLevel2Entry()
{
	if (m_ui.dbListView->currentItem()==NULL)
	{
		QMessageBox::warning(this,"warning",tr("The entry is not selected !"));
		return;
	}
	if (m_ui.dbListView->currentColumn() != 2)
	{
		QMessageBox::warning(this,"warning",tr("Select the first classification"));
		return;
	}

	AddLevel2InfoDlg addLevel2Dlg;
	if(QDialog::Accepted==addLevel2Dlg.exec())
	{
		int rowindex=m_ui.dbListView->indexOfTopLevelItem(m_ui.dbListView->currentItem());
		TreeItem *selectedEntry=bottomTreeEntries.value(rowindex);
		TreeItem *parentAppendEntry=_getParentOfColunmIndex(selectedEntry,3); 

		TreeItem *newItem=new TreeItem(addLevel2Dlg.getName(),NULL);	
		parentAppendEntry->addChildren(newItem);

		m_pTreeOpsManager->updateIndexOfTree(m_pDbTreeRoot);
		m_pFolderManager->mkdirOfTreeItem(newItem);

		//save it again ,because mkdirOfTreeItem create one new map relationship
		m_pFolderManager->savingMapFile();


		updateDBListView(m_pDbTreeRoot);
		QString desPath=m_pFolderManager->buildFullPath(newItem);
		//phase 2 : copy all the file to folder 
		bool res=addLevel2Dlg.copyFileToDesFolder(desPath);	
		//phase 3: save file map information
		if (res)
		{
			//addLevel3Dlg.saveMapFile(desPath);
		}

		emit dbTreeChanged(m_pDbTreeRoot);

	}
}

void MedicalDbDlg::deleteEntry()
{
	if (m_ui.dbListView->currentItem()==NULL)
	{
		QMessageBox::warning(this,"warning","The entry is not selected !");
		//qDebug("No item select!");
		return;
	}

	if (QMessageBox::Cancel==QMessageBox::warning(this,"Warning","Are you sure to delete the entry?", 
		QMessageBox::Ok|QMessageBox::Cancel))
		return;

	//delete the item you selected
	int rowindex=m_ui.dbListView->indexOfTopLevelItem(m_ui.dbListView->currentItem());

	TreeItem *selectedEntry=bottomTreeEntries.value(rowindex);
	TreeItem *parentEntry=_getParentOfColunmIndex(selectedEntry,m_ui.dbListView->currentColumn()); 
	TreeItem *entryFordelete=_getParentOfColunmIndex(selectedEntry,m_ui.dbListView->currentColumn()+1);

	m_pFolderManager->deleteFolder(entryFordelete);
	parentEntry->removeChildren(entryFordelete);

	m_pTreeOpsManager->updateIndexOfTree(m_pDbTreeRoot);

	//save it again ,because mkdirOfTreeItem create one new map relationship
	m_pFolderManager->savingMapFile();

	updateDBListView(m_pDbTreeRoot);
	emit dbTreeChanged(m_pDbTreeRoot);
}

int MedicalDbDlg::_getDeepLevel( TreeItem *item )
{
	int deep=0;
	TreeItem *tmp=item;
	while(tmp->getParent()!=NULL)
	{
		tmp=tmp->getParent();
		deep++;
	}
	return deep;
}

void MedicalDbDlg::updateDBListView( TreeItem *root )
{
	m_ui.dbListView->clear();
	bottomTreeEntries.clear();
	_updateBottomTreeEntries(root);

#if 1
	for (int i=0;i<bottomTreeEntries.size();i++)
	{
		TreeItem *curentry=bottomTreeEntries.value(i);
		TreeItem *tmpentry=curentry;
		QTreeWidgetItem *curtreeitem=new QTreeWidgetItem(m_ui.dbListView);
		for (int j=_getDeepLevel(curentry);j>0;j--)
		{
			curtreeitem->setText(j-1,QString::fromAscii(tmpentry->data().name));
			tmpentry=tmpentry->getParent();
		}
	}
#else
	//make the table look like a tree

#endif
}

void MedicalDbDlg::_updateBottomTreeEntries( TreeItem *root )
{
	for (int i=0; i<root->childCount();i++)
	{
		TreeItem *curitem=root->child(i);
		//this item is the bottom of tree, deepest one... 
		if (curitem->childCount()==0)
		{
			bottomTreeEntries.append(curitem);
		}
		else
		{
			_updateBottomTreeEntries(curitem);
		}
	}
}

TreeItem * MedicalDbDlg::_getParentOfColunmIndex( TreeItem *item,int colunm )
{
	const int MAX_LEVEL=5;
	if (colunm<0||colunm>MAX_LEVEL)
	{
		return NULL;
	}


	int curDeep=_getDeepLevel(item);

	TreeItem *tmp=item;
	for (int i=0;i<curDeep-colunm;i++)
	{
		tmp=tmp->getParent();
	}
	return tmp;
}

void MedicalDbDlg::editDb( QTreeWidgetItem *item, int column )
{
	if (item==NULL)
		return;

	int row=m_ui.dbListView->indexOfTopLevelItem(item);
	TreeItem *curbottomitem=bottomTreeEntries.value(row);
	if (NULL==curbottomitem)
		return;

	TreeItem *curtreeitem;  //the on double clicked tree item

	switch(column)
	{
	case 0:
		curtreeitem=_getParentOfColunmIndex(curbottomitem,1);
		editFracPart(curtreeitem);
		break;
	case 1:
		curtreeitem=_getParentOfColunmIndex(curbottomitem,2);
		editClassInfo(curtreeitem);
		break;
	case 2:
		curtreeitem=_getParentOfColunmIndex(curbottomitem,3);
		editLevel1(curtreeitem);
		break;
	case 3:
		curtreeitem=_getParentOfColunmIndex(curbottomitem,4);
		editLevel2(curtreeitem);
		break;
	default:
		break;
	}
}


void MedicalDbDlg::editFracPart(TreeItem *treeitem)
{
	if (NULL==treeitem)
		return;

	AddCommonInfoDlg addFracPartDlg(tr("Input the fracture site"));
	addFracPartDlg.ui.nameTextEdit->setPlainText(QString::fromAscii(treeitem->data().name));

	if(QDialog::Accepted==addFracPartDlg.exec())
	{
		UserData curdata;
		memset((void *)&curdata,0,sizeof(UserData));
		strcpy(curdata.name,addFracPartDlg.ui.nameTextEdit->toPlainText().toAscii().data());
		curdata.name[USERDATASIZE]=0;
		treeitem->setData(curdata);
		//updateDBListView(m_pDbTreeRoot);
		emit dbTreeChanged(m_pDbTreeRoot);
	}
}

void MedicalDbDlg::editClassInfo(TreeItem *treeitem)
{
	if (NULL==treeitem)
		return;

	AddCommonInfoDlg addClassNameDlg(tr("Input the classification name"));
	addClassNameDlg.ui.nameTextEdit->setPlainText(QString::fromAscii(treeitem->data().name));
	if(QDialog::Accepted==addClassNameDlg.exec())
	{
		UserData curdata;
		memset((void *)&curdata,0,sizeof(UserData));
		strcpy(curdata.name,addClassNameDlg.ui.nameTextEdit->toPlainText().toAscii().data());
		curdata.name[USERDATASIZE]=0;
		treeitem->setData(curdata);
		//updateDBListView(m_pDbTreeRoot);
		emit dbTreeChanged(m_pDbTreeRoot);
	}	
}

void MedicalDbDlg::editLevel1(TreeItem *treeitem)
{
	if (NULL==treeitem)
		return;

	AddCommonInfoDlg addLevel1Dlg(tr("Input the first classification name"));
	addLevel1Dlg.ui.nameTextEdit->setPlainText(QString::fromAscii(treeitem->data().name));
	if(QDialog::Accepted==addLevel1Dlg.exec())
	{
		UserData curdata;
		memset((void *)&curdata,0,sizeof(UserData));
		strcpy(curdata.name,addLevel1Dlg.ui.nameTextEdit->toPlainText().toAscii().data());
		curdata.name[USERDATASIZE]=0;
		treeitem->setData(curdata);
		//updateDBListView(m_pDbTreeRoot);
		emit dbTreeChanged(m_pDbTreeRoot);
	}
}

void MedicalDbDlg::editLevel2(TreeItem *treeitem)
{

	AddLevel2InfoDlg addLevel3Dlg;
	QString path=m_pFolderManager->buildFullPath(treeitem);
	addLevel3Dlg.restoreSetting(path);
	addLevel3Dlg.setFolderName(path);
	addLevel3Dlg.m_ui.nameTextEdit->setPlainText(QString::fromAscii(treeitem->data().name));
	if(QDialog::Accepted==addLevel3Dlg.exec())
	{
		//phase 1: change the name and information
		UserData curdata;
		memset((void *)&curdata,0,sizeof(UserData));
		strcpy(curdata.name,addLevel3Dlg.m_ui.nameTextEdit->toPlainText().toAscii().data());

		curdata.name[USERDATASIZE]=0;
		treeitem->setData(curdata);
		//phase 2:  copy the new added files to the folder of the treeitem
		bool res=addLevel3Dlg.copyFileToDesFolder(path);	
		//phase 3: save file map information
		if (res)
		{
			//addLevel3Dlg.saveMapFile(desPath);
		}

		emit dbTreeChanged(m_pDbTreeRoot);
	}
}


void MedicalDbDlg::showContextMenu(const QPoint& pos)
{
	if(menu)
	{
		delete menu;
		menu = NULL;
	}
	menu = new QMenu(this);

	QAction *addfracPartAction = menu->addAction(
		QIcon(":/Medical3D/Resources/add.png"),"Fracture site");
	QAction *addClassNameAction=menu->addAction(
		QIcon(":/Medical3D/Resources/add.png"),"Classification name");
	QAction *addLevel1Action=menu->addAction(
		QIcon(":/Medical3D/Resources/add.png"),"First classification");
	QAction *addLevel2Action=menu->addAction(
		QIcon(":/Medical3D/Resources/add.png"),"Second classification");
	QAction *deleteAction=menu->addAction(
		QIcon(":/Medical3D/Resources/delete.png"),"Delete");

	connect(addfracPartAction, SIGNAL(triggered(bool)), this, SLOT(onAddfracPartAction()));
	connect(addClassNameAction, SIGNAL(triggered(bool)), this, SLOT(onAddClassNameAction()));
	connect(addLevel1Action, SIGNAL(triggered(bool)), this, SLOT(onAddLevel1Action()));
	connect(addLevel2Action, SIGNAL(triggered(bool)), this, SLOT(onAddLevel2Action()));
	connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(onDeleteAction()));

	menu->exec(QCursor::pos());
}

void MedicalDbDlg::onAddfracPartAction()
{
	addFracPartEntry();
}

void MedicalDbDlg::onAddClassNameAction()
{
	addClassNameEntry();
}

void MedicalDbDlg::onAddLevel1Action()
{
	addLevel1Entry();
}

void MedicalDbDlg::onAddLevel2Action()
{
	addLevel2Entry();
}

void MedicalDbDlg::onDeleteAction()
{
	deleteEntry();
}