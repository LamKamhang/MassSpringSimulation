#pragma once

#include "../System/simulation.h"
#include "SpringLine.h"
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


class SystemTimerCallback : public vtkCommand
{
public:
	static SystemTimerCallback *New();


	virtual void Execute(vtkObject * caller, unsigned long vtkNotUsed(eventId),
						void * vtkNotUsed(callData));

	void setSystem(const std::shared_ptr<NetSystem> &system);

	void add_line(SpringLine *ptr);

private:
	std::shared_ptr<NetSystem> _system;
	std::vector<std::shared_ptr<SpringLine> > _line_vec;
};

inline SystemTimerCallback* SystemTimerCallback::New()
{
	return new SystemTimerCallback;
}

inline void SystemTimerCallback::setSystem(const std::shared_ptr<NetSystem> &system)
{
	_system = system;
}

inline void SystemTimerCallback::add_line(SpringLine *ptr)
{
	_line_vec.emplace_back(ptr);
}