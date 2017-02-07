#include "StdAfx.h"
#include "PicGroup.h"

PicGroup::PicGroup(QWidget *parent)
	: QWidget(parent)
{
	m_phlayout=new QHBoxLayout(this);
	m_phlayout->setContentsMargins(1,1,1,1) ;
}

PicGroup::~PicGroup()
{
	clearLayout();
	clearPool();
}

bool PicGroup::display( QStringList picFilelist,QStringList namelist )
{
	clearLayout();
	clearPool();

	int size=picFilelist.size();
	if (size<=0)
		return false;

	if (picFilelist.size()!=namelist.size())
		return false;

	for (int i=0;i<size;i++)
	{
		SimplePic *newpic=new SimplePic();
		newpic->setPic(picFilelist.value(i));
		newpic->setName(namelist.value(i));
		m_picPool.append(newpic);
		m_phlayout->addWidget(newpic);
	}

	return true;
}

void PicGroup::clearPool()
{
	for (QVector<SimplePic *>::iterator i=m_picPool.begin(); i!=m_picPool.end();i++)
	{
		delete (*i);
	}
	m_picPool.clear();
}

void PicGroup::clearLayout()
{
	for (QVector<SimplePic *>::iterator i=m_picPool.begin(); i!=m_picPool.end();i++)
	{
		m_phlayout->removeWidget(*i);
	}
}