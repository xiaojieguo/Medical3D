#include "StdAfx.h"
#include "AddLevel2InfoDlg.h"

AddLevel2InfoDlg::AddLevel2InfoDlg(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.STLsList->setContextMenuPolicy(Qt::CustomContextMenu); 
	m_ui.picList->setContextMenuPolicy(Qt::CustomContextMenu); 

	m_menuPic=NULL;
	m_menuStl=NULL;

	connect(m_ui.okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	connect(m_ui.cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	connect(m_ui.addPicBtn,SIGNAL(clicked()),this,SLOT(addPic()));
	connect(m_ui.addSTLsBtn,SIGNAL(clicked()),this,SLOT(addSTLs()));

	connect(m_ui.STLsList,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
		this,SLOT(showColorDlg(QTreeWidgetItem *, int)));
	connect(m_ui.STLsList,SIGNAL(customContextMenuRequested(const QPoint&)), 
		this, SLOT(showContextMenuOfSTL(const QPoint&)));
	connect(m_ui.picList,SIGNAL( customContextMenuRequested(const QPoint&)), 
		this, SLOT(showContextMenuOfPic(const QPoint&)));
}

void AddLevel2InfoDlg::showContextMenuOfSTL(const QPoint &pos)
{
	if(m_menuStl)
	{
		delete m_menuStl;
		m_menuStl = NULL;
	}
	m_menuStl = new QMenu(this);
	QAction *deleteAction = m_menuStl->addAction(
		QIcon(":/Medical3D/Icons/delete.png"),"Delete STL file");

	connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteSTLItem()));
	m_menuStl->exec(QCursor::pos());
}

void AddLevel2InfoDlg::deleteSTLItem()
{
	if (m_ui.STLsList->currentItem()==NULL)
	{
		return;
	}

	int row=m_ui.STLsList->indexOfTopLevelItem(m_ui.STLsList->currentItem());
	//remove in m_stlNameList
	QString filename=m_stlNameList.takeAt(row);
	if(':'==filename.at(0)) //file in the folder
	{
		filename=filename.remove (0,1);
		_deleteFileOnThisFolder(filename);  //delete this file
	}
	//remove in the color map
	QMap<QString,ColorInfo>::iterator iter = m_colormap.find(filename);
	if (iter!=m_colormap.end())
	{
		//yeah ,have found the map info
		m_colormap.remove(filename);
	}

	//save color map file
	saveColorInfoToFile(m_folderName,"info.col");

	//update the UI
	m_ui.STLsList->clear();

	for (int i=0;i<m_stlNameList.size();i++)
	{
		QTreeWidgetItem *newitem=new QTreeWidgetItem(m_ui.STLsList);
		//QString filename=QDir::convertSeparators(m_stlNameList.value(i)).split("\\").last();
		newitem->setText(0,m_stlNameList.value(i));
		ColorInfo curColor;
		QMap<QString,ColorInfo>::iterator iter = m_colormap.find(m_stlNameList.value(i).remove(0,1));
		if (iter==m_colormap.end())
		{
			//not found the color information, indicate this is a new item
			curColor.r=255;
			curColor.g=255;
			curColor.b=255;
		}	
		else
			curColor=iter.value();   
		QPixmap pix(16, 14);
		QColor col(curColor.r,curColor.g,curColor.b);
		pix.fill(col);
		newitem->setIcon(1,QIcon(pix));
		newitem->setText(1,QString("  R=%1,G=%2,B=%3").arg(curColor.r).arg(curColor.g).arg(curColor.b));
	}
}

void AddLevel2InfoDlg::showContextMenuOfPic(const QPoint &pos)
{
	if(m_menuPic)//
	{
		delete m_menuPic;
		m_menuPic = NULL;
	}
	m_menuPic = new QMenu(this);
	QAction *deleteAction = m_menuPic->addAction(
		QIcon(":/Medical3D/Icons/delete.png"),"Delete image file");

	connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deletePicItem()));
	m_menuPic->exec(QCursor::pos());//
}

void AddLevel2InfoDlg::deletePicItem()
{
	if (m_ui.picList->currentItem()==NULL)
	{
		return;
	}

	if(':'==m_picName.at(0)) //file in the folder
	{
		_deleteFileOnThisFolder(m_picName.remove(0,1));  //delete this file
	}
	m_picName="";
	m_ui.picList->clear();
}

AddLevel2InfoDlg::~AddLevel2InfoDlg()
{

}

QString AddLevel2InfoDlg::getPicFileName()
{
	return m_picName;
}

QStringList AddLevel2InfoDlg::getSTLFileNames()
{
	return m_stlNameList;
}

void AddLevel2InfoDlg::addPic()
{
	m_picName=QFileDialog::getOpenFileName(this, tr("Open File"),
		QDir::currentPath(),
		tr("Images (*.png *.jpg)"));
	m_ui.picList->clear();
	m_ui.picList->addItem(m_picName);
}

void AddLevel2InfoDlg::addSTLs()
{
	//Just clear the map for add new elements
	//m_colormap.clear();

	QStringList tmpStls=QFileDialog::getOpenFileNames(this,
		"Select one or more files to open",
		QDir::currentPath(),
		tr("Images (*.stl)"));
	if (tmpStls.size()<=0)
		return ; // do nothing

	// must ensure no same file copy again
	for (int i=0;i<tmpStls.size();i++)
	{
		QString curStl=QDir::convertSeparators(tmpStls.value(i)).split("\\").last();
		curStl=curStl.prepend(':');
		if ( m_stlNameList.contains(curStl ) )
		{
			//already in the folder do nothing , so don't append
			continue;
		}
		else
			m_stlNameList.append(tmpStls.value(i));
	}

	m_ui.STLsList->clear();
	for (int i=0;i<m_stlNameList.size();i++)
	{
		QTreeWidgetItem *newitem=new QTreeWidgetItem(m_ui.STLsList);
		QString filename=QDir::convertSeparators(m_stlNameList.value(i)).split("\\").last();
		//newitem->setText(0,filename);
		newitem->setText(0, m_stlNameList.value(i));

		QMap<QString,ColorInfo>::iterator iter = m_colormap.find(m_stlNameList.value(i).remove(0,1));
		if (iter==m_colormap.end())
		{
			//not found the color information, indicate this is a new item
			ColorInfo info;
			info.r=255;
			info.g=255;
			info.b=255;
			m_colormap.insert(filename,info);
		}	
		else
		{
			ColorInfo curColor=iter.value();   
			QPixmap pix(16, 14);
			QColor col(curColor.r,curColor.g,curColor.b);
			pix.fill(col);
			newitem->setIcon(1,QIcon(pix));
			newitem->setText(1,QString("  R=%1,G=%2,B=%3").arg(curColor.r).arg(curColor.g).arg(curColor.b));
		}
	}
}

QString AddLevel2InfoDlg::getName() const
{
	return m_ui.nameTextEdit->toPlainText();
}

bool AddLevel2InfoDlg::copyFileToDesFolder( QString desFolderName )
{
	QString message;
	int steps=0;
	
	const int TotalSteps=1+m_stlNameList.size()+1;
	QProgressDialog progress("Copye files", "Abort Copy", 0, TotalSteps, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setAutoClose(true);

	bool error=false;
	if(!_copyPic(desFolderName))
	{
		message+=tr("Copy files error! The file isn't selected or has been here.\n");
		error=true;

	}
	steps+=1;
	progress.setValue(steps);

	if (!_copySTL(desFolderName) )
	{
		message+=tr("Copy STL file error! The STL file isn't selected or has been here.\n");
		error=true;
	}
	else
	{
		if(!saveColorInfoToFile(desFolderName,"info.col"))
			message+=tr("Write file error!\n");
	}

	steps+=m_stlNameList.size();
	progress.setValue(steps);

	if (error)
	{
		//QMessageBox::warning(this,"warning",message);
		return false;
	}
	progress.setValue(TotalSteps);
	return true;
}

bool AddLevel2InfoDlg::_copyPic( QString des )
{
	if (':'==m_picName[0])
		return true;

	return _copyFile(m_picName,des);
}

bool AddLevel2InfoDlg::_copySTL( QString des )
{
	bool res=true;
	for (int i=0;i<m_stlNameList.size();i++)
	{
		QString curfile=m_stlNameList.value(i);
		if (":"==curfile.at(0))
		{
			continue;
		}

		if(!_copyFile(curfile,des))
		{
			res=false;
			continue;
		}
	}
	//no matter what happened, just return true to ensure the color map file was updated.
	return true;
}

bool AddLevel2InfoDlg::_copyFile( QString src,QString des )
{
	QString filename=QDir::convertSeparators(src).split("\\").last();
	QString newfilepath=des+"/"+filename;
	return QFile::copy(src,newfilepath);
}

void AddLevel2InfoDlg::showColorDlg(QTreeWidgetItem *item, int column)
{
	if(column==0)
		return;
	QColor col = QColorDialog::getColor(Qt::white, this);
	if (!col.isValid())
		return;

	int row=m_ui.STLsList->indexOfTopLevelItem(item);

	QString stlFile=m_stlNameList.value(row);
	QString curkey;
	if (':'==stlFile.at(0))
		curkey=stlFile.remove(0,1);  //in the local folder
	else
		curkey= QDir::convertSeparators(item->text(0)).split("\\").last();

	ColorInfo cinfo;
	col.getRgb(&cinfo.r,&cinfo.g,&cinfo.b);
	m_colormap.insert(curkey,cinfo);  //overwrite the old value

	QPixmap pix(16, 14);
	pix.fill(col);
	item->setIcon(1,QIcon(pix));
	item->setText(1,QString("  R=%1,G=%2,B=%3").arg(col.red()).arg(col.green()).arg(col.blue()));
}


#define MAXINPUT 500
void AddLevel2InfoDlg::accept()
{
	//save color map file
	if (m_ui.nameTextEdit->toPlainText()=="")
	{
		QMessageBox::warning(this,"Notice","Name can't be null!");
		return;
	}

	if (m_ui.nameTextEdit->toPlainText().size()>=MAXINPUT*2)
	{
		QMessageBox::warning(this,"Notice",QString("Name can't more than %1").arg(MAXINPUT));
		return;
	}

	QDialog::accept();
}

#define  BUFSIZE 256
bool AddLevel2InfoDlg::saveColorInfoToFile(QString desFolderName,QString filename)
{
	char buf[BUFSIZE];
	// char buf[BUFSIZE]  int r , int g, int b
	QFile file(desFolderName+"/"+filename);
	file.open(QIODevice::WriteOnly);

	QDataStream out(&file);

	QStringList  _keys=m_colormap.keys();
	if (_keys.size()<=0)
		return false;

	out << _keys.size();
	for (int i=0;i<_keys.size();i++)
	{
		memset((void *)buf,0,BUFSIZE);
		strcpy(buf,_keys.value(i).toAscii().data());
		buf[BUFSIZE-1]=0;
		out.writeRawData(buf,BUFSIZE);
//		out.writeBytes(buf,BUFSIZE);
		ColorInfo col=m_colormap.value(_keys.value(i));
		out.writeRawData((const char *)&col,sizeof(ColorInfo));
//		out.writeBytes((const char *)&col,sizeof(ColorInfo));
	}
	return true;
}

bool AddLevel2InfoDlg::loadColorInfoFromFile(QString desFolderName,QVector<ColorInfo> &carray ,QStringList &stls)
{
	QFile file(desFolderName+"/info.col");
	if(!file.open(QIODevice::ReadOnly))
		return false;

	QDataStream in(&file);

	int size;
	in >> size;
	if (size<=0)
	{
		return false;
	}
	//QMap<QString ,ColorInfo>  tmpcolormap;
	char buf[BUFSIZE];
	ColorInfo c;
	for (int i=0 ;i<size ;i++)
	{
		memset((void *)buf,0,BUFSIZE);
        int nbytesRead=in.readRawData(buf,BUFSIZE);
//		int nbytesRead=in.readBytes(buf,BUFSIZE);
		buf[BUFSIZE-1]=0;
		QString curfile=QString::fromAscii(buf);
//		in.readRawBytes((char *)&c,sizeof(ColorInfo));
        in.readRawData((char *)&c,sizeof(ColorInfo));
		//tmpcolormap
		carray.append(c);
		stls.append(curfile);
	}

// 	char *buf;
// 	buf = (char *) malloc(sizeof(char)*BUFSIZE);
// 	if (buf!=NULL)
// 	{
// 		
// 		if (buf!=NULL)
// 		{
// 			free(buf);
// 			buf = 0;
// 		}
// 	}
	
	return true;
}

void AddLevel2InfoDlg::restoreSetting(QString desFolderName)
{
	QDir dir(desFolderName);
	QStringList pics=dir.entryList(QStringList()<<"*.jpg", QDir::Files|QDir::NoDotAndDotDot);
	if (pics.size()>0)
	{
		m_picName=":"+pics.value(0);  //
		m_ui.picList->addItem(m_picName);// 
	}

	QStringList stls;
	QVector<ColorInfo> carray;
	if(loadColorInfoFromFile(desFolderName,carray,stls))
	{
		//
		m_colormap.clear();
		for (int i=0;i<carray.size();i++)
		{
			m_colormap.insert(stls.value(i),carray.value(i));
		}

		//display
		m_stlNameList.clear();
		for (int i=0;i<stls.size();i++)
			m_stlNameList.append(":"+stls.value(i));

		for (int i=0;i<stls.size();i++)
		{
			QTreeWidgetItem *newitem=new QTreeWidgetItem(m_ui.STLsList);
			//QString filename=QDir::convertSeparators(m_stlNameList.value(i)).split("\\").last();
			newitem->setText(0,m_stlNameList.value(i));
			ColorInfo curColor=carray.value(i);
			QPixmap pix(16, 14);
			QColor col(curColor.r,curColor.g,curColor.b);
			pix.fill(col);
			newitem->setIcon(1,QIcon(pix));
			newitem->setText(1,QString("  R=%1,G=%2,B=%3").arg(curColor.r).arg(curColor.g).arg(curColor.b));
		}
	}
}

void AddLevel2InfoDlg::setFolderName( QString foldname )
{
	m_folderName=foldname;
}

bool AddLevel2InfoDlg::_deleteFileOnThisFolder( QString filename )
{
	QDir dir(m_folderName);
	return  dir.remove(filename);
}