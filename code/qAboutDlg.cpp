#include "StdAfx.h"
#include "qAboutDlg.h"

qAboutDlg::qAboutDlg(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
}

qAboutDlg::~qAboutDlg()
{

}

void qAboutDlg::on_pushButton_clicked()
{
	this->close();
}