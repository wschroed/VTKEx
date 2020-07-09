#include <vtkSmartPointer.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyData.h>
#include <vtkDiskSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
  vtkNew<vtkDiskSource> diskSource;
  diskSource->Update();

  vtkNew<vtkFeatureEdges> featureEdges;
  featureEdges->SetInputConnection(diskSource->GetOutputPort());
  featureEdges->BoundaryEdgesOn();
  featureEdges->FeatureEdgesOff();
  featureEdges->ManifoldEdgesOff();
  featureEdges->NonManifoldEdgesOff();
  featureEdges->Update();

  // Visualize
  vtkNew<vtkPolyDataMapper> edgeMapper;
  edgeMapper->SetInputConnection(featureEdges->GetOutputPort());
  vtkNew<vtkActor> edgeActor;
  edgeActor->SetMapper(edgeMapper);

  vtkNew<vtkPolyDataMapper> diskMapper;
  diskMapper->SetInputConnection(diskSource->GetOutputPort());
  vtkNew<vtkActor> diskActor;
  diskActor->SetMapper(diskMapper);

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(edgeActor);
  renderer->AddActor(diskActor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
