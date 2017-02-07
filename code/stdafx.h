// cpp
#include <stdio.h>

// qt
#include <QtGui>

// vtk
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkCommand.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkConeSource.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkInteractorStyle.h"
#include "vtkTDxInteractorStyleCamera.h"
#include "vtkTDxInteractorStyleSettings.h"
#include "vtkSTLReader.h"
#include "vtkLODActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPNGReader.h"
#include "vtkJPEGReader.h"
#include "vtkImageActor.h"
#include "QVTKWidget.h"
#include "vtkInteractorStyleImage.h"

// my files
#include "SimplePic.h"
#include "PicGroup.h"
#include "JPEGDispalyWidget.h"
#include "qJPEGWidget.h"
#include "MedicalCommon.h"
#include "TreeItem.h"
#include "FolderManager.h"
#include "TreeOpsManager.h"
#include "VisualManager.h"
#include "AddCommonInfoDlg.h"
#include "AddLevel2InfoDlg.h"
#include "MedicalDbDlg.h"
#include "STLDisplayWidget.h"
#include "DisplayManager.h"
#include "qAboutDlg.h"
#include "Medical3D.h"