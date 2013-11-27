#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#endif

#define PI 3.14159265

#include <time.h>
#include <math.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>

float newTime, oldTime;
double mod = 0;
long sec = 0;
long usec = 0;

double camerahRadius = 20;
double Ex = 200;
double Ey = 1;
double Ez = 90;
double lookx = Ex-20;
double lookz = Ez;
double cameraAngle = 180;

float carSpeed = 0;

bool accelerate = false;
bool decelerate = false;
bool turnRight = false;
bool turnLeft = false;

using namespace std;

float toRadians(float angle)
{
    return (angle*PI/180);
}

void applyPhysics()
{
	newTime = glutGet(GLUT_ELAPSED_TIME)*0.001;

	float timestep = newTime-oldTime;

	if(accelerate)
	{
    	carSpeed += (20*timestep);
    	if(carSpeed>55)
    		carSpeed = 55;

	}
	else if(carSpeed>0)
	{
		carSpeed -= (10*timestep);
		if(carSpeed<0)
			carSpeed = 0;
	}
	if(decelerate)
	{
		carSpeed -= (25*timestep);
		if(carSpeed<-13)
			carSpeed = -13;
	}
	else if(carSpeed<0)
	{
		carSpeed += (10*timestep);
		if(carSpeed>0)
			carSpeed = 0;
	}
	if(carSpeed > 0 && (turnRight || turnLeft))
	{
		if(turnRight)
		{
	        cameraAngle += (sqrt(carSpeed*carSpeed)*.25)*(5*timestep);
	        if(cameraAngle>360)
	        	cameraAngle = cameraAngle-360;
		}
		else if(turnLeft)
		{
	    	cameraAngle -= (sqrt(carSpeed*carSpeed)*.25)*(5*timestep);
	    	if(cameraAngle<0)
	    		cameraAngle = 360+cameraAngle;
		}
		if(carSpeed > 20)
			carSpeed -= 20*timestep;

	}
	else if(carSpeed < 0 && (turnRight || turnLeft))
		{
			if(turnRight)
			{
		        cameraAngle -= (sqrt(carSpeed*carSpeed)*.25)*(5*timestep);
		        if(cameraAngle>360)
		        	cameraAngle = cameraAngle-360;
			}
			else if(turnLeft)
			{
		    	cameraAngle += (sqrt(carSpeed*carSpeed)*.25)*(5*timestep);
		    	if(cameraAngle<0)
		    		cameraAngle = 360+cameraAngle;
			}
			if(carSpeed < -20)
				carSpeed += 30*timestep;
		}

	Ex += (carSpeed*timestep)*cos(toRadians(cameraAngle));
	Ez += (carSpeed*timestep)*sin(toRadians(cameraAngle));
    lookx = Ex+camerahRadius*cos(toRadians(cameraAngle));
    lookz = Ez+camerahRadius*sin(toRadians(cameraAngle));

	oldTime = newTime;
}
void drawTrack()
{
    float width = 20;
    float straightAway = 240;
    float stripes = straightAway/80;
    float corner = 40;
    float turn = 80;
    float x = 0;
    float z = 0;
    float importantx = 0;
    float importantz = 0;
    float triangle[4][3];
    triangle[0][1] = 0;
    triangle[1][1] = 0;
    triangle[2][1] = 0;
    triangle[3][1] = 0;

    glPushMatrix();
    glTranslatef(straightAway/2,0,width+corner+(turn/2));

    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(1,.85,0);
    for(int x = -stripes; x>-straightAway; x-=stripes*2)
    {
    	glVertex3f(x,0,0);
    	glVertex3f(x,0,-width);
    	glVertex3f(x-stripes,0,-width);
    	glVertex3f(x-stripes,0,0);
    }
    glColor3f(0,0,0);
    for(int x = 0; x>-straightAway; x-=stripes*2)
    {
    	glVertex3f(x,0,0);
    	glVertex3f(x,0,-width);
    	glVertex3f(x-stripes,0,-width);
    	glVertex3f(x-stripes,0,0);
    }

    triangle[0][0] = -straightAway;
    triangle[0][2] = -width;
    triangle[2][0] = -straightAway;
    triangle[2][2] = 0;

    glVertex3f(-straightAway,0,-width);
    x = -straightAway-sin(toRadians(45))*corner;
    z = -width-corner;
    glVertex3f(x,0,z);
    importantx = x;
    importantz = z;
    x -= cos(toRadians(-45))*width;
    z -= sin(toRadians(-45))*width;
    triangle[3][0] = x;
    triangle[3][2] = z;
    glVertex3f(x,0,z);
    x += sin(toRadians(45))*corner;
    z += corner;
    triangle[1][0] = x;
    triangle[1][2] = z;
    glVertex3f(x,0,z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[2]);
    glEnd();

    triangle[0][0] = importantx;
    triangle[0][2] = importantz;

    glBegin(GL_QUADS);
    x = importantx;
    z = importantz;
    glVertex3f(x,0,z);
    z -= turn;
    importantz = z;
    glVertex3f(x,0,z);
    x -= width;
    triangle[2][0] = x;
    triangle[2][2] = z;
    glVertex3f(x,0,z);
    z += turn;
    triangle[1][0] = x;
    triangle[1][2] = z;
    glVertex3f(x,0,z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[3]);
    glEnd();

    triangle[0][0] = importantx;
    triangle[0][2] = importantz;

    glBegin(GL_QUADS);
    x = importantx;
    z = importantz;
    glVertex3f(x,0,z);
    z -= corner;
    x += sin(toRadians(45))*corner;
    importantx = x;
    importantz = z;
    glVertex3f(x,0,z);
    x -= cos(toRadians(45))*width;
    z -= sin(toRadians(45))*width;
    triangle[3][0] = x;
    triangle[3][2] = z;
    glVertex3f(x,0,z);
    z += corner;
    x -= sin(toRadians(45))*corner;
    triangle[1][0] = x;
    triangle[1][2] = z;
    glVertex3f(x,0,z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[2]);
    glEnd();

    triangle[0][0] = importantx;
    triangle[0][2] = importantz;

    glBegin(GL_QUADS);
    x = importantx;
    z = importantz;
    glVertex3f(x,0,z);
    x += straightAway;
    importantx = x;
    glVertex3f(x,0,z);
    z -= width;
    triangle[2][0] = x;
    triangle[2][2] = z;
    glVertex3f(x,0,z);
    x -= straightAway;
    triangle[1][0] = x;
    triangle[1][2] = z;
    glVertex3f(x,0,z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[3]);
    glEnd();

    triangle[0][0] = importantx;
    triangle[0][2] = importantz;

    glBegin(GL_QUADS);
    x = importantx;
    z = importantz;
    glVertex3f(x,0,z);
    z += corner;
    x += sin(toRadians(45))*corner;
    importantx = x;
    importantz = z;
    glVertex3f(x,0,z);
    x += cos(toRadians(45))*width;
    z -= sin(toRadians(45))*width;
    triangle[3][0] = x;
    triangle[3][2] = z;
    glVertex3f(x,0,z);
    z -= corner;
    x -= sin(toRadians(45))*corner;
    triangle[1][0] = x;
    triangle[1][2] = z;
    glVertex3f(x,0,z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[2]);
    glEnd();

    triangle[0][0] = importantx;
    triangle[0][2] = importantz;

    glBegin(GL_QUADS);
    x = importantx;
    z = importantz;
    glVertex3f(x,0,z);
    z += turn;
    importantz = z;
    glVertex3f(x,0,z);
    x += width;
    triangle[2][0] = x;
    triangle[2][2] = z;
    glVertex3f(x,0,z);
    z -= turn;
    triangle[1][0] = x;
    triangle[1][2] = z;
    glVertex3f(x,0,z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[3]);
    glEnd();

    triangle[0][0] = importantx;
    triangle[0][2] = importantz;

    glBegin(GL_QUADS);
    x = importantx;
    z = importantz;
    glVertex3f(x,0,z);
    z += corner;
    x -= sin(toRadians(45))*corner;
    importantx = x;
    importantz = z;
    glVertex3f(x,0,z);
    z += sin(toRadians(45))*width;
    x += cos(toRadians(45))*width;
    triangle[3][0] = x;
    triangle[3][2] = z;
    glVertex3f(x,0,z);
    z -= corner;
    x += sin(toRadians(45))*corner;
    triangle[1][0] = x;
    triangle[1][2] = z;
    glVertex3f(x,0,z);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[2]);
    triangle[0][0] = importantx;
    triangle[0][2] = importantz;
    triangle[1][0] = 0;
    triangle[1][2] = 0;
    glVertex3fv(triangle[0]);
    glVertex3fv(triangle[1]);
    glVertex3fv(triangle[3]);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0,0.65,0.06);
    glVertex3f(0,0,-width);
    glVertex3f(0,0,-width-turn-(corner*2));
    glVertex3f(-straightAway,0,-width-turn-(corner*2));
    glVertex3f(-straightAway,0,-width);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.93,0.79,0.67);
    glVertex3f(-straightAway,0,-width);
    glVertex3f(-straightAway,0,-width-turn-(corner*2));
    glVertex3f(-straightAway-sin(toRadians(45))*corner,0,-width-turn-corner);
    glVertex3f(-straightAway-sin(toRadians(45))*corner,0,-width-corner);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.93,0.79,0.67);
    glVertex3f(0,0,-width-turn-(corner*2));
    glVertex3f(0,0,-width);
    glVertex3f(sin(toRadians(45))*corner,0,-width-corner);
    glVertex3f(sin(toRadians(45))*corner,0,-width-turn-corner);
    glEnd();

    glPopMatrix();
    glPopMatrix();
}
void drawCar()
{
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(Ex,0,Ez);
    glRotatef(-cameraAngle,0,1,0);

    glPushMatrix();
    glTranslatef(-1.8,0,0);

    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(-2,0,1);
    glVertex3f(2,0,1);
    glVertex3f(2,2,1);
    glVertex3f(-2,2,1);

    glVertex3f(-2,2,1);
    glVertex3f(2,2,1);
    glVertex3f(2,2,-1);
    glVertex3f(-2,2,-1);

    glVertex3f(-2,2,-1);
    glVertex3f(2,2,-1);
    glVertex3f(2,0,-1);
    glVertex3f(-2,0,-1);

    glVertex3f(2,0,1);
    glVertex3f(2,0,-1);
    glVertex3f(2,2,-1);
    glVertex3f(2,2,1);

    glVertex3f(-2,0,1);
    glVertex3f(-2,0,-1);
    glVertex3f(-2,2,-1);
    glVertex3f(-2,2,1);
    glEnd();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void display()
{
	//printf("%f\n",carSpeed);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    applyPhysics();

    gluLookAt(Ex, Ey, Ez, lookx, 1, lookz, 0, 1, 0);

    drawCar();
    drawTrack();

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();

}

void reshape(int width, int height)
{
    float aspect = 1.0*width/height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,width,height);
    gluPerspective(45,aspect, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);

}

void keyboard(unsigned char key, int x, int y)
{
	if(key == 'z')
		Ey = 100;
	else if(key == 'x')
		Ey = 1;
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_UP && !accelerate)
    {
    	accelerate = true;
    }
    if(key == GLUT_KEY_DOWN && !decelerate)
    {
    	decelerate = true;
    }
    if(key == GLUT_KEY_RIGHT && !turnRight)
    {
    	turnRight = true;

        lookx = Ex+camerahRadius*cos(toRadians(cameraAngle));
        lookz = Ez+camerahRadius*sin(toRadians(cameraAngle));
    }
    else if(key == GLUT_KEY_LEFT && !turnLeft)
    {
    	turnLeft = true;

    	lookx = Ex+camerahRadius*cos(toRadians(cameraAngle));
    	lookz = Ez+camerahRadius*sin(toRadians(cameraAngle));
    }

    glutPostRedisplay();
}

void specialup(int key, int x, int y)
{
	if(key == GLUT_KEY_UP && accelerate)
	{
		accelerate = false;
	}
	if(key == GLUT_KEY_DOWN && decelerate)
	{
		decelerate = false;
	}
	if(key == GLUT_KEY_LEFT && turnLeft)
	{
		turnLeft = false;
	}
	if(key == GLUT_KEY_RIGHT && turnRight)
	{
		turnRight = false;
	}

}

void keyup(unsigned char key, int x, int y)
{

}

int main(int argc, char *argv[])
{
    srand(clock());
    rand();
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(500, 50);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Racing");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 1000);

    glClearColor(1, 1, 1, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(Ex, Ey, Ez, 0, 30, 0, 0, 1, 0);

    oldTime = glutGet(GLUT_ELAPSED_TIME)*0.001;


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyup);
    glutSpecialUpFunc(specialup);

    glutMainLoop();
    return 0;
}
