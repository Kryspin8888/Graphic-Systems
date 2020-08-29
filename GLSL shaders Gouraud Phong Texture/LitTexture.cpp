// Mid-level VS & FS Shaders
// By Mariusz Szwoch
// Based on 
// OpenGL SuperBible
// Program by Richard S. Wright Jr.

#include <GLTools.h>	// OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>
#include <math.h>
#define FREEGLUT_STATIC
#include <glut.h>

GLFrame viewFrame;
GLFrame cameraFrame;	
GLFrustum viewFrustum;
GLTriangleBatch sphereBatch;
GLTriangleBatch torusBatch;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeline;
GLShaderManager shaderManager;


GLfloat light0Pos[] = { 140.0f, 250.0f, 140.0f, 1.0f };
GLfloat lightRotation = 0.0f;


// 3. Dodaj swoje imi� i nazwisko do belki tytu�owej
// ...
const char *authorName = "Krzysztof Listewnik";

// !!! w ShaderTypes dopisujemy kolejne shadery (przed SHADERS_NUM ) !!!!!!!!!!!!!
enum ShaderTypes { AMBIENT_DIFFUSE_VS,AMBIENT_DIFFUSE_SPECULAR_Gouraud,AMBIENT_DIFFUSE_SPECULAR_Phong,ADS_TEXTURE_Phong,ADS_TEXTURE_Phong_PARAM,ADS_CARTOON,ADS_TEXTURE_Phong_Optimized, SHADERS_NUM }; // identyfikatory program�w cieniowania

ShaderTypes actualShader = AMBIENT_DIFFUSE_VS; // aktualny program cieniowania

char *shaderNames[ SHADERS_NUM ] = 
{ // WY�WIETLANE nazwy program�w cieniowania -
	"AD_PerVertex", // Ambient + Diffuse
	"ADS_PerVertex_Gouraud",
	"ADS_PerFragment_Phong",
	"ADS_PerFragment_Phong_Texture",
	"ADS_PerFragment_Phong_Param",
	"ADS_PerFragment_Cartoon",
	"ADS_PerFragment_Phong_Texture_Optimized"
};

GLuint shaders[ SHADERS_NUM ]; // obiekty program�w cieniowania wierzcho�k�w (VS+FS)
GLint	locOfLight0Ambient[ SHADERS_NUM ]; // "adres" sk�adowej ambient �wiat�a 0
GLint	locOfLight0Diffuse[ SHADERS_NUM ]; // "adres" sk�adowej diffuse �wiat�a 0
GLint	locOfLight0Specular[ SHADERS_NUM ]; // "adres" sk�adowej specular �wiat�a 0
GLint	locOfMaterialAmbient[ SHADERS_NUM ]; // "adres" sk�adowej ambient materia�u 0
GLint	locOfMaterialDiffuse[ SHADERS_NUM ]; // "adres" sk�adowej diffuse materia�u 0
GLint	locOfMaterialSpecular[ SHADERS_NUM ]; // "adres" sk�adowej specular materia�u 0

GLint	locLight[ SHADERS_NUM ]; // The location of the Light in eye coordinates
GLint	locMVP[ SHADERS_NUM ]; // The location of the ModelViewProjection matrix uniform
GLint	locMV[ SHADERS_NUM ]; // The location of the ModelView matrix uniform
GLint	locNM[ SHADERS_NUM ]; // The location of the Normal matrix uniform
GLint locTexture[ SHADERS_NUM ];

GLuint texture; // podstawowa tekstura
GLuint LUTtexture; // tekstura funkcji transformacji jasno�ci (Look-Up Table)


bool LoadTGATexture( const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode )
{ // Load a TGA as a 2D Texture. Completely initialize the state
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	// Read the texture bits
	pBits = gltReadTGABits( szFileName, &nWidth, &nHeight, &nComponents, &eFormat );
	if ( pBits == NULL ) 
		return false;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D( GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits );
	free( pBits );
	if ( minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST ||
			minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST )
		glGenerateMipmap(GL_TEXTURE_2D);
	return true;
}
 
void SetupRC( void )
{ // Inicjalizacja parametr�w kontekstu renderowania
	char VS_FileName[ 255 ], FS_FileName[ 255 ];
	
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); // czyszczenie t�a
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	shaderManager.InitializeStockShaders();
	viewFrame.MoveForward( 6.0f );

	gltMakeSphere( sphereBatch, 0.5f, 26, 13 ); // utworzenie sfery
	gltMakeTorus( torusBatch, 0.8f, 0.25f, 52, 26 ); // utworzenie torusa


	for ( int shaderIdx = AMBIENT_DIFFUSE_VS; shaderIdx < SHADERS_NUM; shaderIdx++ )
	{ // wczytanie wszystkich program�w cieniowania
		sprintf( VS_FileName, "./shaders/%s.vs", shaderNames[ shaderIdx ] );
		sprintf( FS_FileName, "./shaders/%s.fs", shaderNames[ shaderIdx ] );
		shaders[ shaderIdx ] = gltLoadShaderPairWithAttributes( VS_FileName, FS_FileName, 3, 
																				GLT_ATTRIBUTE_VERTEX, "vVertex", // wsp�rz�dne wierzcho�ka
																				GLT_ATTRIBUTE_NORMAL, "vNormal", // wektor normalny
																				GLT_ATTRIBUTE_TEXTURE0, "vTexture0"); // tekstura
		// "adresy" parametr�w 
		locOfLight0Ambient[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "Light0Ambient" ); // "adres" sk�adowej ambient �wiat�a 0
		locOfLight0Diffuse[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "Light0Diffuse" ); // "adres" sk�adowej diffuse �wiat�a 0
		locOfLight0Specular[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "Light0Specular" ); // "adres" sk�adowej specular �wiat�a 0
		locOfMaterialAmbient[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "MaterialAmbient" ); // "adres" sk�adowej ambient materia�u 0
		locOfMaterialDiffuse[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "MaterialDiffuse" ); // "adres" sk�adowej diffuse materia�u 0
		locOfMaterialSpecular[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "MaterialSpecular" ); // "adres" sk�adowej specular materia�u 0

		locLight[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "vLightPosition");
		locMVP[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "mvpMatrix" );
		locMV[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "mvMatrix" );
		locNM[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "normalMatrix" );
		locTexture[ shaderIdx ] = glGetUniformLocation( shaders[ shaderIdx ], "colorMap" );
	}
	// utworzenie i za�adowanie obiekt�w tekstur
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	LoadTGATexture( "Media/CoolTexture.tga", GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );
	glGenTextures( 1, &LUTtexture );
	glBindTexture( GL_TEXTURE_1D, LUTtexture );
	GLubyte textureData[ 4 ][ 3 ] = { 32, 0, 0,		64,  0, 0,		128, 0, 0,		255, 0, 0 }; 
	glTexImage1D( GL_TEXTURE_1D, 0, GL_RGB, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
}

void ShutdownRC(void)
{ // Zwolnienie zasob�w
	glDeleteTextures( 1, &texture ); 
	glDeleteTextures( 1, &LUTtexture ); 
}

void RenderScene( void )
{ // Narysowanie sceny
	static CStopWatch rotTimer;
	// �wiat�o 0
	GLfloat Light0Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // sk�adowa ambient 
	GLfloat Light0Diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // sk�adowa diffuse 
	GLfloat Light0Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // sk�adowa specular
	// materia� 0
	GLfloat Material0Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // sk�adowa ambient 
	GLfloat Material0Diffuse[] = { 0.1f, 0.1f, 1.0f, 1.0f }; // sk�adowa diffuse 
	GLfloat Material0Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // sk�adowa specular
	// materia� 1
	GLfloat Material1Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // sk�adowa ambient 
	GLfloat Material1Diffuse[] = { 1.0f, 0.1f, 0.1f, 1.0f }; // sk�adowa diffuse 
	GLfloat Material1Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // sk�adowa specular

	GLfloat vEyeLight[] = { -100.0f, 100.0f, 100.0f };
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // wyczyszczenie okna i bufora g��bi
	
	modelViewMatrix.PushMatrix( viewFrame );

	M3DMatrix44f mCamera; // operacje kamery
	cameraFrame.GetCameraMatrix( mCamera );
	modelViewMatrix.MultMatrix( mCamera );

	glBindTexture( GL_TEXTURE_2D, texture );
	glUseProgram( shaders[ actualShader ] ); // wybranie aktualnego programu cieniowania
	// ustawienie aktualnych warto�ci parametr�w
	glUniform4fv( locOfLight0Ambient[ actualShader ], 1, Light0Ambient ); // sk�adowa ambient �wiat�a 0
	glUniform4fv( locOfLight0Diffuse[ actualShader ], 1, Light0Diffuse ); // sk�adowa diffuse �wiat�a 0
	glUniform4fv( locOfLight0Specular[ actualShader ], 1, Light0Specular ); // sk�adowa specular �wiat�a 0
	glUniform3fv( locLight[ actualShader ], 1, vEyeLight );
	glUniform1i( locTexture[ actualShader ], 0 );
	// Narysowanie sfery
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate( rotTimer.GetElapsedSeconds() * 40.0f, 0.0f, 1.0f, 0.0f ); // obr�t wok� osi Y
	modelViewMatrix.Translate( -2.0f, 0.0f, 0.0f ); // przesuni�cie kuli aby obiega�a torus
	glUniformMatrix4fv( locMVP[ actualShader ], 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix() );
	glUniformMatrix4fv( locMV[ actualShader ], 1, GL_FALSE, transformPipeline.GetModelViewMatrix() );
	glUniformMatrix3fv( locNM[ actualShader ], 1, GL_FALSE, transformPipeline.GetNormalMatrix() );
	glUniform4fv( locOfMaterialAmbient[ actualShader ], 1, Material0Ambient ); // sk�adowa ambient materia�u 0
	glUniform4fv( locOfMaterialDiffuse[ actualShader ], 1, Material0Diffuse ); // sk�adowa diffuse materia�u 0
	glUniform4fv( locOfMaterialSpecular[ actualShader ], 1, Material0Specular ); // sk�adowa specular materia�u 0
	sphereBatch.Draw();
	modelViewMatrix.PopMatrix();
	// Narysowanie torusa
	modelViewMatrix.PushMatrix();
	modelViewMatrix.Rotate( - rotTimer.GetElapsedSeconds() * 10.0f, 0.0f, 1.0f, 0.0f ); // r�wnomierny obr�t wok� osi Y
	glUniformMatrix4fv( locMVP[ actualShader ], 1, GL_FALSE, transformPipeline.GetModelViewProjectionMatrix() );
	glUniformMatrix4fv( locMV[ actualShader ], 1, GL_FALSE, transformPipeline.GetModelViewMatrix() );
	glUniformMatrix3fv( locNM[ actualShader ], 1, GL_FALSE, transformPipeline.GetNormalMatrix() );
	glUniform4fv( locOfMaterialAmbient[ actualShader ], 1, Material1Ambient ); // sk�adowa ambient materia�u 0
	glUniform4fv( locOfMaterialDiffuse[ actualShader ], 1, Material1Diffuse ); // sk�adowa diffuse materia�u 0
	glUniform4fv( locOfMaterialSpecular[ actualShader ], 1, Material1Specular ); // sk�adowa specular materia�u 0
	torusBatch.Draw();
	modelViewMatrix.PopMatrix();

	modelViewMatrix.PopMatrix();
	glutSwapBuffers();
	glutPostRedisplay();
}

void ChangeSize( int w, int h )
{ // Zmiana rozmiaru okna na w * h
	if ( h == 0 )
		h = 1;
	glViewport( 0, 0, w, h ); // okno ekranu o nowych (zadanych) wymiarach
	viewFrustum.SetPerspective( 35.0f, float( w ) / float( h ), 1.0f, 100.0f );
	projectionMatrix.LoadMatrix( viewFrustum.GetProjectionMatrix() );
	transformPipeline.SetMatrixStacks( modelViewMatrix, projectionMatrix );
}

char *SetTitle( void )
{
	static char buffer[ 200 ];
	sprintf( buffer, "GLSL shaders by %s [%s]", authorName, shaderNames[ actualShader ] );
	return( buffer );
}

GLfloat specExp = 128.0;
GLfloat objectHeight = 1.0;
void KeyPressFunc( unsigned char key, int x, int y )
{
	GLint specExpKey = glGetUniformLocation( shaders[ actualShader], "specExp");
	GLint objectHeightKey = glGetUniformLocation( shaders[ actualShader], "objectHeight");
	switch ( key )
	{
		case ' ': // spacja
			actualShader = (ShaderTypes)( ( actualShader + 1 ) % SHADERS_NUM ); // naprzemienne w��czenie kolejnych program�w cieniowania
			glutSetWindowTitle( SetTitle() );
			break;
		case 'q':
				specExp += 10.0f;
				glUniform1f(specExpKey,specExp);
				printf("Zwiekszam specExp\n");
			break;
		case 'Q':
				specExp -= 10.0f;
				glUniform1f(specExpKey,specExp);
				printf("Zmniejszam specExp\n");
			break;
		case 'e':
				objectHeight += 0.02;
				glUniform1f(objectHeightKey,objectHeight);
				printf("Zwiekszam objectHeight\n");
			break;
		case 'E':
				objectHeight -= 0.02;
				glUniform1f(objectHeightKey,objectHeight);
				printf("Zmniejszam objectHeight\n");
			break;
		case 27 : // ESC
			glutDestroyWindow( glutGetWindow() );
			break;
	}
	glutPostRedisplay(); // od�wie�enie okna
}

void SpecialKeys(int key, int x, int y)
{
	switch ( key )
	{
		case GLUT_KEY_LEFT:
			cameraFrame.RotateLocalY( 0.1 );
			break;
		case GLUT_KEY_RIGHT:
			cameraFrame.RotateLocalY( -0.1 );
			break;
		case GLUT_KEY_UP:
			cameraFrame.MoveForward( 0.1f );
			break;
		case GLUT_KEY_DOWN:
			cameraFrame.MoveForward(-0.1f);
			break;
	}        
	glutPostRedisplay(); // od�wie�enie okna
}

int main( int argc, char* argv[] )
{ 
	gltSetWorkingDirectory( argv[ 0 ] );
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( SetTitle() ); // utworzenie okna
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc ); // obs�uga klawiatury
	glutSpecialFunc( SpecialKeys ); // obs�uga klawiszy specjalnych
	glutDisplayFunc( RenderScene );
	GLenum err = glewInit();
	if ( GLEW_OK != err) 
	{
		fprintf( stderr, "GLEW Error: %s\n", glewGetErrorString( err ) );
		return 1;
	}
	SetupRC();    
	glutMainLoop();
	ShutdownRC();
	return 0;
}
