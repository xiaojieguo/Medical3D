#ifndef SIMPLEPIC_H
#define SIMPLEPIC_H

class SimplePic : public QWidget
{
	Q_OBJECT

public:
	SimplePic(QWidget *parent=NULL);
	~SimplePic();

	void setPic(QString filename);
	void setName(QString str);

private:
	QLabel *m_img;
//	QLabel *m_name;
	QVBoxLayout *m_vlayout;
};

#endif // SIMPLEPIC_H