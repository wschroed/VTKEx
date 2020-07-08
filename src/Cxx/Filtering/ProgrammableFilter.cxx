#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkProgrammableFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

struct params
{
  vtkPolyData* data;
  vtkProgrammableFilter* filter;
};

void AdjustPoints(void* arguments)
{
  params* input = static_cast<params*>(arguments);

  vtkPoints* inPts = input->data->GetPoints();
  vtkIdType numPts = inPts->GetNumberOfPoints();
  vtkNew<vtkPoints> newPts;
  newPts->SetNumberOfPoints(numPts);

  for(vtkIdType i = 0; i < numPts/2; i++)
  {
    double p[3];
    inPts->GetPoint(i, p);
    p[0] = p[0] + .5;
    p[1] = p[1] + .5;
    p[2] = p[2] + .5;
    newPts->SetPoint(i, p);
  }

  for(vtkIdType i = numPts/2; i < numPts; i++)
  {
    double p[3];
    inPts->GetPoint(i, p);
    p[0] = p[0] - .5;
    p[1] = p[1] - .5;
    p[2] = p[2] - .5;
    newPts->SetPoint(i, p);
  }

  input->filter->GetPolyDataOutput()->CopyStructure(input->data);
  input->filter->GetPolyDataOutput()->SetPoints(newPts);

}

int main(int, char *[])
{
  //Create a sphere
  vtkNew<vtkSphereSource> sphereSource;
  sphereSource->Update();

  vtkNew<vtkProgrammableFilter> programmableFilter;
  programmableFilter->SetInputConnection(sphereSource->GetOutputPort());

  params myParams;
  myParams.data = sphereSource->GetOutput();
  myParams.filter = programmableFilter;

  programmableFilter->SetExecuteMethod(AdjustPoints, &myParams);
  programmableFilter->Update();

  //Create a mapper and actor
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(programmableFilter->GetOutputPort());
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  //Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  //Add the actor to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(.4, .5, .6); // Background color white

  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
