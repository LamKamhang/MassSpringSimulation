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

#include "SystemTimerCallback.hxx"


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
	renderWindow->SetSize(1200, 1600);
	
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Initialize must be called prior to creating timer events.
	renderWindowInteractor->Initialize();
	renderWindowInteractor->CreateRepeatingTimer(10);
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
}