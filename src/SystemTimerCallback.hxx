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

#include "SpringLine.hxx"

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