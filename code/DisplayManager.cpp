#include "StdAfx.h"
#include "DisplayManager.h"

DisplayManager::DisplayManager( QTreeWidget *treewidget,QTabWidget *tabwidget,QObject *parent )
{
	m_pDBTree = treewidget;
	m_pPicWidget = tabwidget->widget(PIC_WIDGET);
	m_pSTLWidget = tabwidget->widget(STL_WIDGET);
		
	//initialWidgetPool();
	initialTabLayout();
	
	connect(m_pDBTree,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),this,SLOT(onDBTreeitemDoubleClicked(QTreeWidgetItem *,int)));
}


DisplayManager::~DisplayManager()
{
	//delete m_pvlayoutPic;  //delete this layout
	delete m_STLDisplay;
	delete m_pPicgroup;
}

void DisplayManager::initialWidgetPool()
{
	for (int i=0;i<MAXPOOLSIZE;i++)
	{
		m_jpegPicWidgetPool.append(new qJPEGWidget());
		m_jpegPicWidgetPool.last()->setVisible(false);
		m_jpegBlackPicWidgetPool.append(new qJPEGWidget());
		m_jpegBlackPicWidgetPool.last()->setVisible(false);

		connect(m_jpegPicWidgetPool.last(),SIGNAL(changeSelfSize(QWidget *,bool)),this,SLOT(updatePicPool(QWidget *,bool)));
		connect(m_jpegBlackPicWidgetPool.last(),SIGNAL(changeSelfSize(QWidget *,bool)),this,SLOT(updateBlackPicPool(QWidget *,bool)));
	}
}

void DisplayManager::initialTabLayout()
{
	//pic
// 	m_pPicWidget->setLayout(&m_vPiclayout);
// 	m_pVPicSplitter=new QSplitter(m_pPicWidget);
// 	m_pVPicSplitter->setOrientation(Qt::Vertical);
// 	m_pVPicSplitter->setHandleWidth(1);
// 	for (int k=0;k<MAXPOOLSIZE/2;k++)
// 	{
// 		QSplitter *tempHsplitter=new QSplitter(m_pVPicSplitter);
// 		tempHsplitter->setHandleWidth(2);
// 		tempHsplitter->setOrientation(Qt::Horizontal);
// 		tempHsplitter->addWidget(m_jpegPicWidgetPool.value(2*k));
// 		tempHsplitter->addWidget(m_jpegPicWidgetPool.value(2*k+1));
// 	}
// 	m_vPiclayout.addWidget(m_pVPicSplitter);
// 	m_pVPicSplitter->setChildrenCollapsible(false);
		
	//stl
 	m_STLDisplay = new STLDisplayWidget();
 	m_STLDisplay->enableColor(true);
  	m_vSTLlayout.addWidget(m_STLDisplay);
 	m_pSTLWidget->setLayout(&m_vSTLlayout); 
}

void DisplayManager::onDBTreeitemDoubleClicked(QTreeWidgetItem * item, int column )
{
#if 0
	if (item->parent()==NULL)
	{
		int index=m_pDBTree->indexOfTopLevelItem(item);
		QMessageBox::warning(m_pDBTree,"waring",QString("Top level treeitem was double clicked! Index = %1").arg(index));
	}
#endif

	if (item==NULL)
		return;

	TreeItem *curtreeitem = m_pTreeItemMap->value(item);
// 	QMessageBox::warning(m_pDBTree,"waring",QString("Item on clicked. name=%1 ,info=%2").arg(name).arg(info));
// 	QMessageBox::warning(m_pDBTree,"waring",QString("path= %1").arg(fullpath));

	QStringList picFiles;
	QStringList names;
	QStringList stlFiles;
	ColorArray carray;

	if (m_pFolderManager->getPicsFileForDisplay(curtreeitem,picFiles,names))
	{
//		QMessageBox::information(0,picFiles.at(0),names.at(0));
		m_pPicgroup->display(picFiles,names);
	}
	else
	{
		m_pPicgroup->display(QStringList(),QStringList());    //play nothing

	}
	
	QString path=m_pFolderManager->buildFullPath(curtreeitem);

	if(AddLevel2InfoDlg::loadColorInfoFromFile(path,carray,stlFiles))
	{
		m_STLDisplay->clear3D();

		QStringList tmplist;
		for(int i=0;i<stlFiles.size();i++)
		{
			tmplist.append(path+"/"+stlFiles.value(i));
		}

		m_STLDisplay->setFiles(tmplist);
		m_STLDisplay->setColors(carray);
		m_STLDisplay->enableColor(true);
		m_STLDisplay->display();
	}
	else
	{
		m_STLDisplay->clear3D();   //play nothing
	}
}

void DisplayManager::setTreeItemMap(QMap<QTreeWidgetItem *,TreeItem *> *pTreeItemMap)
{
	m_pTreeItemMap=pTreeItemMap;
}

void DisplayManager::setTreeOpsManager(TreeOpsManager *pTreeOpsManager)
{
	m_pTreeOpsManager=pTreeOpsManager;
}

void DisplayManager::setFolderManager(FolderManager *pFolderManager )
{
	m_pFolderManager=pFolderManager;
}

void DisplayManager::updatePicPool( QWidget *pwidget ,bool bexpand )
{
	if (pwidget==NULL)
	{
		return;
	}
	qJPEGWidget * pJPEGWidget=(qJPEGWidget *)pwidget;
	for (int i=0;i<m_numCurPics;i++)
	{
		qJPEGWidget *curJPEGWidget=m_jpegPicWidgetPool.value(i);
		if (bexpand)
		{
			//only display the one be doubleclicked
			if (curJPEGWidget==pJPEGWidget)
				curJPEGWidget->setVisible(true);
			else
				curJPEGWidget->setVisible(false);
		}
		else
			curJPEGWidget->setVisible(true);  //expand all widgets in the pool
	}
}

void DisplayManager::displayPic(QStringList picFilelist,QStringList namelist)
{
	//phase destroy the former displays

	int size=picFilelist.size();
	
	for(int i=0;i<size;i++)
	{
		qJPEGWidget *curJPEGWidget=new qJPEGWidget;
		curJPEGWidget->setJPGFileName(picFilelist.value(i));
		curJPEGWidget->setName(namelist.value(i));
		curJPEGWidget->display();
		m_hPiclayout.addWidget(curJPEGWidget);
	}
	m_pPicWidget->setLayout(&m_hPiclayout);
	m_picarea.setWidget(m_pPicWidget);
			
	//first clear all displayed pictrues
#if 0
	clearDisplay(&m_jpegPicWidgetPool);
	
	int numPic=picFilelist.size();
	if (numPic==0)
	{
		//Display nothing
		return;

	}
	if (numPic>= MAXPOOLSIZE)
		numPic=MAXPOOLSIZE;

	m_numCurPics=numPic;
	for(int i=0;i<numPic;i++)
	{
		qJPEGWidget *curJPEGWidget=m_jpegPicWidgetPool.value(i);
		curJPEGWidget->setJPGFileName(picFilelist.value(i));
		curJPEGWidget->setName(namelist.value(i));
		curJPEGWidget->display();
		curJPEGWidget->setVisible(true);
	}
#endif
}

void DisplayManager::displaySTL(QStringList stlFilelist/* ,QStringList namelist*/)
{
	//clear thr formor display
	m_STLDisplay->clear3D();

	m_STLDisplay->setFiles(stlFilelist);
	m_STLDisplay->display();
}

void DisplayManager::clearGridLayout(QGridLayout &layout)
{
	int col=layout.columnCount();
	int row=layout.rowCount();

	for(int i=0;i<col;i++)
	{
		for(int j=0;j<row;j++)
		{
			QLayoutItem *curitem=layout.itemAtPosition(j,i);
			if (curitem!=NULL)
				layout.removeItem(curitem);
		}
	}
	layout.update();
}

void DisplayManager::clearDisplay(QVector<qJPEGWidget *> *pPool )
{
	for (QVector<qJPEGWidget *>::iterator i=pPool->begin();
		i!=pPool->end();i++)
	{
		(*i)->clearWidget();
		(*i)->setVisible(false);
	}
}

void DisplayManager::setPicGroup( PicGroup *picgroups)
{
	m_pPicgroup=picgroups;
}