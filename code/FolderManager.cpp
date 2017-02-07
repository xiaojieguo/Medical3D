#include "StdAfx.h"
#include "FolderManager.h"

const QString basename="./Datum";

FolderManager::FolderManager(QObject *parent)
	: QObject(parent)
{
	QDir dir;
	m_oldermap.clear();
	m_newfoldermap.clear();

	if (!dir.exists(basename))
	{
		dir.mkdir(basename);
	}

	if (QFile::exists(QString::fromAscii(g_folderMapFileName)))
	{
		if (!loadFolderMapFromFile(g_folderMapFileName))
		{
			//QMessageBox::warning(this,"No Map File",tr("Error£¡"));
		}
	}
	else
		_loadNameSpaceInfo();
}

FolderManager::~FolderManager()
{

}

bool FolderManager::mkdirOfTreeItem( TreeItem *item )
{
	int newUniqueName = _getUniqueName();
	if ( newUniqueName < 0 )
		return false;  //get unique name failed

	m_oldermap.insert(item->getIndex(),newUniqueName);

	QString newFolderFullPath=buildFullPath(item);

	QDir dir;
	return dir.mkdir(newFolderFullPath);
}

bool FolderManager::deleteFolder( TreeItem *item )
{
	QString foldername=buildFullPath(item);
	bool res = deleteFolderWithName(foldername);
	if (res)
	{
		_removeAllRelationship(item);
	}
	return res;
}

bool FolderManager::deleteFolderWithName( QString foldername )
{

	return deleteFolderAllItems(foldername);
}

bool FolderManager::_removeAllRelationship( TreeItem *item )
{
	//remove relationship  of itself
	int foldername= m_oldermap.take(item->getIndex());
	m_bSpace[foldername]=false;

	for (int i=0;i<item->childCount();i++)
	{
		_removeAllRelationship(item->child(i));
	}
	return true;
}

int FolderManager::_getUniqueName()
{
	for(int i=0;i<NAMESPACE_SIZE;i++)
	{
		if(!m_bSpace[i])
		{
			m_bSpace[i]=true;
			return i;
		}
	}
	return -1;
}

bool FolderManager::deleteFolderAllItems( QString foldermap )
{
	QDir filedir(foldermap);
	if(!filedir.exists())
	{
		return false;
	}

	QString tempstr;
	const QFileInfoList entries = filedir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot|QDir::Files);

	QFileInfoList::const_iterator i;
	for	(i=entries.begin();i!=entries.end();i++)
	{
		QFileInfo fi=*i;
		if (fi.isDir())
			deleteFolderAllItems(foldermap+"/"+fi.fileName());
		else if (fi.isFile())
			filedir.remove(fi.fileName());
		else
			;  //do nothing
	}
	//delete itself
	QDir dir;
	return dir.rmdir(foldermap);
}

void FolderManager::_loadNameSpaceInfo()
{
	memset((void *)m_bSpace,0,sizeof(m_bSpace)); //set all false

	QList<int> folders= m_oldermap.values();

	for (int i=0;i<folders.size();i++)
	{
		m_bSpace[folders.value(i)]=true;
	}
}

bool FolderManager::saveFolderMapToFile( const char* filename )
{
	FILE *filehandle=fopen(filename,"wb");

	if (NULL==filehandle)
		return false;

	QMapIterator<int, int> i(m_oldermap);
	int tmpbuf[2]; //tmpbuf[0] is key ,tmpbuf[1] is value
	
	while(i.hasNext())
	{
		i.next();
		tmpbuf[0]=i.key();
		tmpbuf[1]=i.value();

		fwrite((void *)tmpbuf,sizeof(int),2,filehandle);
	}

	fclose(filehandle);
	return true;
}

bool FolderManager::loadFolderMapFromFile( const char* filename )
{
	FILE *filehandle=fopen(filename,"rb");

	if (NULL==filehandle)
		return false;

	m_oldermap.clear();

	int tmpbuf[2]; //tmpbuf[0] is key ,tmpbuf[1] is value

	while(fread((void *)tmpbuf,sizeof(int),2,filehandle)!=0)
		m_oldermap.insert(tmpbuf[0],tmpbuf[1]);

	fclose(filehandle);

	_loadNameSpaceInfo();

	return true;
}

void FolderManager::changeNameMap( int oldindex,int newindex )
{
	if (oldindex==-1)
	{
		return;
	}

	if (oldindex==-2)  //-1 is the index of root or the item is new added into the tree
	{
		//new ,need added 
		//foldermap

		return;  //do nothing here
	}

	int foldername= m_oldermap.take(oldindex);  //delete old relationship
	m_oldermap.insert(newindex,foldername);	   //build new relationship		
}

QString FolderManager::buildFullPath( TreeItem *item )
{
	QString fullpath="";

	TreeItem *tmp=item;
	while (tmp->getParent()!=NULL)
	{
		fullpath.prepend(foundFolderName(tmp));
		fullpath.prepend("/");
		tmp=tmp->getParent();
	}
	fullpath.prepend(basename);
	return fullpath;
}

QString FolderManager::foundFolderName( TreeItem *item )
{
	int key=item->getIndex();
	return QString::number(m_oldermap.value(key));
}

void FolderManager::savingMapFile()
{
	//assume true
	saveFolderMapToFile(g_folderMapFileName);
}

void FolderManager::setTreeOpsManager( TreeOpsManager *pTreeOpsManager )
{
	m_pTreeOpsManager=pTreeOpsManager;
}

bool FolderManager::getPicsFileForDisplay( TreeItem *item,QStringList &pics,QStringList &names)
{
	int level=getDeepLevel(item);
	if (level<3 ||level>4)
	{		
		//out of range
		return false;
	}

	pics.clear();
	names.clear();

	if (level == 4)
	{
		QString temp;
//		QMessageBox::information(0,QString::number(item->childCount()),"Error");

		QString path=buildFullPath(item);
		if(getPicFromFolder(path,temp))
		{
//			QMessageBox::information(0,"","Error");
			pics.append(path+"/"+temp);
			names.append(temp);
		}
	
	}
	else if (level==3)
	{
		QString temp;
		for (int i=0;i<item->childCount();i++)
		{
			TreeItem *level2item=item->child(i);
			for (int j=0;j<level2item->childCount();j++)
			{
				TreeItem *level3item=level2item->child(j);
				QString path=buildFullPath(level3item);
				if(getPicFromFolder(path,temp))
				{
					pics.append(path+"/"+temp);
					names.append(temp);
				}
			}
		}
	}
	else
	{
		
	}
	
	if (pics.size()>0)
	{
		return true;
	}
	else
	{
		return false;
	}
		
}

bool FolderManager::getSTLFileForDisplay( TreeItem *item,QStringList &stls)
{
	if (getDeepLevel(item)!=5)  
	{
		return false;
	}
	stls.clear();
	QString path=buildFullPath(item);
	QDir dir(path);
	QStringList tmp=dir.entryList(QStringList()<<"*.stl", QDir::Files|QDir::NoDotAndDotDot);
	if (tmp.size()==0)
		return false;
	else
	{
		for (int i=0;i<tmp.size();i++)
		{
			stls.append(path+"/"+tmp.value(i));
		}

		return true;
	}
}

bool FolderManager::getPicFromFolder( QString foldername,QString &filename )
{
	QDir dir(foldername);
	QStringList tmplist=dir.entryList(QStringList()<<"*.jpg", QDir::Files|QDir::NoDotAndDotDot);
	if (tmplist.size()==0)
		return false;
	else
	{
		filename=tmplist.value(0);
		return true;
	}
}


int FolderManager::getDeepLevel(TreeItem *item)
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
