#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#define  PIC_WIDGET 0
#define  STL_WIDGET	1

#define  MAXPOOLSIZE 14

class DisplayManager : public QObject
{
	Q_OBJECT

public:
	DisplayManager(QTreeWidget *treewidget,QTabWidget *tabwidget,QObject *parent=NULL);
	~DisplayManager();

	void setTreeItemMap(QMap<QTreeWidgetItem *,TreeItem *> *pTreeItemMap);
	void setTreeOpsManager(TreeOpsManager *pTreeOpsManager);
	void setFolderManager(FolderManager *pFolderManager);
	void setPicGroup(PicGroup *picgroups);

private:
	void displayPic(QStringList picFilelist,QStringList namelist);
	void displaySTL(QStringList stlFilelist/*,QStringList namelist*/); //if size of stlFilelist ,display Null
	
	void clearDisplay(QVector<qJPEGWidget *> *pPool);
	void clearGridLayout(QGridLayout &layout);

	void initialWidgetPool();
	void initialTabLayout(); 

private slots:
	void onDBTreeitemDoubleClicked(QTreeWidgetItem * item, int column);
	void updatePicPool(QWidget *pwidget ,bool bexpand);

public:
	STLDisplayWidget *m_STLDisplay;

private:
	QTreeWidget *m_pDBTree;
	QWidget *m_pPicWidget;
	QWidget *m_pSTLWidget;

	QMap<QTreeWidgetItem *,TreeItem *> *m_pTreeItemMap;
	TreeOpsManager *m_pTreeOpsManager;
	FolderManager *m_pFolderManager;

	JPEGDispalyWidget *m_JPEGDisplay;

	QGridLayout m_picGridLayout;

	QVBoxLayout *m_pvlayoutPic;
	QSplitter *m_pvSplitterPic;

	QVBoxLayout *m_pvlayoutBlackPics;
	QSplitter *m_pvSplitterBlackPics;

	QVBoxLayout vlayout2;

	QVector<qJPEGWidget *> m_jpegPicWidgetPool;   //pool for display jpeg on m_pPicWidget
	QVector<qJPEGWidget *> m_jpegBlackPicWidgetPool; //pool for display jpeg on m_pBlackPicWidget

	QVBoxLayout m_vPiclayout;
	QVBoxLayout m_vSTLlayout;
	QSplitter *m_pVPicSplitter;
	QSplitter *m_pVBlackpicSplitter;

	int m_numCurPics;
	int m_numCurBlackPics;

	int m_numOfPics;
	int m_numOfBlackPics;
	int m_numOfSTLs;

	QHBoxLayout m_hPiclayout;
	QScrollArea m_picarea;
	PicGroup *m_pPicgroup;
};

#endif // DISPLAYMANAGER_H