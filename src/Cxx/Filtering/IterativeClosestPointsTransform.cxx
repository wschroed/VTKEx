#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkProperty.h>

namespace
{
void CreatePolyData(vtkSmartPointer<vtkPolyData> polydata);
void PerturbPolyData(vtkSmartPointer<vtkPolyData> polydata);
void TranslatePolyData(vtkSmartPointer<vtkPolyData> polydata);
}

int main(int argc, char *argv[])
{
  vtkNew<vtkPolyData> source;
  vtkNew<vtkPolyData> target;

  if(argc == 3)
  {
    std::cout << "Reading data..." << std::endl;
    std::string strSource = argv[1];
    std::string strTarget = argv[2];
    vtkNew<vtkXMLPolyDataReader> sourceReader;
    sourceReader->SetFileName(strSource.c_str());
    sourceReader->Update();
    source->ShallowCopy(sourceReader->GetOutput());

    vtkNew<vtkXMLPolyDataReader> targetReader;
    targetReader->SetFileName(strTarget.c_str());
    targetReader->Update();
    target->ShallowCopy(targetReader->GetOutput());
  }
  else
  {
    std::cout << "Creating data..." << std::endl;
    CreatePolyData(source);
    target->ShallowCopy(source);
    TranslatePolyData(target);
    PerturbPolyData(target);
  }

  // Setup ICP transform
  vtkNew<vtkIterativeClosestPointTransform> icp;
  icp->SetSource(source);
  icp->SetTarget(target);
  icp->GetLandmarkTransform()->SetModeToRigidBody();
  icp->SetMaximumNumberOfIterations(20);
  //icp->StartByMatchingCentroidsOn();
  icp->Modified();
  icp->Update();

  // Get the resulting transformation matrix (this matrix takes the source points to the target points)
  vtkSmartPointer<vtkMatrix4x4> m = icp->GetMatrix();
  std::cout << "The resulting matrix is: " << *m << std::endl;

  // Transform the source points by the ICP solution
  vtkNew<vtkTransformPolyDataFilter> icpTransformFilter;
  icpTransformFilter->SetInputData(source);
  icpTransformFilter->SetTransform(icp);
  icpTransformFilter->Update();

  /*
  // If you need to take the target points to the source points, the matrix is:
  icp->Inverse();
  vtkSmartPointer<vtkMatrix4x4> minv = icp->GetMatrix();
  std::cout << "The resulting inverse matrix is: " << *minv << std::cout;
  */

  // Visualize
  vtkNew<vtkPolyDataMapper> sourceMapper;
  sourceMapper->SetInputData(source);

  vtkNew<vtkActor> sourceActor;
  sourceActor->SetMapper(sourceMapper);
  sourceActor->GetProperty()->SetColor(1,0,0);
  sourceActor->GetProperty()->SetPointSize(4);

  vtkNew<vtkPolyDataMapper> targetMapper;
  targetMapper->SetInputData(target);

  vtkNew<vtkActor> targetActor;
  targetActor->SetMapper(targetMapper);
  targetActor->GetProperty()->SetColor(0,1,0);
  targetActor->GetProperty()->SetPointSize(4);

  vtkNew<vtkPolyDataMapper> solutionMapper;
  solutionMapper->SetInputConnection(icpTransformFilter->GetOutputPort());

  vtkNew<vtkActor> solutionActor;
  solutionActor->SetMapper(solutionMapper);
  solutionActor->GetProperty()->SetColor(0,0,1);
  solutionActor->GetProperty()->SetPointSize(3);

  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(sourceActor);
  renderer->AddActor(targetActor);
  renderer->AddActor(solutionActor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

namespace // anonymous
{

void CreatePolyData(vtkSmartPointer<vtkPolyData> polydata)
{
  // This function creates a set of 4 points (the origin and a point unit distance along each axis)

  vtkNew<vtkPoints> points;

  // Create points
  double origin[3] = {0.0, 0.0, 0.0};
  points->InsertNextPoint(origin);
  double p1[3] = {1.0, 0.0, 0.0};
  points->InsertNextPoint(p1);
  double p2[3] = {0.0, 1.0, 0.0};
  points->InsertNextPoint(p2);
  double p3[3] = {0.0, 0.0, 1.0};
  points->InsertNextPoint(p3);

  vtkNew<vtkPolyData> temp;
  temp->SetPoints(points);

  vtkNew<vtkVertexGlyphFilter> vertexFilter;
  vertexFilter->SetInputData(temp);
  vertexFilter->Update();

  polydata->ShallowCopy(vertexFilter->GetOutput());
}

void PerturbPolyData(vtkSmartPointer<vtkPolyData> polydata)
{
  vtkNew<vtkPoints> points;
  points->ShallowCopy(polydata->GetPoints());

  for(vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
  {
    double p[3];
    points->GetPoint(i, p);
    double perturb[3];
    if(i%3 == 0)
    {
      perturb[0] = .1; perturb[1] = 0; perturb[2] = 0;
    }
    else if(i%3 == 1)
    {
      perturb[0] = 0; perturb[1] = .1; perturb[2] = 0;
    }
    else
    {
      perturb[0] = 0; perturb[1] = 0; perturb[2] = .1;
    }

    for(unsigned int j = 0; j < 3; j++)
    {
      p[j] += perturb[j];
    }
    points->SetPoint(i, p);
  }

  polydata->SetPoints(points);

}

void TranslatePolyData(vtkSmartPointer<vtkPolyData> polydata)
{
  vtkNew<vtkTransform> transform;
  transform->Translate(0,.3,0);

  vtkNew<vtkTransformPolyDataFilter> transformFilter;
  transformFilter->SetInputData(polydata);
  transformFilter->SetTransform(transform);
  transformFilter->Update();

  polydata->ShallowCopy(transformFilter->GetOutput());
}

} // end anonymous namespace
