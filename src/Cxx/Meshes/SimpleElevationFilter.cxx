#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkDelaunay2D.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include <vtkSimpleElevationFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

int main(int, char *[])
{
  // Created a grid of points (heigh/terrian map)
  vtkNew<vtkPoints> points;

  unsigned int GridSize = 10;
  for(unsigned int x = 0; x < GridSize; x++)
  {
    for(unsigned int y = 0; y < GridSize; y++)
    {
      points->InsertNextPoint(x, y, (x+y)/(y+1));
    }
  }

  double bounds[6];
  points->GetBounds(bounds);

  // Add the grid points to a polydata object
  vtkNew<vtkPolyData> inputPolyData;
  inputPolyData->SetPoints(points);

  // Triangulate the grid points
  vtkNew<vtkDelaunay2D> delaunay;
  delaunay->SetInputData(inputPolyData);
  delaunay->Update();

  vtkNew<vtkSimpleElevationFilter> elevationFilter;
  elevationFilter->SetInputConnection(delaunay->GetOutputPort());
  elevationFilter->SetVector(0.0, 0.0, 1);
  elevationFilter->Update();

  vtkNew<vtkPolyData> output;
  output->ShallowCopy(dynamic_cast<vtkPolyData*>(elevationFilter->GetOutput()));

  vtkFloatArray* elevation = dynamic_cast<vtkFloatArray*>(output->GetPointData()->GetArray("Elevation"));

  // Create the color map
  vtkNew<vtkLookupTable> colorLookupTable;
  colorLookupTable->SetTableRange(bounds[4], bounds[5]);
  colorLookupTable->Build();

  // Generate the colors for each point based on the color map
  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetNumberOfComponents(3);
  colors->SetName("Colors");

  for(vtkIdType i = 0; i < output->GetNumberOfPoints(); i++)
  {
    double val = elevation->GetValue(i);
    std::cout << "val: " << val << std::endl;

    double dcolor[3];
    colorLookupTable->GetColor(val, dcolor);
    std::cout << "dcolor: " << dcolor[0] << " " << dcolor[1] << " " << dcolor[2] << std::endl;
    unsigned char color[3];
    for(unsigned int j = 0; j < 3; j++)
    {
      color[j] = 255 * dcolor[j]/1.0;
    }
    std::cout << "color: " << (int)color[0] << " " << (int)color[1] << " " << (int)color[2] << std::endl;

    colors->InsertNextTupleValue(color);
  }

  output->GetPointData()->AddArray(colors);

  // Visualize
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(output);

  vtkNew<vtkActor> actor;
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
