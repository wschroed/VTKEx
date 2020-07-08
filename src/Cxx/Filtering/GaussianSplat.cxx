#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkContourFilter.h>
#include <vtkGaussianSplatter.h>
#include <vtkSphereSource.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
  // Create points on a sphere
  vtkNew<vtkSphereSource> sphereSource ;
  sphereSource->Update();

  vtkNew<vtkPolyData> polydata ;
  polydata->SetPoints(sphereSource->GetOutput()->GetPoints());

  vtkNew<vtkGaussianSplatter> splatter ;
  splatter->SetInputData(polydata);
  splatter->SetSampleDimensions(50,50,50);
  splatter->SetRadius(0.5);
  splatter->ScalarWarpingOff();

  vtkNew<vtkContourFilter> surface ;
  surface->SetInputConnection(splatter->GetOutputPort());
  surface->SetValue(0,0.01);

  // Create a mapper and actor
  vtkNew<vtkPolyDataMapper> mapper ;
  mapper->SetInputConnection(surface->GetOutputPort());

  vtkNew<vtkActor> actor ;
  actor->SetMapper(mapper);

  // Visualize
  vtkNew<vtkRenderer> renderer ;
  vtkNew<vtkRenderWindow> renderWindow ;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor ;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1); // Background color white

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;

}
