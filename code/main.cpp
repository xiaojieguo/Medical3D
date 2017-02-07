#include "stdafx.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/Medical3D/Icons/app.jpg"));

//	QApplication::setStyle("windowsxp");
	QApplication::addLibraryPath("./plugins");

	Medical3D w;
	w.show();

	return a.exec();
}