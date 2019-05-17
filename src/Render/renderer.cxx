#include "renderer.h"
#include "SystemTimerCallback.h"


Renderer::Renderer(unsigned long duration)
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
	// renderer->SetBackground(1,1,1);

	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Mass Springs System");
	renderWindow->SetSize(1200, 1600);
	
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Initialize must be called prior to creating timer events.
	renderWindowInteractor->Initialize();
	renderWindowInteractor->CreateRepeatingTimer(duration);
}

void Renderer::AddTimerEvent(const std::shared_ptr<NetSystem> &system)
{
	auto cb = vtkSmartPointer<SystemTimerCallback>::New();
	cb->setSystem(system);

	auto xt = system->get_x();
	auto spring_vec = system->get_spring_vec();

	for (auto spring: spring_vec)
	{
		unsigned lid = spring->get_lid();
		unsigned rid = spring->get_rid();

		SpringLine *line = new SpringLine(xt(3*lid), xt(3*lid+1), xt(3*lid+2), xt(3*rid), xt(3*rid+1), xt(3*rid+2));
		// SpringLine *line = new SpringLine(xt(3*lid), xt(3*lid+1), xt(3*lid+2), xt(3*rid), xt(3*rid+1), xt(3*rid+2), 0, 0, 0);
		line->lid = lid;
		line->rid = rid;
		renderer->AddActor(line->actor);
		cb->add_line(line);
	}

	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
}