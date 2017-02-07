#ifndef STLDISPLAYWIDGET_H
#define STLDISPLAYWIDGET_H

class STLDisplayWidget : public QVTKWidget
{
	Q_OBJECT

public:
	STLDisplayWidget(QWidget *parent = NULL);
	~STLDisplayWidget();

	void setFiles(QStringList stlFiles);
	void setColors(ColorArray carray);
	void display();
	void clear3D();
	void enableColor(bool b);

private slots:
	void xxx();
	
protected:
	vtkRenderer* m_ren;

private:
	double m_angleSensitivity;
	double m_translationSensitivity;
	QVector<vtkSTLReader *> m_stlReaders;
	QVector<vtkPolyDataMapper *> m_dataMappers;
	QVector<vtkLODActor *> m_vtkActors;
	QStringList m_stlFiles;
	ColorArray m_colorArray;
	bool m_bColor; 
};

#endif // STLDISPLAYWIDGET_H