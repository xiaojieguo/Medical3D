#include "stdafx.h"

extern const char  *g_dbFileName="medicalDb.db";
extern const char  *g_folderMapFileName="folderInfo.info";

Medical3D::Medical3D(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	m_ui.setupUi(this);
	m_ui.dbTreeView->expandAll();
	m_pMedicalDbDlg=new MedicalDbDlg(this);
	m_pFolderManager=new FolderManager;
	m_pVisualManager=new VisualManager;
	m_pTreeOpsManager=new TreeOpsManager;
	m_pDisplayManager=new DisplayManager(m_ui.dbTreeView,m_ui.displaytabWidget);
	
	m_pPicgroup=new PicGroup;
	m_ui.picScrollArea->setWidget(m_pPicgroup);
	m_pDisplayManager->setPicGroup(m_pPicgroup);

	m_pTreeOpsManager->setFolderManager(m_pFolderManager);

	m_pMedicalDbDlg->setFolderManager(m_pFolderManager);
	m_pMedicalDbDlg->setTreeOpsManager(m_pTreeOpsManager);
	
	if(!QFile::exists(QString::fromAscii(g_dbFileName)))
	{
		m_pDbTreeRoot=new TreeItem("I am root",NULL);
		m_pTreeOpsManager->writeDbTreeToFile(m_pDbTreeRoot,g_dbFileName);
	}
	else
	{
		if(!m_pTreeOpsManager->readDbTreeFromFile(m_pDbTreeRoot,g_dbFileName))
		{
			//read database error
			QMessageBox::warning(this,"No Database File",tr("Load database error !"));
			m_pDbTreeRoot=new TreeItem("I am root",NULL);
			m_pTreeOpsManager->writeDbTreeToFile(m_pDbTreeRoot,g_dbFileName);
		}
	}
	
	m_pTreeOpsManager->updateIndexOfTree(m_pDbTreeRoot,true);

	m_pMedicalDbDlg->setDbTree(m_pDbTreeRoot);
	m_pDisplayManager->setTreeItemMap(&m_treeItemMap);
	m_pDisplayManager->setTreeOpsManager(m_pTreeOpsManager);
	m_pDisplayManager->setFolderManager(m_pFolderManager);

	UpdateDbTreeView(m_pDbTreeRoot);
	
	connect(m_ui.dbOpsAction,SIGNAL(triggered()),this,SLOT(openDbDlg()));
	//connect(ui.action,SIGNAL(triggered()), (QObject *)m_pDisplayManager->m_STLDisplay,SLOT( xxx()));
	connect(m_ui.action,SIGNAL(triggered()), this,SLOT(showAboutDlg()));
	connect(m_pTreeOpsManager,SIGNAL(treeItemIndexChanged(int,int)),m_pFolderManager,SLOT(changeNameMap(int, int)));
	connect(m_pMedicalDbDlg,SIGNAL(dbTreeChanged(TreeItem *)),m_pTreeOpsManager,SLOT(saveToDB(TreeItem *)));
	connect(m_pMedicalDbDlg,SIGNAL(dbTreeChanged(TreeItem *)),this,SLOT(UpdateDbTreeView(TreeItem *)));
	connect(m_pTreeOpsManager,SIGNAL(treeFolderMapNeedSaved()),m_pFolderManager,SLOT(savingMapFile()));

//	connect(m_ui.dbTreeView,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),this,SLOT(showItemInfo(QTreeWidgetItem *,int)));

	this->showMaximized();
}

Medical3D::~Medical3D()
{
	delete m_pFolderManager;
	delete m_pVisualManager;
	delete m_pTreeOpsManager;
	delete m_pDbTreeRoot;
	delete m_pDisplayManager;
}

void Medical3D::UpdateDbTreeView(TreeItem *root)
{
	m_ui.dbTreeView->clear();
	m_treeItemMap.clear(); //clear the <QTreeWidgetItem ,TreeItem> Map
	_updateDbTree(root,NULL);
	m_ui.dbTreeView->expandAll();
}

void Medical3D::openDbDlg()
{
	m_pMedicalDbDlg->exec();
}

void Medical3D::_updateDbTree( TreeItem *item, QTreeWidgetItem *parent )
{
	if(item->childCount()<=0)
		return;

	QTreeWidgetItem *treewidgetitem;

	QIcon icon;
	for(int index=0;index<(item->childCount());index++)
	{
		TreeItem *curitem=item->child(index); 
		if (curitem->getParent()==m_pDbTreeRoot)
		{
			treewidgetitem=new QTreeWidgetItem(m_ui.dbTreeView);
		}
		else
		{
			treewidgetitem = new QTreeWidgetItem(parent);
		}
		treewidgetitem->setText(0,QString::fromAscii(curitem->data().name));
		m_treeItemMap.insert(treewidgetitem,curitem);
		int deep=m_pFolderManager->getDeepLevel(curitem);
		switch(deep)
		{
		case 1:
			icon.addFile(":/Medical3D/Icons/Frac.png");
			break;
		case 2:
			icon.addFile(":/Medical3D/Icons/Flag.png");
			break;
		case 3:
			//one star
			icon.addFile(":/Medical3D/Icons/OneStar.png");
			break;
		case 4:
			//two stars
			icon.addFile(":/Medical3D/Icons/TwoStars.png");
			break;
		default:
			break;
		}

		if (deep>0 && deep <5)
			treewidgetitem->setIcon(0,icon);

		_updateDbTree(curitem, treewidgetitem);
	}
}

void Medical3D::showAboutDlg()
{
	qAboutDlg aboutdlg;
	aboutdlg.exec();
}