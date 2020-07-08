#include <vtkSmartPointer.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

#include <vtkPerlinNoise.h>

int main(int, char *[])
{
  vtkNew<vtkPerlinNoise> perlinNoise;
  perlinNoise->SetFrequency(2, 1.25, 1.5);
  perlinNoise->SetPhase(0, 0, 0);

  vtkNew<vtkSampleFunction> sample;
  sample->SetImplicitFunction(perlinNoise);
  sample->SetSampleDimensions(65, 65, 20);
  sample->ComputeNormalsOff();

  vtkNew<vtkContourFilter> surface;
  surface->SetInputConnection(sample->GetOutputPort());
  surface->SetValue(0, 0.0);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(surface->GetOutputPort());
  mapper->ScalarVisibilityOff();

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0.2, 0.4, 0.6);

  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(renderWindow);

  // Add the actors to the renderer, set the background and size

  renderer->AddActor(actor);
  renderer->SetBackground(1, 1, 1);
  renderWindow->SetSize(300, 300);
  renderer->ResetCamera();
  renderWindow->Render();
  interactor->Start();

  return EXIT_SUCCESS;
}
