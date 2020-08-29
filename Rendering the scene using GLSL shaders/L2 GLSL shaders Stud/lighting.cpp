// lighting.c
// By M.Szwoch, Based on
// OpenGL SuperBible, Chapter 17
// Demonstrates vertex and fragment shaders together
// Program by Benjamin Lipchak

#include "../shared/gltools.h"   // OpenGL toolkit
#include "../shared/glFrame.h"   // Frame toolkit

#include <stdio.h>

// 3. Dodaj swoje imiê i nazwisko do belki tytu³owej
// ...
const char *authorName = "Gall Anonim";

enum ShaderTypes { SIMPLE, DIFFUSE, SPECULAR, THREELIGHTS, FOG, PARAMS, SHADERS_NUM };

GLuint vShader[ SHADERS_NUM ]; // identyfikatory programów cieniowania wierzcho³ków (VS)
GLuint fShader[ SHADERS_NUM ]; // identyfikatory programów cieniowania fragmentów (FS)
GLuint progObj[ SHADERS_NUM ]; // obiekty programów cieniowania (VS+FS)
GLboolean needsValidation[ SHADERS_NUM ]; // informacja o aktualnoœci programu cieniownaia
char *shaderNames[ SHADERS_NUM ] = 
{ // nazwy programów cieniowania
	"simple", 
	"diffuse", 
	"specular", 
	"3lights",
	"fog",
	"params"
};
// aktualny program cieniowania
ShaderTypes actualShader = SIMPLE;
// rozmiary okna
GLint windowWidth = 1024;
GLint windowHeight = 768;

GLint mainMenu, shaderMenu;             // menu handles

GLfloat cameraPos[] = { 100.0f, 75.0f, 150.0f, 1.0f };
GLdouble cameraZoom = 0.4;

GLfloat lightPos0[] = { 140.0f, 250.0f, 140.0f, 1.0f };
GLfloat lightPos1[] = { -140.0f, 250.0f, 140.0f, 1.0f };
GLfloat lightPos2[] = { 0.0f, 250.0f, -200.0f, 1.0f };

GLfloat lightRotation = 0.0f;

#define MAX_INFO_LOG_SIZE 2048
GLchar infoLog[ MAX_INFO_LOG_SIZE ]; // bufor na b³êdy obs³ugi programów cieniowania
void FatalError( char *errorKind, char *errorDescription, char *errorDetails )
{ // wyœwietlenie informacji o b³êdzie i zakoñczenie programu ("radykalna" obs³uga b³êdów)
	fprintf( stderr, "!!! %s ERROR !!!\n", errorKind );
	fprintf( stderr, " %s \"%s\"\n", errorDescription, errorDetails );
	Sleep( 5000 );
	exit( 0 );
}
GLchar *LoadShaderText( const char *fileName )
{ // Za³adowanie z pliku kodu Ÿród³owego programu cieniowania jako tekstu w formacie ASCIIZ
	GLchar *shaderText = NULL;
	GLint shaderLength = 0;
	FILE *fp;
	fp = fopen( fileName, "r" );
	if ( fp != NULL )
	{
		while ( fgetc( fp ) != EOF )
			shaderLength++;
		rewind( fp );
		shaderText = (GLchar *)malloc( shaderLength + 1 );
		if ( shaderText != NULL )
			fread( shaderText, 1, shaderLength, fp );
		shaderText[ shaderLength ] = '\0';
		fclose( fp );
	}
	return shaderText;
}
void LoadAndCompileShader( char * fullFileName, GLuint shaderKind, GLuint *shaderNum )
{ // za³adowanie programu cieniowania z dysku i jego kompilacja
	// za³adowanie programu cieniowania z dysku
	GLint success;
	GLchar *shString = LoadShaderText( fullFileName ); // za³adowanie kodu Ÿród³owego
	fprintf( stdout, "Loading '%s' shader\n", fullFileName );

	if ( !shString )
		FatalError( "LOADING", "Unable to load", fullFileName );

	// 5. Utworzenie obiektu cieniowania funkcj¹ glCreateShader()
	// Parametrem jest rodzaj obiektu cieniowania (shaderKind)
	// Wynik wywo³ania nale¿y podstawiæ do identyfikator obiektu (*shaderNum)
	// ...
	*shaderNum = glCreateShader(shaderKind);

	// 6. Wskazanie kodu Ÿród³owego obiektu cieniowania funkcj¹ glShaderSource()
	// Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	// drugim liczba obiektów (1), 
	// trzecim wskaŸnik na wskaŸnik (const GLchar **)& na kod Ÿród³owy
	// czwartym NULL
	// ...
	glShaderSource(*shaderNum,1,(const GLchar **)&shString,NULL);


	free( shString );

	// 7. Kompilacja obiektu cieniowania funkcj¹ glCompileShader()
	// Parametrem jest identyfikator obiektu cieniowania, 
	// ...
	glCompileShader(*shaderNum);


	// 8. Sprawdzenie statusu kompilacji funkcj¹ glGetShaderiv()
	// Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	// drugim rodzaj sprawdzanego b³êdu (GL_COMPILE_STATUS)
	// trzecim adres zmiennej na zwracany kod b³êdu (success)
	// ...
	glGetShaderiv(*shaderNum,GL_COMPILE_STATUS,&success);


	// 9. Informacja o ewentualnym b³êdzie w przypadku, gdy success == 0
	// Tekst komunikatu pobierany jest funkcj¹ glGetShaderInfoLog()
	//  Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	//  drugim rozmiar bufora (MAX_INFO_LOG_SIZE)
	//  trzecim NULL
	//  czwartym bufor (infoLog)
	// Tekst b³êdu nale¿y wypisaæ funkcj¹ FatalError()
	// ...
		if(success == 0){
			glGetShaderInfoLog(*shaderNum,MAX_INFO_LOG_SIZE,NULL,infoLog);
			FatalError("Kompilacja Shadera",infoLog,fullFileName);
		}





}
void PrepareShader( GLint shaderNum )
{ // Przygotowanie programu cieniowania
	char fullFileName[ 255 ];
	GLint success;

	// Okreœlenie w fullFileName nazwy wzglêdnej pliku ze wskazanym przez shaderNum programem cieniowania wierzcho³ków
	sprintf( fullFileName, "./shaders/%s.vs", shaderNames[ shaderNum ] );

	// 11. Za³adowanie i kompilacja programu cieniowania wierzcho³ków funkcj¹ LoadAndCompileShader()
	// Pierwszym parametrem jest nazwa pliku,
	// drugim rodzaj programu cieniowania (GL_VERTEX_SHADER) 
	// trzecim adres odpowiedniego pola w tablicy identyfikatorów obiektów cieniowania vShader
	// ...
	LoadAndCompileShader(fullFileName,GL_VERTEX_SHADER,&vShader[shaderNum]);

	// 12. Okreœlenie nazwy wzglêdnej pliku ze wskazanym (shaderNum) programem cieniowania fragmentów
	// Pliki maj¹ rozszerzenie "fs"
	// ...
	sprintf( fullFileName, "./shaders/%s.fs", shaderNames[ shaderNum ] );

	// 13. Za³adowanie i kompilacja programu cieniowania fragmentów: (GL_FRAGMENT_SHADER, fShader)
	// ...
	LoadAndCompileShader(fullFileName,GL_FRAGMENT_SHADER,&fShader[shaderNum]);

	// 14. Utworzenie obiektu programu cieniowania funkcj¹ glCreateProgram()
	// Identyfikator obiektu programu cieniowania nale¿y podstawiæ do odpowiedniego miejsca tablicy progObj
	// ...
	progObj[shaderNum] = glCreateProgram();

	// 15. Do³¹czenie do obiektu programu cieniowania skompilowanego kodu VS funkcj¹ glAttachShader()
	// Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	// drugim parametrem identyfikator obiektu VS
	// ...
	glAttachShader(progObj[shaderNum],vShader[shaderNum]);

	// 16. Do³¹czenie do obiektu programu cieniowania skompilowanego kodu FS
	// ...
	glAttachShader(progObj[shaderNum],fShader[shaderNum]);

	// 17. £¹czenie programu cieniowania funkcj¹ glLinkProgram()
	// Parametrem jest identyfikator obiektu programu cieniowania
	// ...
	glLinkProgram(progObj[shaderNum]);

	// 18. Sprawdzenie statusu ³¹czenia funkcj¹ glGetProgramiv()
	// Pierwszym parametrem jest identyfikator obiektu programu cieniowania, 
	// drugim rodzaj sprawdzanego b³êdu (GL_LINK_STATUS)
	// trzecim adres zmiennej na zwracany kod b³êdu (success)
	// ...
	glGetProgramiv(progObj[shaderNum],GL_LINK_STATUS,&success);


	// 19. Informacja o ewentualnym b³êdzie w przypadku, gdy success == 0
	// SHADER LINKING", "Unable to link"
	// ...
		if(success == 0)
		{
			FatalError("SHADER LINKING","Unable to link",fullFileName);
		}





	// 20. Ustawienie w odpowiednim miejscu tablicy needsValidation[] 
	// informacji o koniecznoœci walidacji programu cieniowania (GL_TRUE)
	// ...
	needsValidation[shaderNum] = GL_TRUE;

}
// ------------------------------------------------------------------------
void DrawModels(void)
{ // narysowanie obiektów na scenie, ustawienie parametrów Uniform dla jednostek cieniowania
	M3DVector3f lightPos0Eye, lightPos1Eye, lightPos2Eye;
	M3DMatrix44f mv;

	// transformacja œwiat³a do przestrzeni kamery (eye space)
	glPushMatrix();
	glRotatef( lightRotation, 0.0, 1.0, 0.0 );
	glGetFloatv( GL_MODELVIEW_MATRIX, mv );
	m3dTransformVector3( lightPos0Eye, lightPos0, mv );


	if ( actualShader >= THREELIGHTS )
	{
		m3dTransformVector3( lightPos1Eye, lightPos1, mv );
		m3dTransformVector3( lightPos2Eye, lightPos2, mv );
	}

	glPopMatrix();
	GLint uniformLoc;

	// 29. Pobranie identyfikatora zmiennej Uniform dla Ÿród³a œwiat³a nr 0 funkcj¹ glGetUniformLocation() 
	// Pierwszym parametrem jest aktualny identyfikator obiektu programu cieniowania
	// drugim parametrem jest nazwa zmiennej Uniform "lightPos[0]"
	// Wynik nale¿y podstawiæ do zmiennej uniformLoc
	// ...
	uniformLoc = glGetUniformLocation(progObj[actualShader],"lightPos[0]");


	// 30. Je¿eli zwrócony identyfikator jest ró¿ny od -1, ustawienie aktualnej pozycji œwiat³a 0 funkcj¹ glUniform3fv()
	// Pierwszym parametrem jest identyfikator zmiennej, 
	// drugim parametrem jest liczba argumentów (1)
	// trzecim parametrem wektor wspó³rzêdnych oœwietlenia o (lightPos0Eye)
	// ...
	if(uniformLoc != -1)
		glUniform3fv(uniformLoc,1,lightPos0Eye);



	// 37. Analogiczne ustawienie parametru dla œwiat³a nr 1
	// ...
	/*uniformLoc = glGetUniformLocation(actualShader,"lightPos[1]");
	if(uniformLoc != -1)
		glUniform3fv(1,1,lightPos1Eye);*/


	// 37. Analogiczne ustawienie parametru dla œwiat³a nr 2
	// ...
	/*uniformLoc = glGetUniformLocation(actualShader,"lightPos[2]");
	if(uniformLoc != -1)
		glUniform3fv(2,1,lightPos2Eye);*/



	// Narysowanie niebieskiej podstawy obiektów
	glColor3f( 0.0f, 0.0f, 0.90f );
	glNormal3f( 0.0f, 1.0f, 0.0f );
	glBegin( GL_QUADS );
		glVertex3f( -100.0f, -25.0f, -100.0f );
		glVertex3f( -100.0f, -25.0f, 100.0f );		
		glVertex3f( 100.0f,  -25.0f, 100.0f );
		glVertex3f( 100.0f,  -25.0f, -100.0f );
	glEnd();
	// czerwony szeœcian
	glColor3f( 1.0f, 0.0f, 0.0f );
	glutSolidCube( 48.0f );
	// zielona sfera
	glColor3f( 0.0f, 1.0f, 0.0f );
	glPushMatrix();
	glTranslatef( -60.0f, 0.0f, 0.0f );
	glutSolidSphere( 25.0f, 50, 50 );
	glPopMatrix();
	// bordowy torus
	glColor3f( 1.0f, 0.0f, 1.0f );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, 60.0f );
	glutSolidTorus( 8.0f, 16.0f, 50, 50 );
	glPopMatrix();
	// ¿ó³ty sto¿ek
	glColor3f( 1.0f, 1.0f, 0.0f );
	glPushMatrix();
	glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
	glTranslatef( 60.0f, 0.0f, -24.0f );
	glutSolidCone( 25.0f, 50.0f, 50, 50 );
	glPopMatrix();
	// morski czajniczek
	glColor3f( 0.0f, 1.0f, 1.0f );
	glPushMatrix();
	glTranslatef( 0.0f, 0.0f, -60.0f );
	glutSolidTeapot( 25.0f );
	glPopMatrix();
}
void RenderScene( void )
{ // rysowanie sceny
	// ustawienie perspektywy
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	if ( windowWidth > windowHeight )
	{
		GLdouble ar = (GLdouble)windowWidth / (GLdouble)windowHeight;
		glFrustum( -ar * cameraZoom, ar * cameraZoom, -cameraZoom, cameraZoom, 1.0, 1000.0 );
	}
	else
	{
		GLdouble ar = (GLdouble)windowHeight / (GLdouble)windowWidth;
		glFrustum( -cameraZoom, cameraZoom, -ar * cameraZoom, ar * cameraZoom, 1.0, 1000.0 );
	}
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( cameraPos[ 0 ], cameraPos[ 1 ], cameraPos[ 2 ], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
	glViewport( 0, 0, windowWidth, windowHeight );
	// wyczyszczenie okna
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	GLint success;
	// 20. Walidacja aktualnego (actualShader) programu cieniowania. 
	// WY£¥CZNIE w przypadku, gdy potrzebuje walidacji needsValidation[]
	// UWAGA! actualShader jest indeksem!!!!!
	// ...
	if ( needsValidation[ actualShader ] )
	{

		// 21. Walidacja aktualnego (actualShader) programu cieniowania funkcj¹ glValidateProgram()
		// Parametrem jest aktualny identyfikator obiektu programu cieniowania
		// ...
		glValidateProgram(progObj[ actualShader ]);

		// 22. Sprawdzenie statusu walidacji funkcj¹ glGetProgramiv()
		// Pierwszym parametrem jest identyfikator obiektu programu cieniowania, 
		// drugim rodzaj sprawdzanego b³êdu (GL_VALIDATE_STATUS)
		// trzecim adres zmiennej na zwracany kod b³êdu (success)
		// ...
		glGetProgramiv(progObj[ actualShader ],GL_VALIDATE_STATUS,&success);

		// 23. Informacja o ewentualnym b³êdzie w przypadku, gdy success == 0
		// "SHADER VALIDATION", "Unable to validate"
		// ...
			if(success == 0)
			{
				FatalError("SHADER VLIDATION", "Unable to validate","-");
			}





		// 24. Wyzerowanie informacji o potrzebie walidacji aktualnego programu cieniowania
		// ...
		needsValidation[ actualShader ] = GL_FALSE;
	}
	// Narysowanie modeli 
	DrawModels();
	if ( glGetError() != GL_NO_ERROR )
		fprintf( stderr, "GL Error!\n" );
	// narysowanie sceny
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{ // inicjalizacja sceny
	fprintf( stdout, "Masters of GLSL shaders (2016)\n\n" );
	// 4. Sprawdzenie spe³nienia minimalnych wymogów wersji cieniowania SM (shadermodel)
	if ( !GLEE_VERSION_2_0 && ( !GLEE_ARB_fragment_shader || !GLEE_ARB_vertex_shader || 
				!GLEE_ARB_shader_objects || !GLEE_ARB_shading_language_100 ) )
		FatalError( "OpenGL version", "OpenGL version", "earlier then 2.0" );

	fprintf( stdout, "Controls:\n" );
	fprintf( stdout, "\tR/L arrows\t+/- rotate lights\n\n" );
	fprintf( stdout, "\tx/X\t\tMove +/- in x direction\n" );
	fprintf( stdout, "\ty/Y\t\tMove +/- in y direction\n" );
	fprintf( stdout, "\tz/Z\t\tMove +/- in z direction\n\n" );
	fprintf( stdout, "\tq, ESC\t\tExit demo\n\n" );

	// ustawienia OpenGL
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); // czarne t³o
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );

	// 25. Wczytanie i przygotowanie wszystkich SHADERS_NUM programów cieniowania funkcj¹ PrepareShader()
	// ...
	for (int i = 0; i < SHADERS_NUM; i++) 
	{
		PrepareShader(i);
	}


	// 26. Ustawienie aktualnego (actualShader) programu cieniowania funkcj¹ glUseProgram()
	// Parametrem jest identyfikator aktualnego programu cieniowania
	// UWAGA! actualShader jest indeksem!!!!!
	// ...
	glUseProgram(progObj[actualShader]);

}
char *SetTitle( void )
{
	static char buffer[ 200 ];
	sprintf( buffer, "Master of GLSL shaders by %s [%s]", authorName, shaderNames[ actualShader ] );
	return( buffer );
}
GLfloat pom = 0.1;
void KeyPressFunc( unsigned char key, int x, int y )
{
	GLint uniformLoc;
	
	uniformLoc = glGetUniformLocation(progObj[actualShader],"density");
	switch ( key )
	{
		case ' ': // spacja
			actualShader = (ShaderTypes)( ( actualShader + 1 ) % SHADERS_NUM ); // naprzemienne w³¹czenie kolejnych programów cieniowania
			glutSetWindowTitle( SetTitle() );

			// 27. Ustawienie nowego (actualShader) programu cieniowania funkcj¹ glUseProgram()
			// Parametrem jest identyfikator aktualnego programu cieniowania
			// ...
			glUseProgram(progObj[actualShader]);

			break;
		case 'x':
			cameraPos[ 0 ] += 5.0f;
			break;
		case 'X':
         cameraPos[0] -= 5.0f;
			break;
		case 'y':
			cameraPos[ 1 ] += 5.0f;
			break;
		case 'Y':
			cameraPos[ 1 ] -= 5.0f;
			break;
		case 'z':
			cameraPos[ 2 ] += 5.0f;
			break;
		case 'Z':
			cameraPos[2] -= 5.0f;
			break;
		case 'q':

			pom += 0.1;
				glUniform1f(uniformLoc,pom);

			break;
		case 'Q':
			pom -= 0.1;
				glUniform1f(uniformLoc,pom);

			break;
		case 27 : /* ESC */
			glutDestroyWindow( glutGetWindow() );
			break;
	}
	glutPostRedisplay(); // odœwie¿enie okna
}
void SpecialKeys(int key, int x, int y)
{
	switch ( key )
	{
		case GLUT_KEY_LEFT:
			lightRotation -= 5.0f;
			break;
		case GLUT_KEY_RIGHT:
			lightRotation += 5.0f;
			break;
		case GLUT_KEY_UP:
			cameraPos[ 1 ] += 5.0f;
			break;
		case GLUT_KEY_DOWN:
			cameraPos[1] -= 5.0f;
			break;
	}
	glutPostRedisplay(); // odœwie¿enie okna
}
void ChangeSize( int w, int h )
{
	windowWidth = w;
	windowHeight = h;
}

int main( int argc, char* argv[] )
{
	GLint i;
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( windowWidth, windowHeight );
	glutCreateWindow( SetTitle() ); // utworzenie okna
	glutReshapeFunc( ChangeSize );
	glutKeyboardFunc( KeyPressFunc );
	glutSpecialFunc( SpecialKeys );
	glutDisplayFunc( RenderScene );
	SetupRC();
	glutMainLoop();

	if ( glDeleteProgram && glDeleteShader )
	{
		// 26. Zwolnienie pamiêci dla wszystkich (SHADERS_NUM) obiektów programu cieniowania 
		// oraz jednostek cieniowania VS i FS funkcjami glDeleteProgram() oraz glDeleteShader()
		// ...
		for ( i = 0; i < SHADERS_NUM; i++ )
		{
			glDeleteProgram( progObj[ i ] );
			glDeleteShader( vShader[ i ] );
			glDeleteShader( fShader[ i ] );
		}
	}
	return 0;
}
