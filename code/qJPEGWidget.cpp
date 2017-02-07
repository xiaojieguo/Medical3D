#include "StdAfx.h"
#include "qJPEGWidget.h"

qJPEGWidget::qJPEGWidget(QWidget *parent)
	: QWidget(parent)
{
	m_bmax=false;
	m_pJPEGWidget=new JPEGDispalyWidget(this);
	m_pName=new QLabel(this);
	m_pName->setAlignment(Qt::AlignCenter);

	m_vlayout.addWidget(m_pJPEGWidget);
	m_vlayout.addWidget(m_pName);
	this->setLayout(&m_vlayout);
	this->setVisible(false);
	m_pName->setVisible(false);
	connect(m_pJPEGWidget,SIGNAL(changeSelfSize(bool)),this,SLOT(JPEGWidgetDoubleClicked(bool)));
}

qJPEGWidget::~qJPEGWidget()
{

}

void qJPEGWidget::setJPGFileName(QString jpgFileName)
{
	m_pJPEGWidget->setJPGFileName(jpgFileName);
}

void qJPEGWidget::display()
{
	m_pJPEGWidget->display();
}

void qJPEGWidget::setName( QString name )
{
	m_pName->setText(name);
}

void qJPEGWidget::clearWidget()
{
	m_pName->setText("");
	m_pJPEGWidget->clearPic();
}

//void qJPEGWidget::mouseDoubleClickEvent( QMouseEvent *event )
//{
//	emit changeSelfSize(this,!m_bmax);
//	m_bmax=!m_bmax;
//	QWidget::mouseDoubleClickEvent(event);
//}

void qJPEGWidget::setVisible( bool visible )
{
	m_pName->setVisible(visible);
	m_pJPEGWidget->setVisible(visible);
	QWidget::setVisible(visible);
}

void qJPEGWidget::JPEGWidgetDoubleClicked(bool bmax )
{
	emit changeSelfSize(this,bmax);
}