#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkLandmarkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVertexGlyphFilter.h>

int main(int, char *[])
{
  /*
  This demo creates a coordinate frame (+x, +y, +z) of vectors and a rotated,
  peturbed frame (+z, +y, -x) and aligns the rotated frame to the original as best as possible.
  */

  vtkNew<vtkPoints> sourcePoints;
  double sourcePoint1[3] = {1.0, 0.0, 0.0};
  sourcePoints->InsertNextPoint(sourcePoint1);
  double sourcePoint2[3] = {0.0, 1.0, 0.0};
  sourcePoints->InsertNextPoint(sourcePoint2);
  double sourcePoint3[3] = {0.0, 0.0, 1.0};
  sourcePoints->InsertNextPoint(sourcePoint3);

  vtkNew<vtkPoints> targetPoints;
  double targetPoint1[3] = {0.0, 0.0, 1.1};
  targetPoints->InsertNextPoint(targetPoint1);
  double targetPoint2[3] = {0.0, 1.02, 0.0};
  targetPoints->InsertNextPoint(targetPoint2);
  double targetPoint3[3] = {-1.11, 0.0, 0.0};
  targetPoints->InsertNextPoint(targetPoint3);

  // Setup the transform
  vtkNew<vtkLandmarkTransform> landmarkTransform ;
  landmarkTransform->SetSourceLandmarks(sourcePoints);
  landmarkTransform->SetTargetLandmarks(targetPoints);
  landmarkTransform->SetModeToRigidBody();
  landmarkTransform->Update(); //should this be here?

  vtkNew<vtkPolyData> source;
  source->SetPoints(sourcePoints);

  vtkNew<vtkPolyData> target;
  target->SetPoints(targetPoints);

  vtkNew<vtkVertexGlyphFilter> sourceGlyphFilter;
  sourceGlyphFilter->SetInputData(source);
  sourceGlyphFilter->Update();

  vtkNew<vtkVertexGlyphFilter> targetGlyphFilter;
  targetGlyphFilter->SetInputData(target);
  targetGlyphFilter->Update();

  vtkNew<vtkTransformPolyDataFilter> transformFilter;
  transformFilter->SetInputConnection(sourceGlyphFilter->GetOutputPort());
  transformFilter->SetTransform(landmarkTransform);
  transformFilter->Update();

  // Display the transformation matrix that was computed
  vtkMatrix4x4* mat = landmarkTransform->GetMatrix();
  std::cout << "Matrix: " << *mat << std::endl;

  // Visualize
  vtkNew<vtkPolyDataMapper> sourceMapper;
  sourceMapper->SetInputConnection(sourceGlyphFilter->GetOutputPort());

  vtkNew<vtkActor> sourceActor;
  sourceActor->SetMapper(sourceMapper);
  sourceActor->GetProperty()->SetColor(0,1,0);
  sourceActor->GetProperty()->SetPointSize(4);

  vtkNew<vtkPolyDataMapper> targetMapper;
  targetMapper->SetInputConnection(targetGlyphFilter->GetOutputPort());

  vtkNew<vtkActor> targetActor;
  targetActor->SetMapper(targetMapper);
  targetActor->GetProperty()->SetColor(1,0,0);
  targetActor->GetProperty()->SetPointSize(4);

  vtkNew<vtkPolyDataMapper> solutionMapper;
  solutionMapper->SetInputConnection(transformFilter->GetOutputPort());

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
