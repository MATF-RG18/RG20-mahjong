#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define numOfTiles 72
#define NUM_OF_FACES 14
typedef struct{
	float x,y,z;
	int visible,unmatched;
	float r,g,b;
	int face;
}Tile;


int* values;
int* indices;
void initialise();
void shuffle(int *array, size_t n);
int check_availability(int index);


static void on_click(int button, int state,int x, int y);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

Tile* tiles;

int selected=0;
int indexOfSelected=0;

/******************************* 
		OSLOBODI MEMORIJU


*******************************/

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1200, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    tiles = malloc(numOfTiles*sizeof(Tile));
    if(tiles==NULL){
    	fprintf(stderr,"Tile array allocation failure");
    	exit(EXIT_FAILURE);
    }
    values=malloc(NUM_OF_FACES*sizeof(int));
    if(NULL==values){
       	fprintf(stderr,"Value array allocation failure");
    	exit(EXIT_FAILURE);
 	}

    indices=malloc(numOfTiles*sizeof(int));
    if(NULL==indices){
       	fprintf(stderr,"Index array allocation failure");
    	exit(EXIT_FAILURE);
 	}

    initialise();
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutMouseFunc(on_click);
    glutDisplayFunc(on_display);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
  
    glutMainLoop();

    return 0;
}

void shuffle(int *array, int n){
    if (n > 1) {
        int i;
        for (i=0;i<n-1; i++){
          int j = i + rand()/(RAND_MAX/(n-i)+1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

void initialise(){
	int i,j;
	for(i=0;i<NUM_OF_FACES;i++){
		if(i<7)
			values[i]=4;
		else if (i<13)
			values[i]=6;
		else
			values[i]=8;
	}
	for(i=0;i<numOfTiles;i++)
		indices[i]=i;

	shuffle(values,NUM_OF_FACES);
	shuffle(indices,numOfTiles);
	int sum=0;
	for(i=0;i<NUM_OF_FACES;i++){
		for(j=0;j<values[i];i++){
			tiles[indices[sum+j]]=i;
		}
		sum+=values[i];
	}

	free(values);
	free(indices);	

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

static void on_click(int button, int state,int x, int y){
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && x>480 && x<800 && y>30 && y<680){
            int i,j;
            i=(y-30)/81;
            j=(int)(x-480)/(39);
            int pom;
            printf("Kliknuto na %i %i\n",x,y);
            if(i==0){
                printf("kliknuto na %i\n",71-j);
                pom=71-j;
            }
            else if(i==1){
                printf("kliknuto na %i\n",44-j);
            	pom=44-j;
            }
            else if(i==6){
                printf("kliknuto na %i\n",14-j);
                pom=14-j;
            }
            
            else if(i==7){
                printf("kliknuto na %i\n",7-j);
                pom=7-j;
            }
            else if(j==1){
                printf("kliknuto na %i\n",37-(i-2)*6);
                pom=37-(i-2)*6;
            }
            else if(j==6){
                printf("kliknuto na %i\n",32-(i-2)*6);
                pom=32-(i-2)*6
            }
            else if(i==2){
            	pom = 61-j;
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                	printf("kliknuto na %i\n",pom-23);
                	pom=pom-23;
                }
            }

            else if(i==5){
            	pom = 47-j;
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                	printf("kliknuto na %i\n",pom-29);
                	pom=pom-29;
                }
            }

            else if(j==2){
            	pom = 55-4*(i-3);
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                	printf("kliknuto na %i\n",pom-25-3*(i-3));
                	pom=pom-25-3*(i-3);
                }
            }

            else if(j==5){
            	pom = 52-4*(i-3);
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                	printf("kliknuto na %i\n",pom-25-2*(i-3));
                	pom=pom-25-2*(i-3);
                }
            }

            else{
            	pom = 63-2*(i-3)-(j-3);
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                	pom = pom -9-2*(i-3);
                	if (tiles[pom].unmatched)
                		printf("kliknuto na %i\n",pom);
                	else{
                		pom=pom-25-2*(i-3);
                		printf("kliknuto na %i\n",pom-25-2*(i-3));
                	}
                }
            }
            if(check_availability(pom)){

            	if(selected){
            		if(pom==indexOfSelected){
            			selected=0;
            		}
            		else if(tiles[pom].face==tiles[indexOfSelected].face){
            			tiles[pom].unmatched=0;
            			tiles[indexOfSelected].unmatched=0;
            			selected=0;
            		}
            	}
            	else{
            		selected=1;
            		indexOfSelected=pom;
            	}
            	glutPostRedisplay();
        	}
            
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
     GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1 };

    GLfloat ambient_coeffs[] = { 0.4, 0.4, 0.4, 1 };
    GLfloat diffuse_coeffs[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat shininess = 20;
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
   
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	GLfloat light_position[] = {5,15,6, 1 };
	 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    gluLookAt(5,15,6, 5, 2.5, 7, 0, 0,1);
    
    
    


    int i;
    for(i=0;i<numOfTiles;i++){
        if(tiles[i].unmatched){
        	glPushMatrix();
            	glScalef(1,0.3,2);
            	glTranslatef(tiles[i].x,tiles[i].y,tiles[i].z);
            	/*glColor3f(tiles[i].r,tiles[i].g,tiles[i].b);
                glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
                glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
                glMaterialf(GL_FRONT, GL_SHININESS, shininess);
*/            	
                diffuse_coeffs[1] = 1;
		
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
                glutSolidCube(1);

        	glPopMatrix();
        }
    }
    glutSwapBuffers();
}


int check_availability(int index){
	if(index>54 && index<64)
		return 1;
	if(index>36 && index<49)
		return 1;
	if(index==51 || index==52)
		return 1;
	if(index>7 && index<15)
		return 1;
	if(index>18 && index<33 && ((index-19)%6==0 || (index-19)%6==1))
		return 1;
	if(index==0 || index==7 || index==71 || index==64)
		return 1;
	return (tiles[index-1].unmatched==0 || tiles[index+1].unmatched==0);

}