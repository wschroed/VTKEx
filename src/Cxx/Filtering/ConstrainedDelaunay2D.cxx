#include <vtkSmartPointer.h>
#include <vtkDelaunay2D.h>

#include <vtkCellArray.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolygon.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkVersionMacros.h> // For version macros

int main(int, char *[])
{
  // Generate a 10 x 10 grid of points
  vtkNew<vtkPoints> points;
  for(unsigned int x = 0; x < 10; x++)
  {
    for(unsigned int y = 0; y < 10; y++)
    {
      points->InsertNextPoint(x + vtkMath::Random(-.25, .25),
                              y + vtkMath::Random(-.25,.25),
                              0);
    }
  }

  vtkNew<vtkPolyData> aPolyData;
  aPolyData->SetPoints(points);

  // Create a cell array to store the polygon in
  vtkNew<vtkCellArray> aCellArray;

  // Define a polygonal hole with a clockwise polygon
  vtkNew<vtkPolygon> aPolygon;

  aPolygon->GetPointIds()->InsertNextId(22);
  aPolygon->GetPointIds()->InsertNextId(23);
  aPolygon->GetPointIds()->InsertNextId(24);
  aPolygon->GetPointIds()->InsertNextId(25);
  aPolygon->GetPointIds()->InsertNextId(35);
  aPolygon->GetPointIds()->InsertNextId(45);
  aPolygon->GetPointIds()->InsertNextId(44);
  aPolygon->GetPointIds()->InsertNextId(43);
  aPolygon->GetPointIds()->InsertNextId(42);
  aPolygon->GetPointIds()->InsertNextId(32);

  aCellArray->InsertNextCell(aPolygon);

  // Create a polydata to store the boundary. The points must be the
  // same as the points we will triangulate.
  vtkNew<vtkPolyData> boundary;
  boundary->SetPoints(aPolyData->GetPoints());
  boundary->SetPolys(aCellArray);

  // Triangulate the grid points
  vtkNew<vtkDelaunay2D> delaunay;
  delaunay->SetInputData(aPolyData);
  delaunay->SetSourceData(boundary);

  // Visualize
  vtkNew<vtkPolyDataMapper> meshMapper;
  meshMapper->SetInputConnection(delaunay->GetOutputPort());

  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkActor> meshActor;
  meshActor->SetMapper(meshMapper);
  meshActor->GetProperty()->EdgeVisibilityOn();
  meshActor->GetProperty()->SetEdgeColor(colors->GetColor3d("Peacock").GetData());
  meshActor->GetProperty()->SetInterpolationToFlat();

  vtkNew<vtkPolyDataMapper> boundaryMapper;
  boundaryMapper->SetInputData(boundary);

  vtkNew<vtkActor> boundaryActor;
  boundaryActor->SetMapper(boundaryMapper);
  boundaryActor->GetProperty()->SetColor(colors->GetColor3d("Raspberry").GetData());
  boundaryActor->GetProperty()->SetLineWidth(3);
  boundaryActor->GetProperty()->EdgeVisibilityOn();
  boundaryActor->GetProperty()->SetEdgeColor(1,0,0);
  boundaryActor->GetProperty()->SetRepresentationToWireframe();

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(meshActor);
  renderer->AddActor(boundaryActor);
  renderer->SetBackground(colors->GetColor3d("Mint").GetData());

  // Render and interact
  renderWindow->SetSize(640, 480);
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
