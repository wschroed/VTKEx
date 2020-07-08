#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDelaunay2D.h>
#include <vtkMath.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>

int main(int, char *[])
{
  // Create points on an XY grid with random Z coordinate
  vtkNew<vtkPoints> points;

  unsigned int gridSize = 10;
  for(unsigned int x = 0; x < gridSize; x++)
  {
    for(unsigned int y = 0; y < gridSize; y++)
    {
      points->InsertNextPoint(x, y, vtkMath::Random(0.0, 3.0));
    }
  }

  // Add the grid points to a polydata object
  vtkNew<vtkPolyData> polydata;
  polydata->SetPoints(points);

  vtkNew<vtkVertexGlyphFilter> glyphFilter;
  glyphFilter->SetInputData(polydata);
  glyphFilter->Update();

  // Create a mapper and actor
  vtkNew<vtkPolyDataMapper> pointsMapper;
  pointsMapper->SetInputConnection(glyphFilter->GetOutputPort());

  vtkNew<vtkActor> pointsActor;
  pointsActor->SetMapper(pointsMapper);
  pointsActor->GetProperty()->SetPointSize(3);
  pointsActor->GetProperty()->SetColor(1,0,0);
  // Triangulate the grid points
  vtkNew<vtkDelaunay2D> delaunay;
  delaunay->SetInputData(polydata);
  delaunay->Update();

  // Create a mapper and actor
  vtkNew<vtkPolyDataMapper> triangulatedMapper;
  triangulatedMapper->SetInputConnection(delaunay->GetOutputPort());

  vtkNew<vtkActor> triangulatedActor;
  triangulatedActor->SetMapper(triangulatedMapper);

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(pointsActor);
  renderer->AddActor(triangulatedActor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
