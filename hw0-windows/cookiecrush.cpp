//============================================================================
// Name        : bubble-shooter.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Bubble Shooting...
//============================================================================
#ifndef COOKIE_CRUSH_CPP
#define COOKIE_CRUSH_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include "util.h"
#include<fstream>
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // Cookie radius in pixels...
int mx1; int mx2; int my1; int my2;
const int width = 900, height = 660; // i have set my window size to be 900 x 660
int dball = 0; // difference between cookies center of 0 pixels
int byoffset = bradius + dball; // 2 * bradius + dball; // board yoffset
int cookie_selector[width / 60][height / 60 - 1];
float score = 0;
int setcolor, psetcolor;

enum GameState { // Use to check different states of game...
	Ready, Start, Over, Pause
};
GameState gamestate = Start;

const int ncookies = 6;
enum Cookies {
	CK_BISCUIT, CK_BURGER, CK_CROISSONT, CK_CUPCAKE, CK_DONUT, CK_STAR
};
float dt = 0, lastframe = 0;//to increase the scope and us in display
int mouseSwap = 0;
GLuint texture[ncookies];
GLuint tid[ncookies];
string tnames[] = { "biscuit.png", "burger.png", "croissont.png", "cupcake.png",
"donut.png", "star.png" };
GLuint mtid[ncookies];
int cwidth = 60, cheight = 60; // 60x60 pixels cookies width & height...
int cookieArray[width/60][height/60-1];
//void swap(int&, int&);//function to swap cookies
void RegisterTextures()
/*Function is used to load the textures from the files and display*/
// Please do not change it...
{
	// allocate a texture name
	glGenTextures(ncookies, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);
	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < ncookies; ++i) {

		// Read current cookie

		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cwidth, cheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawCookie(const Cookies &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	//	glTranslatef(0, 0, 0);
	//	glRotatef(-M_PI / 2, 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}

float Distance(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
void Pixels2Cell(int px, int py, int & cx, int &cy) {

}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{

}

void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(0.7/*Red Component*/, 0.5/*Green Component*/,
		0.12/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	// #----------------- Write your code here ----------------------------#
	//write your drawing commands here or call your drawing functions...

	// draw cookies..;
	//-------------------drawing cookies--------------------------
	
	//----------------------actual game in rady state-----------------------------------
	if (gamestate==Ready)
	{	for (int row = 0; row < width / 60; row++)
		{
			for (int column = 0; column < height / 60 - 1; column++)
			{
				//DrawCookie(cookiesType[cookieArray[row][column]],row*60,column*60,cwidth,cheight);
				int cookie;
				cookie = cookieArray[row][column];
				switch (cookie)
				{
				case 0:
					DrawCookie(CK_BISCUIT, row * 60, column * 60, cwidth, cheight);
					break;
				case 1:
					DrawCookie(CK_BURGER, row * 60, column * 60, cwidth, cheight);
					break;
				case 2:
					DrawCookie(CK_CROISSONT, row * 60, column * 60, cwidth, cheight);
					break;
				case 3:
					DrawCookie(CK_STAR, row * 60, column * 60, cwidth, cheight);
					break;
				case 4:
					DrawCookie(CK_CUPCAKE, row * 60, column * 60, cwidth, cheight);
					break;
				case 5:
					DrawCookie(CK_DONUT, row * 60, column * 60, cwidth, cheight);
					break;
				}
			}
		}
		//---------------------------traveersing cookies array--------------------------------------
		for (int rowTraverse = 0; rowTraverse < width / 60; rowTraverse++)
		{
			for (int columnTraverse = 0; columnTraverse < height / 60 - 1; columnTraverse++)
			{
				//------------------checking for columns----------------------------
				if ((cookieArray[rowTraverse][columnTraverse]
					== cookieArray[rowTraverse][columnTraverse + 1])
					&& (cookieArray[rowTraverse][columnTraverse]
					== cookieArray[rowTraverse][columnTraverse + 2]))//checking for three
				{
					if ((cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse - 1][columnTraverse])
						&& (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse - 2][columnTraverse]))
					{
						cookieArray[rowTraverse][columnTraverse] = 6;
						cookieArray[rowTraverse][columnTraverse+1] = 6;
						cookieArray[rowTraverse][columnTraverse+2] = 6;
						cookieArray[rowTraverse-1][columnTraverse] = 6;
						cookieArray[rowTraverse-2][columnTraverse] = 6;
					}
					if ((cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse + 1][columnTraverse])
						&& (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse + 2][columnTraverse]))
					{
						cookieArray[rowTraverse][columnTraverse] = 6;
						cookieArray[rowTraverse][columnTraverse + 1] = 6;
						cookieArray[rowTraverse][columnTraverse + 2] = 6;
						cookieArray[rowTraverse + 1][columnTraverse] = 6;
						cookieArray[rowTraverse + 2][columnTraverse] = 6;
					}
					if (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse][columnTraverse + 3])//checking for four
					{
						if (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse][columnTraverse + 4])//checking fot five
						{
							if (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse][columnTraverse + 5])//checking for six
							{
								cookieArray[rowTraverse][columnTraverse] = 6;
								cookieArray[rowTraverse][columnTraverse + 1] = 6;
								cookieArray[rowTraverse][columnTraverse + 2] = 6;
								cookieArray[rowTraverse][columnTraverse + 3] = 6;
								cookieArray[rowTraverse][columnTraverse + 4] = 6;
								cookieArray[rowTraverse][columnTraverse + 5] = 6;

							}
							else
							{
								cookieArray[rowTraverse][columnTraverse] = 6;
								cookieArray[rowTraverse][columnTraverse + 1] = 6;
								cookieArray[rowTraverse][columnTraverse + 2] = 6;
								cookieArray[rowTraverse][columnTraverse + 3] = 6;
								cookieArray[rowTraverse][columnTraverse + 4] = 6;

							}

						}
						else
						{
							cookieArray[rowTraverse][columnTraverse] = 6;
							cookieArray[rowTraverse][columnTraverse + 1] = 6;
							cookieArray[rowTraverse][columnTraverse + 2] = 6;
							cookieArray[rowTraverse][columnTraverse + 3] = 6;

						}

					}
					else
					{
						cookieArray[rowTraverse][columnTraverse] = 6;
						cookieArray[rowTraverse][columnTraverse + 1] = 6;
						cookieArray[rowTraverse][columnTraverse + 2] = 6;

					}
				}
				//---------------checking for columns------------------------------
				//---------------checking for rows---------------------------------
				if (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse + 1][columnTraverse] && cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse + 2][columnTraverse])//checking for three
				{
					//mouseSwap = 1;
					if (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse + 3][columnTraverse])//checking for four
					{
						if (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse + 4][columnTraverse])//cheking for five
						{
							if (cookieArray[rowTraverse][columnTraverse] == cookieArray[rowTraverse + 5][columnTraverse])//checking for six
							{
								cookieArray[rowTraverse][columnTraverse] = 6;
								cookieArray[rowTraverse + 1][columnTraverse] = 6;
								cookieArray[rowTraverse + 2][columnTraverse] = 6;
								cookieArray[rowTraverse + 3][columnTraverse] = 6;
								cookieArray[rowTraverse + 4][columnTraverse] = 6;
								cookieArray[rowTraverse + 5][columnTraverse] = 6;
							}
							else
							{
								cookieArray[rowTraverse][columnTraverse] = 6;
								cookieArray[rowTraverse + 1][columnTraverse] = 6;
								cookieArray[rowTraverse + 2][columnTraverse] = 6;
								cookieArray[rowTraverse + 3][columnTraverse] = 6;
								cookieArray[rowTraverse + 4][columnTraverse] = 6;
							}
						}
						else
						{
							cookieArray[rowTraverse][columnTraverse] = 6;
							cookieArray[rowTraverse + 1][columnTraverse] = 6;
							cookieArray[rowTraverse + 2][columnTraverse] = 6;
							cookieArray[rowTraverse + 3][columnTraverse] = 6;
						}
					}
					else
					{
						cookieArray[rowTraverse][columnTraverse] = 6;
						cookieArray[rowTraverse + 1][columnTraverse] = 6;
						cookieArray[rowTraverse + 2][columnTraverse] = 6;
					}
				}
				//--------------------checking for rows-----------------------------------


				//-----------------swapping for random from top------------------------------------
				for (int i = 0; i < width / 60; i++)//i for rows 
				{
					for (int j = 0; j < height / 60 - 1; j++)//j for columns
					{
						if (cookieArray[i][j] == 6)
						{
							for (int k = j; k < height / 60 - 1; k++)
							{
								if (k == height / 60 - 2)
								{
									cookieArray[i][k] = GetRandInRange(0, 5);
									score++;
								}
								else
								{
									int temp1 = cookieArray[i][k];
									cookieArray[i][k] = cookieArray[i][k + 1];
									cookieArray[i][k + 1] = temp1;
								}
							}
						}
					}
				}

			}
		}

		DrawString(width / 60 * 3, height - 45, width, height,
			"Score " + Num2Str(score), colors[BLUE]);
		DrawString(width / 60 * 20, height - 45, width, height, "COOKIE CRUSH", colors[BLUE]);
		DrawString(width / 60 * 50, height - 45, width, height, "Timer " + Num2Str(120 - lastframe), colors[BLUE]);
	}
	//mouseSwap = 0;
	//----------------------actual game in ready state-----------------------------
	//-----------------ending game after 120 seconds---------------------------------
	if (lastframe == 120)
	{
		gamestate = Over;
	}
	//-----------------ending game after 120 seconds---------------------------------
	
	//-------------------when game starts--------------------------------------------
	if (gamestate == Start)
	{
		DrawString(width / 2-200, height / 2, width, height, "Click any where to start the game", colors[BLUE]);
	}
	if (gamestate == Over)
	{
		DrawString(width / 2 - 200, height / 2, width, height, "Game Over", colors[BLUE]);
		DrawString(width / 2 - 200, height / 2-50, width, height, "Your Score = " +Num2Str(score), colors[BLUE]);
	}

	// #----------------- Write your code till here ----------------------------#
	// do not modify below this
	DrawString(width, height, width, height, "", colors[WHITE_SMOKE]);
	glutSwapBuffers(); // do not modify this line..

}
/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {

	cout << "Current Mouse Coordinates X=" << x << " Y= " << height - y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {
	

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		
		if (state == GLUT_DOWN && gamestate == Ready) {
			//cout << "Left Mouse Button Pressed at Coordinates X=" << x /60<< " Y= "
				//<< (height - y) /60<< endl;
			
			mx1 = x/60;
			my1 =(height- y)/60;
			
		}
		if (state == GLUT_UP && gamestate == Ready) {
			//cout << "Left Mouse Button Released at Coordinates X=" << x /60<< " Y= "
				//<<(height-  y)/60 << endl;
			
			mx2 = x / 60;
			my2 = (height - y) / 60;
			if ((abs(mx1 - mx2) == 1) || (abs(my1 - my2) == 1))
			{
				if ((abs(mx1 - mx2) == 1) && (abs(my1 - my2) == 1))
				{
					//so that nothing happens in diognals
				}
				else
				{
					int temp = cookieArray[mx1][my1];
					cookieArray[mx1][my1] = cookieArray[mx2][my2];
					cookieArray[mx2][my2] = temp;
				}
				
			}
			
		}
		if (gamestate == Start)
		{
			gamestate = Ready;
		}
		int mx1=-1, mx2=-1, my1=-1, my2=-1;
		

	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
		
	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
	if ((key == 83 || key == 115)&& gamestate==Start)
	{
		gamestate = Ready;
	}

}

/*
* This function is called after every 1000.0 milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {
	
		dt = (m - lastframe) / 1000.0;
		lastframe = m;

		// dt is time elapsed between two frames..
		glutPostRedisplay();

		// implement your functionality here
		cout << "dt = " << dt << endl;
		cout << "lastframe = " << lastframe << endl;
		cout << "m = " << m << endl;

		glutTimerFunc(1000, Timer, m + 1);
	
}


/*
* our gateway main function
* */

int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...
	//Write your code here for filling the canvas with different Cookies. You can use the GetCookie function for getting the random shaped cookies
	
		for (int i = 0; i < width / 60; i++)
		{
			for (int j = 0; j < height / 60 - 1; ++j)
			{
				cookieArray[i][j] = GetRandInRange(0, 5);
			}
		}
	

	glutInit(&argc, argv); // initialize the graphics library...

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Cookie Crush"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Load the textures from the file..;
	RegisterTextures();
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.

	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	//glutPassiveMotionFunc(MouseMoved);
	//// This function tells the library to call our Timer function after 1000.0 milliseconds...
	glutTimerFunc(1000, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
