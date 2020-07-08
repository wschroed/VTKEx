//need to set VTK_USE_MATLAB_MEX ON

#include <vtkSmartPointer.h>
#include <vtkMatlabEngineFilter.h>

int main(int argc, char *argv[])
{
  vtkNew<vtkMatlabEngineFilter> matlabFilter ;

  return EXIT_SUCCESS;
}
