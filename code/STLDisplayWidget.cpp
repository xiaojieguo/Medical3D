#include "StdAfx.h"
#include "STLDisplayWidget.h"

STLDisplayWidget::STLDisplayWidget(QWidget *parent)
	: QVTKWidget(parent)
{
	this->GetRenderWindow()->StereoCapableWindowOn();
	this->SetUseTDx(true);

	QVTKInteractor *iren=this->GetInteractor();
	vtkInteractorStyle *s=
		static_cast<vtkInteractorStyle *>(iren->GetInteractorStyle());
	vtkTDxInteractorStyleCamera *t=
		static_cast<vtkTDxInteractorStyleCamera *>(s->GetTDxStyle());

	m_angleSensitivity=0.02;
	m_translationSensitivity=0.001;

	t->GetSettings()->SetAngleSensitivity(m_angleSensitivity);
	t->GetSettings()->SetTranslationXSensitivity(m_translationSensitivity);
	t->GetSettings()->SetTranslationYSensitivity(m_translationSensitivity);
	t->GetSettings()->SetTranslationZSensitivity(m_translationSensitivity);

	m_ren = vtkRenderer::New();
	this->GetRenderWindow()->AddRenderer(m_ren);

	m_ren->SetBackground(0.56 ,0.541,1);
}

STLDisplayWidget::~STLDisplayWidget()
{
	m_ren->Delete();
}

void STLDisplayWidget::setFiles( QStringList stlFiles )
{
	m_stlFiles.clear();
	m_stlFiles=stlFiles;
}

void STLDisplayWidget::display()
{
	m_vtkActors.clear();
	m_dataMappers.clear();
	m_stlReaders.clear();

	for (int i=0;i<m_stlFiles.size();i++)
	{
		vtkSTLReader *sr=vtkSTLReader::New();
		sr->SetFileName(m_stlFiles.value(i).toAscii().data());
		m_stlReaders.append(sr);

		vtkPolyDataMapper *stlMapper=vtkPolyDataMapper::New();
		stlMapper->SetInputConnection(sr->GetOutputPort());
		m_dataMappers.append(stlMapper);

		vtkLODActor *stlActor=vtkLODActor::New();
		stlActor->SetMapper(stlMapper);
		if (m_bColor)
		{
			ColorInfo curColor=m_colorArray.value(i);
			stlActor->GetProperty()->SetColor(curColor.r/255.0,curColor.g/255.0,curColor.b/255.0); //set colors
		}
		m_vtkActors.append(stlActor);
	}

	for(int i=0;i<m_vtkActors.size();i++)
	{
		m_ren->AddViewProp(m_vtkActors.value(i));
	}

	m_ren-> ResetCamera();
	m_ren-> GetActiveCamera()->Zoom(1);
	//
	m_ren->GetActiveCamera()->SetViewUp(0,0.21,0.97); 

	m_ren->GetActiveCamera()->SetPosition(200,950,-400);
	m_ren->GetActiveCamera()->SetDistance(750 );

	for (int i=0;i<m_stlFiles.size();i++)
	{
		m_vtkActors.value(i)->Delete();
		m_dataMappers.value(i)->Delete();
		m_stlReaders.value(i)->Delete();
	}
	this->show();
	this->update();
}

void STLDisplayWidget::clear3D()
{
	m_ren->Clear();
	m_ren->RemoveAllViewProps();
	this->update();
}

void STLDisplayWidget::enableColor( bool b )
{
	m_bColor=b;
}

void STLDisplayWidget::setColors( ColorArray carray )
{
	m_colorArray.clear();
	m_colorArray=carray;
}

void STLDisplayWidget::xxx()
{
	double distance=m_ren->GetActiveCamera()->GetDistance();
	double pos[3];
	m_ren->GetActiveCamera()->GetPosition(pos);
	double view[3];
	m_ren->GetActiveCamera()->GetViewUp(view);
}