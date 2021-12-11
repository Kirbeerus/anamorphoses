/********************************************************/
/*                     cube.cpp                                                 */
/********************************************************/
/*                Affiche a l'ecran un thore en 3D                      */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#define _USE_MATH_DEFINES

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */
#endif
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

#define grandR 4
#define petitR 1
#define n 20
#define m 100

void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
char presse;
int anglex,angley,x,y,xold,yold;

/*class Point*/
class Point{
    public :
	//coordonnées x, y et z du point
	double x;
	double y;
	double z;
	// couleur r, v et b du point
	float r;
	float g;
	float b;
};



Point tore[n*m];
int ftore[n*(m+1)][4];

void genererTore(){
    int cpt = 0;
    for(int i = 0;i<n;i++){
        for(int j = 0;j<m;j++){
            Point tmp;
            tmp.x = (grandR+petitR*cos(i*((2*M_PI)/n)))*cos(j*((2*M_PI)/m));
            tmp.y = petitR*sin(i*((2*M_PI)/n));
            tmp.z = (grandR+petitR*cos(i*((2*M_PI)/n)))*sin(j*((2*M_PI)/m));

            tmp.r = 1.0;
            tmp.g = 1.0;
            tmp.b = 1.0;

            tore[cpt] = tmp;
            cpt++;
        }
    }


};

void genereFaceToreQuadra(){
        for(int i=0;i< n*m;i++){
            ftore[i][0] = i;
            ftore[i][1] = (i+1)%(n*m);
            ftore[i][2] = (i+1+m)%(n*m);
            ftore[i][3] = (i+m)%(n*m);
        }

}

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  genererTore();
  genereFaceToreQuadra();
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("cube");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);

  glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
   gluPerspective(60 ,1,.1,30.);



  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}


void affichage()
{
  int i,j;
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glTranslatef(0.,0.,-5.);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);

      glBegin(GL_POINTS);
      for (int i=0;i<n*m;i++){
          glColor3f(tore[i].r,tore[i].g,tore[i].b);
          glVertex3f(tore[i].x,tore[i].y,tore[i].z);
      }
      glEnd();

    for (i=0;i<n*m;i++)
    {
      glBegin(GL_POLYGON);
      for (j=0;j<4;j++){
          glColor3f(tore[ftore[i][j]].r,tore[ftore[i][j]].g,tore[ftore[i][j]].b);
          glVertex3f(tore[ftore[i][j]].x,tore[ftore[i][j]].y,tore[ftore[i][j]].z);
      }
      glEnd();
    }

  glFlush();

  //On echange les buffers
  glutSwapBuffers();
}


void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);
    }
}

void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0; /* le booleen presse passe a 0 (faux) */
}

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;
  }


