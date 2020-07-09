#include <vtkSmartPointer.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkClipClosedSurface.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkSphereSource.h>

//
// Demonstrate the use of clipping of polygonal data
//

int main (int argc, char *argv[])
{
  // PolyData to process
  vtkSmartPointer<vtkPolyData> polyData;

  if (argc > 1)
  {
    vtkNew<vtkXMLPolyDataReader> reader;
    reader->SetFileName(argv[1]);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else
  {
    // Create a sphere
    vtkNew<vtkSphereSource> sphereSource;
    sphereSource->SetThetaResolution(20);
    sphereSource->SetPhiResolution(11);
    sphereSource->Update();

    polyData = sphereSource->GetOutput();
  }

  double *center = polyData->GetCenter();
  vtkNew<vtkPlane> plane1;
  plane1->SetOrigin(center[0], center[1], center[2]);
  plane1->SetNormal(0.0, -1.0, 0.0);
  vtkNew<vtkPlane> plane2;
  plane2->SetOrigin(center[0], center[1], center[2]);
  plane2->SetNormal(0.0, 0.0, 1.0);
  vtkNew<vtkPlane> plane3;
  plane3->SetOrigin(center[0], center[1], center[2]);
  plane3->SetNormal(-1.0, 0.0, 0.0);

  vtkNew<vtkPlaneCollection> planes;
  planes->AddItem(plane1);
  planes->AddItem(plane2);
  planes->AddItem(plane3);

  vtkNew<vtkClipClosedSurface> clipper;
  clipper->SetInputData(polyData);
  clipper->SetClippingPlanes(planes);
  clipper->SetActivePlaneId(2);
  clipper->SetScalarModeToColors();
  clipper->SetClipColor(0.8900, 0.8100, 0.3400); // banana
  clipper->SetBaseColor(1.0000, 0.3882, 0.2784); // tomato
  clipper->SetActivePlaneColor(0.6400, 0.5800, 0.5000); // beige

  vtkNew<vtkDataSetMapper> clipMapper;
  clipMapper->SetInputConnection(clipper->GetOutputPort());

  vtkNew<vtkActor> clipActor;
  clipActor->SetMapper(clipMapper);
  clipActor->GetProperty()->SetColor(1.0000,0.3882,0.2784);
  clipActor->GetProperty()->SetInterpolationToFlat();

  // Create graphics stuff
  //
  vtkNew<vtkRenderer> ren1;
  ren1->SetBackground(.3, .4, .6);

  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(ren1);
  renWin->SetSize(512,512);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);

  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(clipActor);

  // Generate an interesting view
  //
  ren1->ResetCamera();
  ren1->GetActiveCamera()->Azimuth(120);
  ren1->GetActiveCamera()->Elevation(30);
  ren1->GetActiveCamera()->Dolly(1.0);
  ren1->ResetCameraClippingRange();

  renWin->Render();
  iren->Initialize();
  iren->Start();

  return EXIT_SUCCESS;
}
