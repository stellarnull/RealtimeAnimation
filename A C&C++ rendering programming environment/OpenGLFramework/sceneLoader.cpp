/***************************************************************************/
/* sceneLoader.cpp                                                         */
/* -----------------------                                                 */
/*                                                                         */
/* A basic OpenGL program that reads in a scene file and displays it.      */
/*     Note that not all the keywords and functionality of the ray tracer  */
/*     is implemented, especially odd material and object types.  Some     */
/*     types are implemented as other types (e.g., "WardTriangles" are     */
/*     simply more efficient triangles for a ray tracer, but is fine for   */
/*     OpenGL to handle with a default "Triangle" class).                  */
/*                                                                         */
/* Chris Wyman (02/01/2008)                                                */
/***************************************************************************/

#include "sceneLoader.h"
#include "RenderingTechniques/renderingData.h"
#include "RenderingTechniques/renderingMethods.h"

Scene *scene = 0;
FrameRate *frameSpeed = 0;
FrameGrab *frameGrab  = 0;
extern RenderingData *data;


void DisplayCallback( void )
{
	frameSpeed->StartFrame();

	// Create a shadow map
	//if (usingAShadowMap)
	//	scene->CreateShadowMap( data->fbo->shadowMap,           // Draw the shadow map into here
	//		                    data->param->shadowMatrixTrans, // Put the shadow map matrix here
	//							0,                              // Light Number (an integer)
	//							shadowMapBias );                // Bias for the shadow map matrix (a float)

	// Into the main FBO texture, draw the scene with no special effects
	data->fbo->mainWin->BindBuffer();
	data->fbo->mainWin->ClearBuffers();
	glLoadIdentity();
	scene->LookAtMatrix();
	scene->SetupEnabledLightsWithCurrentModelview();
	
	// Draw the scene
	scene->Draw(); 

	//TODO:Rewrite your own Draw fuction

	data->fbo->mainWin->UnbindBuffer();

	// Copy the final rendering to the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	data->fbo->mainWin->DisplayAsFullScreenTexture( FBO_COLOR0 );

	// If a screen capture is desired, do it now (before writing timer text)
	if (data->ui->captureScreen)
	{
		frameGrab->CaptureFrame();
		data->ui->captureScreen = false;
	}

	DisplayTimer( frameSpeed->EndFrame(), 1024, 1024 );
	glFlush();
	glutSwapBuffers();
}



int main(int argc, char* argv[] )
{
	bool verbose = false;
	char scenefile[ 256 ];
	char windowTitle[ 512 ];
	printf("**************************************************************************\n");
    printf("*                CAD Course Basic OpenGL Scene Loader                 *\n");
	printf("**************************************************************************\n");

	// Read in the command line parameters 
	printf("(+) Parsing any command line parameters...\n");
	if (argc < 1)
	{
		printf("Usage: %s <sceneFile>\n", argv[0]);
		exit(0);
	}
	for( int i=1; i < argc; i++ )
	{
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") || 
			!strcmp(argv[i], "-?") || !strcmp(argv[i], "/?") )
		{
			printf("Usage: %s <sceneFile>\n", argv[0]);
			exit(0);
		}
		else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "-verbose"))
			verbose = true;
		else
		{
			strncpy( scenefile, argv[i], 255 );
			break;
		}
	}

	// Load the specified scene from file. 
	scene = new Scene( scenefile, verbose );
	sprintf( windowTitle, "Basic Scene Loader (Displaying: %s)", scenefile );

	printf("(+) Initializing OpenGL state...\n");
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
	glutInitWindowSize( scene->GetWidth(), scene->GetHeight() );
	glutCreateWindow( windowTitle );
	glewInit();

	// Check to make sure this card supports all the features used by this program     
	if (!GLEW_VERSION_2_0)
		FatalError("Machine lacks support for OpenGL 2.0!");
	if (!GLEW_EXT_framebuffer_object)
		FatalError("Machine lacks support for OpenGL's framebuffer_object extension!");
	if (!GLEW_ARB_texture_float)
		FatalError("Machine lacks support for OpenGL's texture_float extension!");
	if (!GLEW_ARB_vertex_buffer_object)
		FatalError("Machine lacks support for OpenGL's vertex_buffer_object extension!");

	// There's probably a bunch of other extenstions that I just use automatically now 
	//   But I suspect if all three of these pass then the card/driver should work for 
	//   this basic framework.                                                         

	// Set the GLUT callback functions 
	glutDisplayFunc( DisplayCallback );
	glutReshapeFunc( ReshapeCallback );
	glutIdleFunc( IdleCallback );
	glutMouseFunc( MouseButtonCallback );
	glutMotionFunc( MouseMotionCallback );
	glutKeyboardFunc( KeyboardCallback );
	glutSpecialFunc( SpecialKeyboardCallback ); 

	// Other program setup 
	frameSpeed = new FrameRate( 50 );
	frameGrab  = new FrameGrab();

	// Make sure any preprocessing that needs to be done occurs
	scene->Preprocess();

	// Initialized various data needed in the various rendering (display) callbacks
	InitializeRenderingData();

	// Run the display!
	printf("(+) Starting Main Loop...\n" );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glEnable( GL_NORMALIZE );
	glEnable( GL_CULL_FACE );
	scene->GetLight(0)->Enable();
	glutMainLoop();

	return 0;
}
