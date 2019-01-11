#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <GL/glut.h>

#define numOfTiles 72
#define NUM_OF_FACES 14




static GLuint names[15];


typedef struct{
	float x,y,z;
	int unmatched;
	float r,g,b;
	int face;
}Tile;

typedef struct {
  unsigned short type;
  unsigned int size;
  unsigned short reserved1;
  unsigned short reserved2;
  unsigned int offsetbits;
} BITMAPFILEHEADER;


typedef struct {
  unsigned int size;
  unsigned int width;
  unsigned int height;
  unsigned short planes;
  unsigned short bitcount;
  unsigned int compression;
  unsigned int sizeimage;
  int xpelspermeter;
  int ypelspermeter;
  unsigned int colorsused;
  unsigned int colorsimportant;
} BITMAPINFOHEADER;

typedef struct Image {
  int width, height;
  char *pixels;
} Image;

int* values;
int* indices;
void initialise();
void shuffle(int *array, int n);
int check_availability(int index);
void printFaces();
void printArray(int * array,int n){
	for(int i=0;i<n;i++)
		printf("%d \n",array[i] );
}


static void on_click(int button, int state,int x, int y);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

Tile* tiles;
Image* image;

Image *image_init(int width, int height);
void image_done(Image *image);
void image_read(Image *image, char *filename);
void generateTexture(int i);


void glutTexturedSolidCube(GLdouble size, int texture);
static void drawBox(GLfloat size, GLenum type, int texture);

int selected=0;
int indexOfSelected=0;

/******************************* 
		OSLOBODI MEMORIJU


*******************************/

int main(int argc, char **argv){
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
    printFaces();
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

void printFaces(){

	for(int i=0;i<numOfTiles;i++){
		printf("%d: %d\n",i,tiles[i].face );
	}
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
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glGenTextures(15, names);

    /* Kreira se druga tekstura. */
    for(int i=0;i<15;i++)
        generateTexture(i);
    
    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
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
		for(j=0;j<values[i];j++){
			tiles[indices[sum+j]].face=i;
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
			tiles[i].r=1;
			tiles[i].g=0;
			tiles[i].b=0;
		}
		else if(i<44){


			tiles[i].x=((i-8)%6)*1.1+1.1;
			tiles[i].y=0;
			tiles[i].z=1.1*((i-8)/6)+1.1;
			tiles[i].r=0;
			tiles[i].g=1;
			tiles[i].b=0;
		}
		else if(i<60){
			tiles[i].x=((i-44)%4)*1.1+2.2;
			tiles[i].y=1;
			tiles[i].z=1.1*((i-44)/4)+2.2;
			tiles[i].r=0;
			tiles[i].g=0;
			tiles[i].b=1;
		}
		else if(i<64){
			tiles[i].x=((i-60)%2)*1.1+3.3;
			tiles[i].y=2;
			tiles[i].z=1.1*((i-60)/2)+3.3;
			tiles[i].r=0.5;
			tiles[i].g=0.5;
			tiles[i].b=0.5;
			
		}
		else{
			tiles[i].x=(i-64)*1.1;
			tiles[i].y=0;
			tiles[i].z=7.7;
			tiles[i].r=1;
			tiles[i].g=0;
			tiles[i].b=0;          
		}
	
	
            
        }
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
    case 27:
        if(tiles!=NULL)
            free(tiles);
        exit(0);
        break;

    }
}

static void on_click(int button, int state,int x, int y){
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && x>480 && x<820 && y>30 && y<680){
            int i,j;
            i=(y-30)/81;
            j=(int)(x-480)/(41);
            int pom;
            if (i<0  || j<0 || i>7 || j>7)
            	return;
            //printf("Kliknuto na %i %i\n",x,y);
            if(i==0){
              //  printf("kliknuto na %i\n",71-j);
                pom=71-j;
            }
            else if(i==1){
               // printf("kliknuto na %i\n",44-j);
            	pom=44-j;
            }
            else if(i==6){
              //  printf("kliknuto na %i\n",14-j);
                pom=14-j;
            }
            
            else if(i==7){
               // printf("kliknuto na %i\n",7-j);
                pom=7-j;
            }
            else if(j==1){
              //  printf("kliknuto na %i\n",37-(i-2)*6);
                pom=37-(i-2)*6;
            }
            else if(j==6){
               // printf("kliknuto na %i\n",32-(i-2)*6);
                pom=32-(i-2)*6;
            }
            else if(i==2){
            	pom = 61-j;
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                //	printf("kliknuto na %i\n",pom-23);
                	pom=pom-23;
                }
            }

            else if(i==5){
            	pom = 49-j;
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
               // 	printf("kliknuto na %i\n",pom-29);
                	pom=pom-29;
                }
            }

            else if(j==2){
            	pom = 55-4*(i-3);
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                //	printf("kliknuto na %i\n",pom-25-3*(i-3));
                	pom=pom-24-3*(i-3);
                }
            }

            else if(j==5){
            	pom = 52-4*(i-3);
            	if (tiles[pom].unmatched)
                	printf("kliknuto na %i\n",pom);
                else{
                //	printf("kliknuto na %i\n",pom-25-2*(i-3));
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
                	//	printf("kliknuto na %i\n",pom-25-2*(i-3));
                	}
                }
            }
            if(check_availability(pom)){
            	printf("dostupno %d\n",pom);

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

static void on_timer(int value){
    
}

static void on_reshape(int width, int height){
    glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (float) width / height, 1, 1500);
}

static void on_display(void){
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
    
    
    

    if(selected && tiles[indexOfSelected].unmatched){
    	glPushMatrix();
    		glScalef(1,0.3,2);
    		glTranslatef(tiles[indexOfSelected].x,tiles[indexOfSelected].y+0.1,tiles[indexOfSelected].z);
    		glutWireCube(1);
    	glPopMatrix();
    }

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
                glutTexturedSolidCube(1,tiles[i].face);

        	glPopMatrix();
        }
    }

                    glBindTexture(GL_TEXTURE_2D,0);
                   // image_done(image);


    glutSwapBuffers();
}

int check_availability(int index){
    printf("%d\n",index);
    	if(index>59 && index<64)
		return 1;
	if(index>43 && index<60 && ((index-44)%4==0 || (index-44)%4==3))
		return 1;
	if(index>7 && index<44 && ((index-8)%6==0 || (index-8)%6==5))
		return 1;
	if(index==0 || index==7 || index==71 || index==64)
		return 1;
	return (tiles[index-1].unmatched==0 || tiles[index+1].unmatched==0);

}



Image *image_init(int width, int height) {

  Image *image;

  assert(width >= 0 && height >= 0);

  image = (Image *) malloc(sizeof(Image));
  assert(image != NULL);

  image->width = width;
  image->height = height;
  if (width == 0 || height == 0)
    image->pixels = NULL;
  else {
    image->pixels = (char *)malloc(3 * width * height * sizeof(char));
    assert(image->pixels != NULL);
  }
  return image;
}

void image_done(Image *image) {
  free(image->pixels);
  free(image);
}

void image_read(Image *image, char *filename) {

  FILE *file;
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  unsigned int i;
  unsigned char r, g, b, a;

  free(image->pixels);
  image->pixels = NULL;

  assert((file = fopen(filename, "rb")) != NULL);

  fread(&bfh.type, 2, 1, file);
  fread(&bfh.size, 4, 1, file);
  fread(&bfh.reserved1, 2, 1, file);
  fread(&bfh.reserved2, 2, 1, file);
  fread(&bfh.offsetbits, 4, 1, file);

  fread(&bih.size, 4, 1, file);
  fread(&bih.width, 4, 1, file);
  fread(&bih.height, 4, 1, file);
  fread(&bih.planes, 2, 1, file);
  fread(&bih.bitcount, 2, 1, file);
  fread(&bih.compression, 4, 1, file);
  fread(&bih.sizeimage, 4, 1, file);
  fread(&bih.xpelspermeter, 4, 1, file);
  fread(&bih.ypelspermeter, 4, 1, file);
  fread(&bih.colorsused, 4, 1, file);
  fread(&bih.colorsimportant, 4, 1, file);
  image->width = bih.width;
  image->height = bih.height;
  if (bih.bitcount == 24)
    image->pixels = (char *)malloc(3 * bih.width * bih.height * sizeof(char));
  else if (bih.bitcount == 32)
    image->pixels = (char *)malloc(4 * bih.width * bih.height * sizeof(char));
  else {
    fprintf(stderr, "image_read(): Podrzane su samo slike koje po pikselu cuvaju 24 ili 32 bita podataka.\n");
    exit(1);
  }
  assert(image->pixels != NULL);
  if (bih.bitcount == 24)
    for (i = 0; i < bih.width * bih.height; i++) {
      fread(&b, sizeof(char), 1, file);
      fread(&g, sizeof(char), 1, file);
      fread(&r, sizeof(char), 1, file);

      image->pixels[3 * i] = r;
      image->pixels[3 * i + 1] = g;
      image->pixels[3 * i + 2] = b;
    }
  else if (bih.bitcount == 32)
    for (i = 0; i < bih.width * bih.height; i++) {
      fread(&b, sizeof(char), 1, file);
      fread(&g, sizeof(char), 1, file);
      fread(&r, sizeof(char), 1, file);
      fread(&a, sizeof(char), 1, file);

      image->pixels[4 * i] = r;
      image->pixels[4 * i + 1] = g;
      image->pixels[4 * i + 2] = b;
      image->pixels[4 * i + 3] = a;
    }

  fclose(file);
}

void glutTexturedSolidCube(GLdouble size, int texture){
  drawBox(size, GL_QUADS, texture);
}


void generateTexture(int i){
    image = image_init(0, 0);

    char * s =malloc(7*sizeof(char));
    sprintf(s,"%d.bmp",i);
    printf("%s\n",s);
    /* Kreira se prva tekstura. */
    image_read(image, s);

    /* Generisu se identifikatori tekstura. */
    
    glBindTexture(GL_TEXTURE_2D, names[i]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    
    
}

static void drawBox(GLfloat size, GLenum type, int texture){ 
  static GLfloat n[6][3] =
  { 
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  { 
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;
  
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  
    
  for (i = 5; i >= 0; i--) {
    if(i==1){
        glBindTexture(GL_TEXTURE_2D, names[texture]); // Here we use 'texture' argument to specify which texture we want
  
    glBegin(type);
    glNormal3fv(&n[i][0]); //The only thing that is different from glutSolid cube is that we add glTexCoord function call while drawing vertices
            glTexCoord2f(0, 0);
    glVertex3fv(&v[faces[i][0]][0]);
            glTexCoord2f(1, 0);
    glVertex3fv(&v[faces[i][1]][0]);
            glTexCoord2f(1, 1);
    glVertex3fv(&v[faces[i][2]][0]);
            glTexCoord2f(0, 1);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  
  
    glBindTexture(GL_TEXTURE_2D, 0);
  
    }
      

    else{
        glBindTexture(GL_TEXTURE_2D, names[14]); // Here we use 'texture' argument to specify which texture we want
  
        glBegin(type);
        glNormal3fv(&n[i][0]); //The only thing that is different from glutSolid cube is that we add glTexCoord function call while drawing vertices
            glTexCoord2f(0, 0);
        glVertex3fv(&v[faces[i][0]][0]);
            glTexCoord2f(1, 0);
        glVertex3fv(&v[faces[i][1]][0]);
            glTexCoord2f(1, 1);
        glVertex3fv(&v[faces[i][2]][0]);
            glTexCoord2f(0, 1);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
  
  
        glBindTexture(GL_TEXTURE_2D, 0);
    }
  } 
// Disable texturing
}

