#pragma once

#include "../System/simulation.h"
#include <eigen3/Eigen/Core>

#include <memory>
#include <ctime>
#include <cstdlib>

#include <vtkActor.h>
#include <vtkMath.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCallbackCommand.h>

class SpringLine
{
public:
	SpringLine(double x1, double y1, double z1, double x2, double y2, double z2)
	{srand(time(NULL)*(1+x1*32+x2*16+y1*8+y2*4+z1*2+z2));
	_init(x1, y1, z1, x2, y2, z2, ((rand()%100) / 100.0), ((rand()%100) / 100.0), ((rand()%100) / 100.0));}

	SpringLine(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b)
	{_init(x1, y1, z1, x2, y2, z2, r, g, b);}

	void update(double x1, double y1, double z1, double x2, double y2, double z2);

private:
	void _init(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b);

public:
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkLineSource> line_src;
	unsigned lid, rid;
};

inline void SpringLine::update(double x1, double y1, double z1, double x2, double y2, double z2)
{
	line_src->SetPoint1(x1, y1, z1);
	line_src->SetPoint2(x2, y2, z2);
	line_src->Update();
}

inline void SpringLine::_init(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b)
{
	line_src = vtkSmartPointer<vtkLineSource>::New();
	line_src->SetResolution(10);
	line_src->SetPoint1(x1, y1, z1);
	line_src->SetPoint2(x2, y2, z2);
	line_src->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(line_src->GetOutputPort());

	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetLineWidth(5);
	actor->GetProperty()->SetColor(r, g, b);
}