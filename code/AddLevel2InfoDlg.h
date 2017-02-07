#ifndef ADDLEVEL2INFODLG_H
#define ADDLEVEL2INFODLG_H

#include "ui_AddLevel2InfoDlg.h"

class AddLevel2InfoDlg : public QDialog
{
	Q_OBJECT

public:
	AddLevel2InfoDlg(QWidget *parent = 0);
	~AddLevel2InfoDlg();

	void setFolderName(QString foldname);

	bool saveColorInfoToFile(QString desFolderName, QString filename);

	static bool loadColorInfoFromFile(QString desFolderName,QVector<ColorInfo> &carray ,QStringList &stls);

	bool copyFileToDesFolder(QString desFolderName);

	void restoreSetting(QString desFolderName);
	QString getPicFileName();
	QStringList getSTLFileNames();
	QString getName() const;
	
private:
	bool _copyPic(QString des);
	bool _copySTL(QString des);
	bool _copyFile(QString src,QString des);

	bool _deleteFileOnThisFolder(QString filename);

public slots:
	virtual void accept();

private slots:
	void addPic();
	void addSTLs();
	void showColorDlg(QTreeWidgetItem *item, int column);

	void deletePicItem();
	void showContextMenuOfPic(const QPoint &pos);
	void deleteSTLItem();
	void showContextMenuOfSTL(const QPoint &pos);

public:
	Ui::AddLevel2InfoDlg m_ui;

private:
	QString	m_folderName;
	QString m_picName;
	QStringList m_stlNameList;

	QMap<QString,ColorInfo> m_colormap;

	QMenu *m_menuPic;
	QMenu *m_menuStl;
};

#endif // ADDLEVEL2INFODLG_H