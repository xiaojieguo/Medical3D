#include "StdAfx.h"
#include "SimplePic.h"

SimplePic::SimplePic(QWidget *parent)
	: QWidget(parent)
{
	m_vlayout=new QVBoxLayout(this);
	m_vlayout->setContentsMargins(1,1,1,1);
	m_img=new QLabel(this);
//	m_name=new QLabel(this);
	QFont nameFont("Times New Roman", 10);
	m_img->setAlignment(Qt::AlignCenter);
//	m_name->setAlignment(Qt::AlignCenter);

//	m_name->setMaximumHeight(20);
//	m_name->setFont(nameFont );
	m_vlayout->addWidget(m_img);
//	m_vlayout->addWidget(m_name);
	//this->setAutoFillBackground(true);
	//this->setPaletteBackgroundColor(QColor(143,138,255));
	this->setStyleSheet("background-color:rgb(143,138,255);");
}

SimplePic::~SimplePic()
{

}

void SimplePic::setPic( QString filename)
{
	m_img->clear();

	QImage picture(filename);
	
	QImage scaledpic=picture.scaled(QSize(400,400),Qt::KeepAspectRatio);
	m_img->setPixmap(QPixmap::fromImage(scaledpic));
}

void SimplePic::setName( QString str )
{
	QString strNoendl=str.replace('\n','  ');
// 	if (strNoendl.length()>=30)
// 	{
// //		m_name->setText(strNoendl.left(25)+"...");
// 	}
//	else
//		m_name->setText(strNoendl);
}