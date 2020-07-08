#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
  // Create the polydata geometry
  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->Update();

  // Set up the actor to display the untransformed polydata

  vtkNew<vtkPolyDataMapper> originalMapper;
  originalMapper->SetInputConnection(sphereSource->GetOutputPort());

  vtkNew<vtkActor> originalActor;
  originalActor->SetMapper(originalMapper);
  originalActor->GetProperty()->SetColor(1,0,0);

  // Set up the transform filter
  vtkNew<vtkTransform> translation;
  translation->Translate(1.0, 2.0, 3.0);

  vtkNew<vtkTransformPolyDataFilter> transformFilter;
  transformFilter->SetInputConnection(sphereSource->GetOutputPort());
  transformFilter->SetTransform(translation);
  transformFilter->Update();

  // Set up the actor to display the transformed polydata
  vtkNew<vtkPolyDataMapper> transformedMapper;
  transformedMapper->SetInputConnection(transformFilter->GetOutputPort());

  vtkNew<vtkActor> transformedActor;
  transformedActor->SetMapper(transformedMapper);
  transformedActor->GetProperty()->SetColor(0,1,0);

  // Set up the rest of the visualization pipeline
  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(originalActor);
  renderer->AddActor(transformedActor);
  renderer->SetBackground(.3, .6, .3); // Set renderer's background color to green

  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
