#ifndef PICGROUP_H
#define PICGROUP_H

class PicGroup : public QWidget
{
	Q_OBJECT

public:
	PicGroup(QWidget *parent=NULL);
	~PicGroup();

	bool display(QStringList picFilelist,QStringList namelist);

private:
	void clearPool();
	void clearLayout();

private:
	QHBoxLayout *m_phlayout;
	QVector<SimplePic *> m_picPool;
};

#endif // PICGROUP_H