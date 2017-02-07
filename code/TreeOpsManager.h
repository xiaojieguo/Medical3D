#ifndef TREEOPSMANAGER_H
#define TREEOPSMANAGER_H

#define PRE_TRAVERSAL 0   // preorder traversal
#define IN_TRAVERSAL  1   // inorder traversal

/*If the parentID=-2 means the parent is the root.*/
// The index of root is -1
typedef struct _storeitem
{
	int parentID;
	TreeItem *item;
}storeItem;

typedef struct _storeData
{
	int parentID;
	UserData data; 
}storeData;

class TreeOpsManager : public QObject
{
	Q_OBJECT

public:
	TreeOpsManager(QObject *parent=NULL);
	~TreeOpsManager();

	bool readDbTreeFromFile(TreeItem * &root,const char *filename);

	bool writeDbTreeToFile(TreeItem *root,const char *filename);

	void updateIndexOfTree(TreeItem *root,bool newCreate=false,int mode=PRE_TRAVERSAL);

	void setFolderManager( FolderManager *pFolderManager);

private:
	void preTraversalTree(TreeItem *parent,bool newCreate);
	void preTraversalAddTreeItem(TreeItem *item);
	void updateStoreEntries();
	bool writeStoreEntriesToFile(const char *filename);
	bool readStoreEntriesFromFile(const char *filename);

private slots:
	void saveToDB(TreeItem *root);

signals:
	void treeItemIndexChanged(int oldindex, int newindex);
	void treeFolderMapNeedSaved();

private:
	int m_treeIndex;
	FolderManager *m_pFolderManager;
	QVector<storeItem > m_storeEntries;  //blank at the first
	QVector<TreeItem *> m_treeEntries;
};

#endif // TREEOPSMANAGER_H