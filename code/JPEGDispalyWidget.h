#ifndef JPEGDISPALYWIDGET_H
#define JPEGDISPALYWIDGET_H

class JPEGDispalyWidget : public QVTKWidget
{
	Q_OBJECT

public:
	JPEGDispalyWidget(QWidget *parent=NULL);
	~JPEGDispalyWidget();
	void setJPGFileName(QString jpgFileName);
	void display();
	void clearPic();

private:
	QString m_jpgFileName;
	QVTKInteractor *iren;
	bool m_bmax;
signals:
	void changeSelfSize(bool bmax);
	void justShowMe(QWidget *self,bool bmax);

protected:
	vtkRenderer* Ren1;
	virtual void mouseDoubleClickEvent(QMouseEvent * event);
};

#endif // JPEGDISPALYWIDGET_H
