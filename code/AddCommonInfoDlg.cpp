#include "StdAfx.h"
#include "AddCommonInfoDlg.h"

AddCommonInfoDlg::AddCommonInfoDlg(QString userlabel,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	connect(ui.cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));

	ui.userDefinelabel->setText(userlabel); 
}

AddCommonInfoDlg::~AddCommonInfoDlg()
{

}

QString AddCommonInfoDlg::getName() const
{
	return ui.nameTextEdit->toPlainText();
}

#define MAXINPUT 500
void AddCommonInfoDlg::accept()
{
	//save color map file
	if (ui.nameTextEdit->toPlainText()=="")
	{
		QMessageBox::warning(this,"Notice","Name can't null£¡");
		return;
	}

	if (ui.nameTextEdit->toPlainText().size()>=MAXINPUT*2)
	{
		QMessageBox::warning(this,"Notice",QString("Name can't be more than %1 characters.").arg(MAXINPUT));
		return;
	}

	QDialog::accept();
}