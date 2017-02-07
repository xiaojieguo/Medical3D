#include "StdAfx.h"
#include "JPEGDispalyWidget.h"

JPEGDispalyWidget::JPEGDispalyWidget(QWidget *parent)
	: QVTKWidget(parent)
{
	setMinimumWidth(400);
	m_bmax=false;

	vtkRenderWindow* renwin = vtkRenderWindow::New();
	renwin->StereoCapableWindowOn();
	this->SetUseTDx(true);
	this->SetRenderWindow(renwin);

	renwin->Delete();

	iren=this->GetInteractor();

	Ren1 = vtkRenderer::New();
	this->GetRenderWindow()->AddRenderer(Ren1);
	Ren1->SetBackground(0.56 ,0.541,1);
}

JPEGDispalyWidget::~JPEGDispalyWidget()
{
	Ren1->Delete();
}

void JPEGDispalyWidget::setJPGFileName( QString jpgFileName )
{
	m_jpgFileName=jpgFileName;
}

void JPEGDispalyWidget::display()
{
	if(m_jpgFileName=="")
		return;
	vtkJPEGReader *m_reader = vtkJPEGReader::New();
	m_reader->SetFileName(m_jpgFileName.toAscii().data());
	m_reader->Update();

	vtkImageActor *m_Actor = vtkImageActor::New();
	m_Actor->SetInput(m_reader->GetOutput());

	Ren1->AddActor(m_Actor);

	vtkInteractorStyleImage *interactor = vtkInteractorStyleImage::New();
	iren->SetInteractorStyle(interactor);

	Ren1-> ResetCamera();
	Ren1-> GetActiveCamera()->Zoom(1.0);

	interactor->Delete();
	m_Actor->Delete();
	m_reader->Delete();
	//this->resize(600,600);
	this->update();

}


void JPEGDispalyWidget::mouseDoubleClickEvent( QMouseEvent * event )
{

	//emit changeSelfSize(!m_bmax);
	emit justShowMe(this,!m_bmax);
	m_bmax=!m_bmax;
	QWidget::mouseDoubleClickEvent(event);
}

void JPEGDispalyWidget::clearPic()
{
	Ren1->Clear();
	Ren1->RemoveAllViewProps();
}