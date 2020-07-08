#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph3D.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
  vtkNew<vtkPoints> points ;
  points->InsertNextPoint(0,0,0);
  points->InsertNextPoint(1,1,1);
  points->InsertNextPoint(2,2,2);
  vtkNew<vtkPolyData> polydata ;
  polydata->SetPoints(points);

  // Create anything you want here, we will use a cube for the demo.
  vtkNew<vtkCubeSource> cubeSource ;

  vtkNew<vtkGlyph3D> glyph3D ;
  glyph3D->SetSourceConnection(cubeSource->GetOutputPort());
  glyph3D->SetInputData(polydata);
  glyph3D->Update();

  // Visualize
  vtkNew<vtkPolyDataMapper> mapper ;
  mapper->SetInputConnection(glyph3D->GetOutputPort());

  vtkNew<vtkActor> actor ;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
