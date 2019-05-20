#pragma once

#include "../System/simulation.h"

#include <Eigen/Core>

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

class Renderer
{
public:
	Renderer(unsigned long duration = 10);
	void AddTimerEvent(const std::shared_ptr<NetSystem> &system);

	void render();
private:

private:
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
};


inline void Renderer::render()
{
	renderWindowInteractor->Start();
}