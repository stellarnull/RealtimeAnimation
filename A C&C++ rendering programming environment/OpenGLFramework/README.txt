  Basic OpenGL Framework

This framework was tested in Visual Studion 2008 on my desktop, as
well as Visual Studio 2008 on my laptop.  You should be able to 
directly open and compile it, assuming you have GLUT and GLEW 
installed correctly on your machine.  If not, see here:
  http://www.cs.uiowa.edu/~cwyman/classes/common/howto/winGLUT.html

Please note that the "bin/" directory includes glut32.dll and
glew32.dll, so if you compile using a different version, you may
need to delete those .dll's before running the executable.

If you have problems compiling, because of conflicting definitions
of "exit()," you have four choices.  Two are described on the
above web page.  You can also comment out line 161 of <glut.h>
(a declaration of exit()) or change the declaration to match that
from <stdio.h>

All the textures, shaders, models, and scene files needed to run
the basic program are included in the "bin/" directory.  Please
keep all additional textures, shaders, models, and scenes in 
their corresponding directories.

Running the program __requires__ a command-line parameter that 
specifies what scene file to load.  Three example scene files
are provided "sponzaModel.txt" and
"cbox_meters.txt".  You may, but need not specify a pathname, 
since the program explicitly searches in the "scenes/" directory.

For information on where to start looking to understand how the
framework works, start with:
  sceneLoader.cpp   <-- main() and a basic GLUT display callback
  glInterface.cpp   <-- idle & reshape callbacks plus the 
                        "ApplyUICommand" which is where UI 
                        commands (defined in the scene file) are
                        processed.  If you need access to standard
                        keyboard & mouse callbacks, see 
                        Interface/glKeyAndMouseCallbacks.cpp
  bin/scenes/cbox_meters.txt  <-- A basic scene 
  bin/scenes/sponzaModel.txt <-- A much more advanced scene 
                        
  Utils/glslProgram.h  <-- My GLSL shader class
  Scene/Scene.h        <-- A class that describes the scene, it's
                           view, objects, and materials
  Scene/Camera.h       <-- A class that describes the scene camera
  Scene/glLight.h      <-- A class that describes scene lights
  RenderingTechniques/initializeRenderingData.cpp <-- When I use
                        this framework, data needed by various 
                        rendering techniques is defined here.  I
                        then either modify my DisplayCallback()
                        or create additional files in the 
                        "RenderingTechniques/" directory with 
                        alternate display callbacks.

