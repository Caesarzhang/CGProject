#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <math.h>
#include <iostream>
using namespace std;
#define LengthOfCube 60.0
#define LengthOfColumn 1.0
#define j2h(x) (3.1415926*(x)/180.0)
#define StartViewX (LengthOfCube/2+1.5*LengthOfColumn)
#define StartHeadX (LengthOfCube/2+LengthOfColumn/2)
#define HeadCameDistance (1.732051*LengthOfColumn)

bool Transparent = 1;/*判断地图是否透明*/

int Up[2] = { 0,1 }, View[2] = { 1,1 }, Left[2] = { 2,1 };
int Direction[3][2] = { {0,1},{1,1},{2,1} };
/*int Up = 0, USign = 1,
View = 1, VSign = -1 
Leftnode=2,leftsign=1对号入座*/

GLfloat camera[] = { StartViewX,-30,0.0 }, 
		location[] = { StartViewX,50.0,0.0 }, /*摄像机看向的点，也就是摄像机前50像素的位置*/
		angle = 0.0, 
		CameraUp[] = { 1.0,0.0,0.0 }, /*摄像机上向量*/
		head[] = { StartHeadX,HeadCameDistance -30,0.0 },/*虵头*/
		lightPosition[][4] = { { 0.0,-1.0,0.0,0.0 },{ 0.0,0.0,0.0,1.0 } },/*环境光的位置*/
		wallSpecular[] = { 0.3,0.3,0.3,1.0 }, 
		headColor[] = { 0.725,0.494,0.11,1.0 },	
		floorColor[] = { 0.64,0.376,0.2,0.7 },
		wallShininess = 50.0;       //镜面属性 小-粗糙

void init();
void keyboardFunc(unsigned char key, int x, int y);
void display();
void processMousePassiveMotion(int x, int y);
void changeDirection(int Up, int signOfUp, int oldD, int signOfOldD, int newD, int signOfNewD);
void calcLeft();

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("贪吃虵");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFunc);
	//glutPassiveMotionFunc(processMousePassiveMotion);
	init();
	glutMainLoop();
	return 0;
}

void calcLeft()
{
	Left[0] = 3 - View[0] - Up[0];
	int v1[3] = { 0,0,0 };
	int v2[3] = { 0,0,0 };
	v1[Up[0]] = Up[1];
	v2[View[0]] = View[1];
	Left[1] = v1[1] * v2[2] + v1[2] * v2[0] + v1[0] * v2[1] - v1[2] * v2[1] - v1[0] * v2[2] - v1[1] * v2[0];
}

void processMousePassiveMotion(int x, int y)
{
	
}

void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess)
{
	static const GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

void drawWorld()
{
	const static GLfloat blue_color[] = { 0.0f, 0.0f, 0.5f, 0.4f };
	if (Transparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		setMatirial(blue_color, 30.0);

	}	
		glPushMatrix();
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition[1]);
		if(!Transparent)
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, floorColor);
		glutSolidCube(LengthOfCube);
		glPopMatrix();
		glDepthMask(GL_TRUE);
	
}
void debugDisplay()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	int camera0[3] = { 0,0,0 }, view0[3] = { 0,0,0 };
	camera0[Up[0]] = 100 * Up[1];
	view0[View[0]] = View[1]*1.0;
	gluLookAt(camera0[0], camera0[1], camera0[2], 0, 0, 0, view0[0], view0[1], view0[2]);
	glViewport(-80, 100, 400, 400);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 1200, 800);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.5f, 0.1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], head[0], head[1], head[2], CameraUp[0], CameraUp[1], CameraUp[2]); //相机位置,中心位置，上向量
	//gluLookAt(camera[0], camera[1], camera[2], location[0], location[1], location[2], CameraUp[0], CameraUp[1], CameraUp[2]);  //相机位置,中心位置，上向量
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wallSpecular);	//镜面反射颜色
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, wallShininess);//镜面反射指数

	drawWorld();

	glPushMatrix();
	{
		//glTranslated(head[0], head[1], head[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, floorColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, headColor);
		glutSolidCube(40.0);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(head[0], head[1], head[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, headColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, headColor);
		glutSolidCube(0.5);
	}
	glPopMatrix();
	//debugDisplay();
	glutSwapBuffers();
}

void turn(int direction)
{
	double angle = 0.0;
	for (angle = 0.0; angle < 90; angle=angle+0.2)
	{
		camera[View[0]] = head[View[0]] - HeadCameDistance * View[1] * cos(j2h(angle));
		camera[Left[0]] = head[Left[0]] - HeadCameDistance * Left[1] * sin(j2h(angle))*direction;
		glutPostRedisplay();
	}
}
void keyboardFunc(unsigned char key, int x, int y)
{
	angle = 0.0;
	switch (key)
	{
	case'w':
		camera[View[0]] += View[1];
		head[View[0]] += View[1];
		location[View[0]] += View[1];
		break;
	case's':
		camera[View[0]] -= View[1];
		head[View[0]] -= View[1];
		location[View[0]] -= View[1];
		break;
	case'a':
		//camera[View[0]] += HeadCameDistance * View[1] ;
		//camera[Left[0]] -= HeadCameDistance * Left[1] ;
		for (angle = 0.0; angle < 90; angle = angle + 0.04)
		{
			camera[View[0]] = head[View[0]] - HeadCameDistance * View[1] * cos(j2h(angle));
			camera[Left[0]] = head[Left[0]] - HeadCameDistance * Left[1] * sin(j2h(angle));
			glutPostRedisplay();
		}
		swap(View[0],Left[0]);
		swap(View[1], Left[1]);
		Left[1] = -1 * Left[1];
		break;
	case'd':
		turn(-1);
		swap(View[0], Left[0]);
		swap(View[1], Left[1]);
		View[1] = -1 * View[1];
		break;
	}
	glutPostRedisplay();
	cout << View[0] * View[1] << endl << Left[0] * Left[1] << endl << endl;
}

void init()
{
	GLfloat backEmi[] = { 1.0,0.9,0.01,0.9 };
	glClearColor(0.0, 0.0, 0.0, 1.0);//背景颜色
	glClearDepth(1.0);				//从这个depth开始看不见
	glMatrixMode(GL_MODELVIEW);  //设置当前操作的矩阵为“模型视图矩阵”
	glLoadIdentity();            //在进行变换前把当前矩阵设置为单位矩阵
	glShadeModel(GL_SMOOTH);     //光滑着色模式
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);         //启用环境光
	glEnable(GL_DEPTH_TEST);     //根据坐标远近隐藏被遮住的图形
	glLoadIdentity();//在进行变换前把当前矩阵设置为单位矩阵
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition[0]);
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, backEmi);
	glLightfv(GL_LIGHT0, GL_SPECULAR, backEmi);
}