#ifndef QJPEGWIDGET_H
#define QJPEGWIDGET_H

class qJPEGWidget : public QWidget
{
	Q_OBJECT

public:
	qJPEGWidget(QWidget *parent=NULL);
	~qJPEGWidget();
	void setJPGFileName(QString jpgFileName);
	void display();
	void setName(QString name);
	void clearWidget();

private:
	JPEGDispalyWidget *m_pJPEGWidget;
	QLabel *m_pName;
	QVBoxLayout m_vlayout;
	bool m_bmax;
signals:
	void changeSelfSize(QWidget *_this,bool bmax); //if max = true ,hide the others widgets, if max=false, display all widgets. 
protected:
	//virtual void mouseDoubleClickEvent(QMouseEvent *event);
public slots:
	virtual void setVisible(bool visible);

private slots:
	void JPEGWidgetDoubleClicked(bool bmax);
};

#endif // QJPEGWIDGET_H