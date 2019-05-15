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
	vtkSmartPointer<vtkActor> getActor() const;
	void update(double x1, double y1, double z1, double x2, double y2, double z2);
private:

private:
	vtkSmartPointer<vtkActor> _actor;
public:
	vtkSmartPointer<vtkLineSource> line_src;
	unsigned lid, rid;
};

class SystemTimerCallback : public vtkCommand
{
public:
	static SystemTimerCallback *New()
	{
		SystemTimerCallback *cb = new SystemTimerCallback;
		return cb;
	}

	virtual void Execute(vtkObject * caller, unsigned long vtkNotUsed(eventId),
						void * vtkNotUsed(callData))
	{
		_system->simulate();
		_system->render();
		
		auto xt = _system->get_x();
		auto spring_vec = _system->get_spring_vec();

		for (auto line: lines_vec)
		{
			unsigned lid = line->lid;
			unsigned rid = line->rid;

			line->line_src->SetPoint1(xt(3*lid), xt(3*lid+1), xt(3*lid+2));
			line->line_src->SetPoint2(xt(3*rid), xt(3*rid+1), xt(3*rid+2));
			line->line_src->Update();
		}
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
      	iren->GetRenderWindow()->Render();
	}

	void setSystem(const std::shared_ptr<NetSystem> &system)
	{
		_system = system;
	}

	void add_line(SpringLine *ptr)
	{
		lines_vec.emplace_back(ptr);
	}

private:
	std::shared_ptr<NetSystem> _system;
	std::vector<std::shared_ptr<SpringLine> > lines_vec;
};

class Renderer
{
public:
	Renderer();
	void AddTimerEvent(const std::shared_ptr<NetSystem> &system);

	void render();
private:

private:
	vtkSmartPointer<vtkNamedColors> colors;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
};

Renderer::Renderer()
{
	colors = vtkSmartPointer<vtkNamedColors>::New();

	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Mass Springs System");
	renderWindow->SetSize(600, 800);
	
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Initialize must be called prior to creating timer events.
	renderWindowInteractor->Initialize();
	renderWindowInteractor->CreateRepeatingTimer(100);
}

void Renderer::AddTimerEvent(const std::shared_ptr<NetSystem> &system)
{
	auto cb = vtkSmartPointer<SystemTimerCallback>::New();
	cb->setSystem(system);

	auto xt = system->get_x();
	auto spring_vec = system->get_spring_vec();

	for (auto spring: spring_vec)
	{
		unsigned lid = spring->getLID();
		unsigned rid = spring->getRID();

		SpringLine *line = new SpringLine(xt(3*lid), xt(3*lid+1), xt(3*lid+2), xt(3*rid), xt(3*rid+1), xt(3*rid+2));
		line->lid = lid;
		line->rid = rid;
		renderer->AddActor(line->getActor());
		cb->add_line(line);
	}

	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
}

void Renderer::render()
{
	renderWindowInteractor->Start();
	renderWindowInteractor->Render();
}

SpringLine::SpringLine(double x1, double y1, double z1, double x2, double y2, double z2)
{
	line_src = vtkSmartPointer<vtkLineSource>::New();
	line_src->SetResolution(20);
	line_src->SetPoint1(x1, y1, z1);
	line_src->SetPoint2(x2, y2, z2);
	line_src->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(line_src->GetOutputPort());

	_actor = vtkSmartPointer<vtkActor>::New();
	_actor->SetMapper(mapper);
	_actor->GetProperty()->SetLineWidth(20);
	srand(time(NULL)*(1+x1*32+x2*16+y1*8+y2*4+z1*2+z2));
	_actor->GetProperty()->SetColor(((rand()%100) / 100.0), ((rand()%100) / 100.0), ((rand()%100) / 100.0));
}

void SpringLine::update(double x1, double y1, double z1, double x2, double y2, double z2)
{
	line_src->SetPoint1(x1, y1, z1);
	line_src->SetPoint2(x2, y2, z2);
	line_src->Update();
}

vtkSmartPointer<vtkActor> SpringLine::getActor() const
{
	return _actor;
}