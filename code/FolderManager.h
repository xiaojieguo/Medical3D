#ifndef FOLDERMANAGER_H
#define FOLDERMANAGER_H

#define  NAMESPACE_SIZE 10000

class TreeOpsManager;

class FolderManager : public QObject
{
	Q_OBJECT

public:
	FolderManager(QObject *parent=NULL);
	~FolderManager();

	void setTreeOpsManager(TreeOpsManager *pTreeOpsManager);
	bool mkdirOfTreeItem(TreeItem *item);
	bool deleteFolder(TreeItem *item);

	bool saveFolderMapToFile(const char* filename); 

	bool loadFolderMapFromFile(const char* filename);

	QString buildFullPath(TreeItem *item);

	bool getPicsFileForDisplay(TreeItem *item,QStringList &pics,QStringList &names);

	bool getSTLFileForDisplay(TreeItem *item,QStringList &stls);

	int getDeepLevel(TreeItem *item);

private:
	bool deleteFolderWithName(QString foldername);
	bool _removeAllRelationship(TreeItem *item);
	int _getUniqueName();
	void _loadNameSpaceInfo();     //initialize the bool array of space
	QString foundFolderName(TreeItem *item);

	bool deleteFolderAllItems(QString foldermap);
	bool getPicFromFolder(QString foldername,QString &filename);

private slots:
	void changeNameMap(int oldindex,int newindex);

public slots:
	void savingMapFile();

public:
	QMap<int,int> m_oldermap;  //<index , foldername to int>

	//when the tree changed , the foldermap should be updated. we use newfoldermap to store all the
	//new relationships and set foldermap = newfoldermap. 
	QMap<int,int> m_newfoldermap;  //<index , foldername to int>

private:
	bool m_bSpace[NAMESPACE_SIZE];
	TreeOpsManager *m_pTreeOpsManager;
};

#endif // FOLDERMANAGER_H