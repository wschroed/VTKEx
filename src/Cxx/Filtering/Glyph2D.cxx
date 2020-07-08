#include <vtkSmartPointer.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph2D.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>

int main(int, char *[])
{
  vtkNew<vtkPoints> points ;
  points->InsertNextPoint(0,0,0);
  points->InsertNextPoint(1,1,0);
  points->InsertNextPoint(2,2,0);

  vtkNew<vtkPolyData> polydata ;
  polydata->SetPoints(points);

  // Create anything you want here, we will use a polygon for the demo.
  vtkNew<vtkRegularPolygonSource> polygonSource ;

  vtkNew<vtkGlyph2D> glyph2D ;
  glyph2D->SetSourceConnection(polygonSource->GetOutputPort());
  glyph2D->SetInputData(polydata);
  glyph2D->Update();

  vtkNew<vtkPolyDataMapper> mapper ;
  mapper->SetInputConnection(glyph2D->GetOutputPort());
  mapper->Update();

  vtkNew<vtkActor> actor ;
  actor->SetMapper(mapper);

  // Visualize
  vtkNew<vtkRenderer> renderer ;
  vtkNew<vtkRenderWindow> renderWindow ;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor ;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);

  vtkNew<vtkInteractorStyleImage> style ;
  renderWindowInteractor->SetInteractorStyle( style );

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
