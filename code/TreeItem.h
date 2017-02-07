#ifndef TREEITEM_H
#define TREEITEM_H

// There is only one column, so I have disable all the column operations
class TreeItem
{
public:
	TreeItem(UserData &data, TreeItem *parent = 0);
	TreeItem(QString name,TreeItem *parent=0,QString define="");
	~TreeItem();

	TreeItem *child(int number);
	int childCount() const;

	void addChildren(TreeItem *child);

	TreeItem *getParent();
	bool removeChildren(int position, int count);
	bool removeChildren( TreeItem * &child);
	int childNumber() const;

	bool setData(const UserData &data);
	void setParent(TreeItem *parent);
	UserData &data();

	int getIndex() const;
	void setIndex(int index);

private:
	QList<TreeItem*> m_childItems;
	UserData m_userdata;
	TreeItem *m_parentItem;
	int m_index;
};

#endif