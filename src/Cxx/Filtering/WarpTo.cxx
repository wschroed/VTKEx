#include <vtkSmartPointer.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkWarpTo.h>

int main(int, char*[])
{
  // Create the RenderWindow, Renderer and both Actors
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Create a line
  vtkNew<vtkLineSource> lineSource ;
  lineSource->SetPoint1(0.0, 0.0, 0.0);
  lineSource->SetPoint2(0.0, 1.0, 0.0);
  lineSource->SetResolution(20);
  lineSource->Update();

  // Create a tube (cylinder) around the line
  vtkNew<vtkTubeFilter> tubeFilter ;
  tubeFilter->SetInputConnection(lineSource->GetOutputPort());
  tubeFilter->SetRadius(.01); //default is .5
  tubeFilter->SetNumberOfSides(50);
  tubeFilter->Update();

  vtkNew<vtkWarpTo> warpTo ;
  warpTo->SetInputConnection(tubeFilter->GetOutputPort());
  warpTo->SetPosition(10, 1, 0);
  warpTo->SetScaleFactor(5);
  warpTo->AbsoluteOn();

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputConnection(warpTo->GetOutputPort());
  mapper->ScalarVisibilityOff();

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  renderer->AddActor(actor);

  renderWindow->Render();
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
