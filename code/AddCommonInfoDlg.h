#ifndef ADDCOMMONINFODLG_H
#define ADDCOMMONINFODLG_H

#include <QDialog>
#include "ui_AddCommonInfoDlg.h"

class AddCommonInfoDlg : public QDialog
{
	Q_OBJECT

public:
	AddCommonInfoDlg(QString userlabel,QWidget *parent = 0);
	~AddCommonInfoDlg();

	QString getName() const;

public slots:
	virtual void accept();

public:
	Ui::AddCommonInfoDlg ui;
};

#endif // ADDCOMMONINFODLG_H
