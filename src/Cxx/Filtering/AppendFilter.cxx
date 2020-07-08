#include <vtkSmartPointer.h>
#include <vtkAppendFilter.h>

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPointSource.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkNamedColors.h>

int main(int, char *[])
{
  // Create 5 points (vtkPolyData)
  vtkNew<vtkPointSource> pointSource;
  pointSource->SetNumberOfPoints(5);
  pointSource->Update();

  vtkSmartPointer<vtkPolyData> polydata = pointSource->GetOutput();

  std::cout << "There are " << polydata->GetNumberOfPoints()
            << " points in the polydata." << std::endl;

  // Create 2 points in a vtkUnstructuredGrid

  vtkNew<vtkPoints> points;
  points->InsertNextPoint(0,0,0);
  points->InsertNextPoint(0,0,1);

  vtkNew<vtkUnstructuredGrid> ug;
  ug->SetPoints(points);

  std::cout << "There are " << ug->GetNumberOfPoints()
            << " points in the unstructured grid." << std::endl;

  // Combine the two data sets
  vtkNew<vtkAppendFilter> appendFilter;
  appendFilter->AddInputData(polydata);
  appendFilter->AddInputData(ug);
  appendFilter->Update();

  vtkNew<vtkUnstructuredGrid> combined;
  std::cout << "There are " << combined->GetNumberOfPoints()
            << " points combined." << std::endl;

  // Create a mapper and actor
  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputConnection(appendFilter->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(5);

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
