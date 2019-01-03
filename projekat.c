#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>


/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    
  
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    }
}

static void on_timer(int value)
{
    
}

static void on_reshape(int width, int height)
{
    /* Postavlja se viewport. */
    glViewport(0, 0, width, height);

    /* Postavljaju se parametri projekcije. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (float) width / height, 1, 1500);
}

static void on_display(void)
{
    /* Postavlja se boja svih piksela na zadatu boju pozadine. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int ind1=1;
    int ind2=0;
    /* Postavlja se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5,15,6, 5, 2.5, 7, 0, 0,1);
    int i;
    for(i=0;i<8;i++){
        glPushMatrix();

            glScalef(1,0.3,2);
            
            glTranslatef(i*1.1,0,0);
            glColor3f(ind1,ind2,0);
            glutSolidCube(1);

        glPopMatrix();
        ind1=1-ind1;
        ind2=1-ind2;
    }
    int j;
    for(i=0;i<6;i++){
        for(j=0;j<6;j++){
            glPushMatrix();

                glScalef(1,0.3,2);
            
                glTranslatef(1.1+i*1.1,0,j*1.1+1.1);
                glColor3f(ind1,ind2,0);
                glutSolidCube(1);

            glPopMatrix();
        ind1=1-ind1;
        ind2=1-ind2;

        }
    }
    
    for(i=0;i<8;i++){
        glPushMatrix();

            glScalef(1,0.3,2);
            
            glTranslatef(i*1.1,0,7.7);
            glColor3f(ind2,ind1,0);
            glutSolidCube(1);

        glPopMatrix();
        ind1=1-ind1;
        ind2=1-ind2;
    }
     for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            glPushMatrix();

                glScalef(1,0.3,2);
            
                glTranslatef(2.2+i*1.1,1,j*1.1+2.2);
                glColor3f(0,0.5*ind1,ind2);
                glutSolidCube(1);

            glPopMatrix();
        ind1=1-ind1;
        ind2=1-ind2;

        }
    }

    glPushMatrix();

        glScalef(1,0.3,2);
            glTranslatef(3.3,2,3.3);
            glColor3f(0.5,0.5,0.5);
            glutSolidCube(1);

    glPopMatrix();
    
    glPushMatrix();

        glScalef(1,0.3,2);
            glTranslatef(4.4,2,3.3);
            glColor3f(0.5,0.5,0.5);
            glutSolidCube(1);

    glPopMatrix();
    glPushMatrix();

        glScalef(1,0.3,2);
            glTranslatef(3.3,2,4.4);
            glColor3f(0.5,0.5,0.5);
            glutSolidCube(1);

    glPopMatrix();
    
    glPushMatrix();

        glScalef(1,0.3,2);
            glTranslatef(4.4,2,4.4);
            glColor3f(0.5,0.5,0.5);
            glutSolidCube(1);

    glPopMatrix();
    /* Postavlja se nova slika u prozor. */
    glutSwapBuffers();
}
