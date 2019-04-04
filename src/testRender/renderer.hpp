#pragma once

#include <memory>
#include "../particle.hpp"
#include "../spring.hpp"
#include "../simulation.hpp"

#include <vtkActor.h>
#include <vtkCylinderSource.h>
#include <vtkMath.h>
#include <vtkMinimalStandardRandomSequence.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCallbackCommand.h>

#include "../vector.hpp"

#include <ctime>

inline vtkSmartPointer<vtkTransform> getTransform(mymath::Vector<double, 3> start, mymath::Vector<double, 3> end);

class MassTimerCallback : public vtkCommand
{
public:
	static MassTimerCallback *New()
	{
		MassTimerCallback *cb = new MassTimerCallback;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId,
						void * vtkNotUsed(callData))
	{
		actor->SetPosition(_mass->getPos().getItem(0), _mass->getPos().getItem(1), _mass->getPos().getItem(2));
		std::cout << "mass" << _mass->getPos() << std::endl;
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
		iren->GetRenderWindow()->Render();
	}

	void setMass(const std::shared_ptr<Particle> &mass)
	{
		_mass = mass;
	}

private:
	std::shared_ptr<Particle> _mass;
	int TimerCount;
public:
	vtkActor* actor;
};

class SpringTimerCallback : public vtkCommand
{
public:
	static SpringTimerCallback *New()
	{
		SpringTimerCallback *cb = new SpringTimerCallback;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject *caller, unsigned long eventId,
						void * vtkNotUsed(callData))
	{
		auto transform = getTransform(_spring->getLeft()->getPos(), _spring->getRight()->getPos());
		std::cout << "left:" << _spring->getLeft()->getPos() << std::endl;
		std::cout << "right:" << _spring->getRight()->getPos() << std::endl;
		actor->SetUserMatrix(transform->GetMatrix());
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
		iren->GetRenderWindow()->Render();
	}

	void setSpring(const std::shared_ptr<Spring> &spring)
	{
		_spring = spring;
	}

private:
	std::shared_ptr<Spring> _spring;
	int TimerCount;
public:
	vtkActor* actor;
};

class SystemTimerCallback : public vtkCommand
{
public:
	static SystemTimerCallback *New()
	{
		SystemTimerCallback *cb = new SystemTimerCallback;
		return cb;
	}

	virtual void Execute(vtkObject * vtkNotUsed(caller), unsigned long vtkNotUsed(eventId),
						void * vtkNotUsed(callData))
	{
		_system->update();
	}

	void setSystem(const std::shared_ptr<ChainSystem1D> &system)
	{
		_system = system;
	}

private:
	std::shared_ptr<ChainSystem1D> _system;
};

class Renderer
{
public:
	Renderer();
	void AddTimerEvent(const std::shared_ptr<ChainSystem1D> &system);
	void AddActor(vtkSmartPointer<vtkActor> p, const std::shared_ptr<Spring> &spring);
	void AddActor(vtkSmartPointer<vtkActor> p, const std::shared_ptr<Particle> &mass);
	void render();

private:

private:
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
};

Renderer::Renderer()
{
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renderer->SetBackground(0, 0, 0);

	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Mass Springs System");
	renderWindow->SetSize(600, 800);
	

	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Initialize must be called prior to creating timer events.
	renderWindowInteractor->Initialize();
	renderWindowInteractor->CreateRepeatingTimer(100);
}

void Renderer::AddTimerEvent(const std::shared_ptr<ChainSystem1D> &system)
{
	auto cb = vtkSmartPointer<SystemTimerCallback>::New();
	cb->setSystem(system);

	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
}

void Renderer::AddActor(vtkSmartPointer<vtkActor> p, const std::shared_ptr<Spring> &spring)
{
	renderer->AddActor(p);

	auto cb = vtkSmartPointer<SpringTimerCallback>::New();
	cb->setSpring(spring);
	cb->actor = p;

	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
}

void Renderer::AddActor(vtkSmartPointer<vtkActor> p, const std::shared_ptr<Particle> &mass)
{
	renderer->AddActor(p);
	
	auto cb = vtkSmartPointer<MassTimerCallback>::New();
	cb->setMass(mass);
	cb->actor = p;
	// renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
}

void Renderer::render()
{
	renderWindowInteractor->Start();
	renderWindowInteractor->Render();
}

class Sphere
{
public:
	Sphere(mymath::Vector<double, 3> pos, double radius = 0.5);
	vtkSmartPointer<vtkActor> getData() const;
private:

private:
	vtkSmartPointer<vtkActor> sphere;
};

Sphere::Sphere(mymath::Vector<double, 3> pos, double radius)
{
	auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(pos.getItem(0), pos.getItem(1), pos.getItem(2));
	sphereSource->SetRadius(radius);

	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	sphere = vtkSmartPointer<vtkActor>::New();
	sphere->SetMapper(mapper);
	sphere->GetProperty()->SetColor(1, 0, 0);
}

vtkSmartPointer<vtkActor> Sphere::getData() const
{
	return sphere;
}

class Cylinder
{
public:
	Cylinder(mymath::Vector<double, 3> start, mymath::Vector<double, 3> end, mymath::Vector<double, 3> color = {1, 1, 1});
	vtkSmartPointer<vtkActor> getData();
private:

private:
	vtkSmartPointer<vtkActor> cylinder;
};

vtkSmartPointer<vtkActor> Cylinder::getData()
{
	return cylinder;
}

vtkSmartPointer<vtkTransform> getTransform(mymath::Vector<double, 3> start, mymath::Vector<double, 3> end)
{
	auto rng = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();
	rng->SetSeed((int)time(NULL)); // For testing.

	// Compute a basis
	auto tempX = end - start;
	double normalizedX[3] = {tempX.getItem(0), tempX.getItem(1), tempX.getItem(2)};
	double normalizedY[3];
	double normalizedZ[3];

	double length = vtkMath::Norm(normalizedX);
	vtkMath::Normalize(normalizedX);

	// The Z axis is an arbitrary vector cross X
	double arbitrary[3];
	for (auto i = 0; i < 3; ++i)
	{
		rng->Next();
		arbitrary[i] = rng->GetRangeValue(-10, 10);
	}
	vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
	vtkMath::Normalize(normalizedZ);

	// The Y axis is Z cross X
	vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
	
	auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();

	// Create the direction cosine matrix
	matrix->Identity();
	for (unsigned int i = 0; i < 3; i++)
	{
		matrix->SetElement(i, 0, normalizedX[i]);
		matrix->SetElement(i, 1, normalizedY[i]);
		matrix->SetElement(i, 2, normalizedZ[i]);
	}

	// Apply the transforms
	auto transform = vtkSmartPointer<vtkTransform>::New();
	transform->Translate(start.getItem(0), start.getItem(1), start.getItem(2));   // translate to starting point
	transform->Concatenate(matrix);     // apply direction cosines
	transform->RotateZ(-90.0);          // align cylinder to x axis
	transform->Scale(0.5, length, 0.5); // scale along the height vector
	transform->Translate(0, 0.5, 0);     // translate to start of cylinder

	return transform;
}
Cylinder::Cylinder(mymath::Vector<double, 3> start, mymath::Vector<double, 3> end, mymath::Vector<double, 3> color)
{
	auto cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
	cylinderSource->SetResolution(15);

	auto transform = getTransform(start, end);

	// Create a mapper and actor for the cylinder
	auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinder = vtkSmartPointer<vtkActor>::New();

	mapper->SetInputConnection(cylinderSource->GetOutputPort());
	cylinder->SetUserMatrix(transform->GetMatrix());

	cylinder->SetMapper(mapper);
	cylinder->GetProperty()->SetColor(color.getItem(0), color.getItem(1), color.getItem(2));
}