#include "StdAfx.h"
#include "TreeOpsManager.h"

TreeOpsManager::TreeOpsManager(QObject *parent)
	: QObject(parent)
{

}

TreeOpsManager::~TreeOpsManager()
{

}

bool TreeOpsManager::readDbTreeFromFile( TreeItem * &root,const char *filename )
{
	m_storeEntries.clear();

	if (!readStoreEntriesFromFile(filename))
		return false;

	for (QVector<storeItem>::iterator i=m_storeEntries.begin();i!=m_storeEntries.end();i++)
	{
		TreeItem *curTreeItem=i->item;

		if ((i->parentID==-2))
			curTreeItem->setParent(NULL);  //this item is root, its parentID=-2 and parent is NULL
		else
		{
			int parentPosInArray=1+(i->parentID);
			TreeItem *patrentItem=m_storeEntries.value(parentPosInArray).item;
			patrentItem->addChildren(curTreeItem);
		}
	}
	root=m_storeEntries.value(0).item;
	return true;
}

bool TreeOpsManager::writeDbTreeToFile( TreeItem *root,const char *filename)
{
	if(root==NULL)
		return false;

	m_treeEntries.clear();

	updateIndexOfTree(root,false);
	preTraversalAddTreeItem(root);
	updateStoreEntries();

	return writeStoreEntriesToFile(filename);
}

//update the newfoldermap in FolderManager ,and all the new index in each treeitem.
void TreeOpsManager::updateIndexOfTree( TreeItem *root,bool newCreate/* =false*/,int mode/*=PRE_TRAVERSAL*/ )
{
	m_pFolderManager->m_newfoldermap.clear();  //clear this map

	m_treeIndex=-1;
	preTraversalTree(root,newCreate);

	if (!newCreate)
	{
		m_pFolderManager->m_oldermap.clear();
		m_pFolderManager->m_oldermap=m_pFolderManager->m_newfoldermap;
	}
}

void TreeOpsManager::preTraversalTree( TreeItem *parent,bool newCreate)
{
	int oldIndex=parent->getIndex();
	parent->setIndex(m_treeIndex++); //set the new index
	if (newCreate)
	{
		// do nothing for the newfoldermap
	}
	else
	{
		if (oldIndex>=0) //means the item is new
		{
			int oldValue=m_pFolderManager->m_oldermap.value(oldIndex); 
			int newIndex=parent->getIndex();
			m_pFolderManager->m_newfoldermap.insert(newIndex,oldValue);
		}
	}

	for (int i=0;i<parent->childCount();i++)
	{
		preTraversalTree(parent->child(i),newCreate);
	}
}

void TreeOpsManager::preTraversalAddTreeItem( TreeItem *item )
{
	if(item==NULL)
		return ;
	m_treeEntries.append(item);

	for(int i=0;i<item->childCount();i++)
	{
		TreeItem *curchilditem=item->child(i);
		preTraversalAddTreeItem(curchilditem);
	}
}

void TreeOpsManager::updateStoreEntries()
{
	m_storeEntries.clear();
	for (QVector<TreeItem *>::iterator i=m_treeEntries.begin();i!=m_treeEntries.end();i++)
	{
		storeItem tmpStoreItem;
		tmpStoreItem.item=(*i);
		if ((*i)->getParent()==NULL)			
			tmpStoreItem.parentID=-2;
		else
			tmpStoreItem.parentID=(*i)->getParent()->getIndex();
		m_storeEntries.append(tmpStoreItem);
	}
}

bool TreeOpsManager::writeStoreEntriesToFile( const char *filename)
{
	FILE *fhandle=fopen(filename,"wb");
	if (NULL==fhandle)
		return false;

	for(QVector<storeItem>::iterator i=m_storeEntries.begin();i!=m_storeEntries.end();i++)
	{
		storeData tmpstoreDate; 
		tmpstoreDate.data=(*i).item->data();
		tmpstoreDate.parentID=(*i).parentID;
		fwrite((void *)&tmpstoreDate,sizeof(storeData),1,fhandle);
	}
	fclose(fhandle);
	return true;
}

bool TreeOpsManager::readStoreEntriesFromFile( const char *filename )
{
	FILE *fhandle=fopen(filename,"rb");

	if (NULL==fhandle)
		return false;

	storeData tmpstoreDate;

	while(fread((void *)&tmpstoreDate,sizeof(storeData),1,fhandle)!=0)
	{   
		storeItem tmpstoreItem;

		TreeItem *newItem=new TreeItem(tmpstoreDate.data);
		tmpstoreItem.item=newItem;
		tmpstoreItem.parentID=tmpstoreDate.parentID;

		m_storeEntries.append(tmpstoreItem);
	}

	fclose(fhandle);

	return true;
}

void TreeOpsManager::saveToDB( TreeItem *root )
{
	//ignore the failed situation, assume return true
	writeDbTreeToFile(root,g_dbFileName);
}

void TreeOpsManager::setFolderManager( FolderManager *pFolderManager )
{
	m_pFolderManager=pFolderManager;
}