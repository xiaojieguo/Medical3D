#include "StdAfx.h"
#include "TreeItem.h"

TreeItem::TreeItem(UserData &data, TreeItem *parent)
{
	m_userdata = data;
    m_parentItem = parent;
   
	m_index=-2;  //means this m_index is not updated
}
TreeItem::TreeItem(QString name,TreeItem *parent,QString define)
{
	memset((void  *)&m_userdata,0,sizeof(UserData));

	strcpy(m_userdata.name,name.toAscii().data());
	m_userdata.name[USERDATASIZE]=0;

	m_parentItem = parent;
	m_index=-2;  //m_index is initialized as -2
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

TreeItem *TreeItem::child(int number)
{
    return m_childItems.value(number);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

UserData & TreeItem::data()
{
	return m_userdata;
}

TreeItem *TreeItem::getParent()
{
    return m_parentItem;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

bool TreeItem::removeChildren( TreeItem * &child)
{
	if (child==NULL)
		return false;

	if(m_childItems.removeOne(child))
	{
		delete child;
		return true;
	}
	else
	{
		return false;
	}
}

bool TreeItem::setData(const UserData &data )
{
	m_userdata=data;
	return true;
}

void TreeItem::addChildren(TreeItem *child)
{
	if (NULL==child)
		return;
	
	m_childItems.append(child);
	child->setParent(this);
}

void TreeItem::setParent( TreeItem *parent )
{
	m_parentItem=parent;
}

int TreeItem::getIndex() const
{
	return m_index;
}

void TreeItem::setIndex(int index)
{
	m_index=index;
}