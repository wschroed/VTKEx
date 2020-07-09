#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkSphereSource.h>
#include <vtkMatrixMathFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLPolyDataWriter.h>

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    std::cerr << "Required arguments: vtkFile" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename = argv[1]; // "/Data/tensors.vtk";

  std::cout << "filename: " << filename << std::endl;

  vtkNew<vtkUnstructuredGridReader> reader;
  reader->SetFileName(filename.c_str());
  reader->Update();

  vtkNew<vtkDataSetSurfaceFilter> surfaceFilter;
  surfaceFilter->SetInputConnection(reader->GetOutputPort());
  surfaceFilter->Update();

  vtkNew<vtkMatrixMathFilter> matrixMathFilter;
  //matrixMathFilter->SetOperationToDeterminant();
  matrixMathFilter->SetOperationToEigenvalue();
  matrixMathFilter->SetInputConnection(surfaceFilter->GetOutputPort());
  matrixMathFilter->Update();
  matrixMathFilter->GetOutput()->GetPointData()->SetActiveScalars("Eigenvalue");

  vtkNew<vtkXMLPolyDataWriter> writer;
  writer->SetInputConnection(matrixMathFilter->GetOutputPort());
  writer->SetFileName("output.vtp");
  writer->Write();

  // Visualize
  vtkNew<vtkPolyDataMapper> mapper ;
  mapper->SetInputConnection(matrixMathFilter->GetOutputPort());

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
