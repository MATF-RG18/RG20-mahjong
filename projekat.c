#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define numOfTiles 72

typedef struct{
	float x,y,z;
	int visible,unmatched;
	float r,g,b;
}Tile;


void initialise();
void ispisi();
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);
Tile* tiles;
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    tiles = malloc(numOfTiles*sizeof(Tile));
    if(tiles==NULL){
    	fprintf(stderr,"Tile array allocation failure");
    	exit(EXIT_FAILURE);
    }
    initialise();
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    
  
    glutMainLoop();

    return 0;
}



void initialise(){
	int i;
	for(i=0;i<numOfTiles;i++){
		tiles[i].unmatched=1;
		if(i<8){
			tiles[i].x=i*1.1;
			tiles[i].y=0;
			tiles[i].z=0;
			tiles[i].visible=1;
			tiles[i].r=1;
			tiles[i].g=0;
			tiles[i].b=0;
		}
		else if(i<44){


			tiles[i].x=((i-8)%6)*1.1+1.1;
			tiles[i].y=0;
			tiles[i].z=1.1*((i-8)/6)+1.1;
			if((i-8)<6 || (i-8)>28 || (i-8)%6==0 || (i-8)%6==5)
				tiles[i].visible=1;
			else
				tiles[i].visible=0;
			tiles[i].r=0;
			tiles[i].g=1;
			tiles[i].b=0;
		}
		else if(i<60){
			tiles[i].x=((i-44)%4)*1.1+2.2;
			tiles[i].y=1;
			tiles[i].z=1.1*((i-44)/4)+2.2;
			if((i-44)<4 || (i-44)>=12 || (i-44)%4==0 || (i-44)%4==3)
				tiles[i].visible=1;
			else
				tiles[i].visible=0;
			tiles[i].r=0;
			tiles[i].g=0;
			tiles[i].b=1;
		}
		else if(i<64){
			tiles[i].x=((i-60)%2)*1.1+3.3;
			tiles[i].y=2;
			tiles[i].z=1.1*((i-60)/2)+3.3;
			tiles[i].visible=1;
			tiles[i].r=0.5;
			tiles[i].g=0.5;
			tiles[i].b=0.5;
			
		}
		else{
			tiles[i].x=(i-64)*1.1;
			tiles[i].y=0;
			tiles[i].z=7.7;
			tiles[i].visible=1;
			tiles[i].r=1;
			tiles[i].g=0;
			tiles[i].b=0;          
		}
	
	}

}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;

    }
}

static void on_timer(int value)
{
    
}

static void on_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (float) width / height, 1, 1500);
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5,15,6, 5, 2.5, 7, 0, 0,1);
    int i;
    for(i=0;i<numOfTiles;i++){
        if(tiles[i].unmatched){
        	glPushMatrix();

            	glScalef(1,0.3,2);
            	glTranslatef(tiles[i].x,tiles[i].y,tiles[i].z);
            	glColor3f(tiles[i].r,tiles[i].g,tiles[i].b);
            	glutSolidCube(1);

        	glPopMatrix();
        }
    }
    glutSwapBuffers();
}
