#include "SystemTimerCallback.h"

void SystemTimerCallback::Execute(vtkObject * caller, unsigned long vtkNotUsed(eventId),
					void * vtkNotUsed(callData))
{
	_system->simulate();
	_system->render();
	
	auto xt = _system->get_x();
	auto spring_vec = _system->get_spring_vec();

	for (auto line: _line_vec)
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