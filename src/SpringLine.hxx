#pragma once


#include "simulation.hxx"
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
	SpringLine(double x1, double y1, double z1, double x2, double y2, double z2);
	SpringLine(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b);
	vtkSmartPointer<vtkActor> getActor() const;
	void update(double x1, double y1, double z1, double x2, double y2, double z2);
private:
	void _init(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b);
private:
	vtkSmartPointer<vtkActor> _actor;
public:
	vtkSmartPointer<vtkLineSource> line_src;
	unsigned lid, rid;
};

SpringLine::SpringLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
	srand(time(NULL)*(1+x1*32+x2*16+y1*8+y2*4+z1*2+z2));
	_init(x1, y1, z1, x2, y2, z2, ((rand()%100) / 100.0), ((rand()%100) / 100.0), ((rand()%100) / 100.0));
}

SpringLine::SpringLine(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b)
{
	_init(x1, y1, z1, x2, y2, z2, r, g, b);
}

void SpringLine::update(double x1, double y1, double z1, double x2, double y2, double z2)
{
	line_src->SetPoint1(x1, y1, z1);
	line_src->SetPoint2(x2, y2, z2);
	line_src->Update();
}

void SpringLine::_init(double x1, double y1, double z1, double x2, double y2, double z2, double r, double g, double b)
{
	line_src = vtkSmartPointer<vtkLineSource>::New();
	line_src->SetResolution(10);
	line_src->SetPoint1(x1, y1, z1);
	line_src->SetPoint2(x2, y2, z2);
	line_src->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(line_src->GetOutputPort());

	_actor = vtkSmartPointer<vtkActor>::New();
	_actor->SetMapper(mapper);
	_actor->GetProperty()->SetLineWidth(5);
	_actor->GetProperty()->SetColor(r, g, b);
}

vtkSmartPointer<vtkActor> SpringLine::getActor() const
{
	return _actor;
}