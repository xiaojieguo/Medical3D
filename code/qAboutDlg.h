#ifndef QABOUTDLG_H
#define QABOUTDLG_H

#include "ui_qAboutDlg.h"

class qAboutDlg : public QDialog
{
	Q_OBJECT

public:
	qAboutDlg(QWidget *parent = 0);
	~qAboutDlg();

private slots:
	void on_pushButton_clicked();

private:
	Ui::qAboutDlg m_ui;
};

#endif // QABOUTDLG_H