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

#define rayonCylidnre 5
#define tailleCylidnre 15
#define cranCylindre 50
#define tailleCylidnre 10
#define taillePlan 20.0
#define nbPointImage 8

void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void processSpecialKeys(int key, int xx, int yy);
char presse;
int anglex,angley,x,y,xold,yold;

double posx = 12.0;
double posy = 12.0;
double posz = 8.0;
double orix = 0.0;
double oriy = 5.0;
double oriz = -2.0;
double upx = 0.0;
double upy = 1.0;
double upz = 0.0;

/*class Point*/
class Point{
    public :
	//coordonnées x, y et z du point
	float x;
	float y;
	float z;
	// couleur r, v et b du point
	float r;
	float g;
	float b;
};



Point cylindre[cranCylindre*2];
int fcylindre[cranCylindre*2+1][4];

Point plan[4];
int fplan[4];

Point objet[nbPointImage];
int fobjet[6][4];

Point imageReflete[nbPointImage];

void genererObjet(){
    Point tmp;
    tmp.x = -3.0;tmp.y = 1.0;tmp.z = -3.0;
    tmp.r = 0.4;tmp.g = 0.2;tmp.b = 0.0;
    objet[0] = tmp;
    Point tmp2;
    tmp2.x = 3.0;tmp2.y = 1.0;tmp2.z = -3.0;
    tmp2.r = 0.4;tmp2.g = 0.2;tmp2.b = 0.0;
    objet[1] = tmp2;
    Point tmp3;
    tmp3.x = -3.0;tmp3.y = 1.0;tmp3.z = 3.0;
    tmp3.r = 0.4;tmp3.g = 0.2;tmp3.b = 0.0;
    objet[2] = tmp3;
    Point tmp4;
    tmp4.x = 3.0;tmp4.y = 1.0;tmp4.z = 3.0;
    tmp4.r = 0.4;tmp4.g = 0.2;tmp4.b = 0.0;
    objet[3] = tmp4;
    Point tmp5;
    tmp5.x = -3.0;tmp5.y = 7.0;tmp5.z = -3.0;
    tmp5.r = 0.4;tmp5.g = 0.2;tmp5.b = 0.0;
    objet[4] = tmp5;
    Point tmp6;
    tmp6.x = 3.0;tmp6.y = 7.0;tmp6.z = -3.0;
    tmp6.r = 0.4;tmp6.g = 0.2;tmp6.b = 0.0;
    objet[5] = tmp6;
    Point tmp7;
    tmp7.x = -3.0;tmp7.y = 7.0;tmp7.z = 3.0;
    tmp7.r = 0.4;tmp7.g = 0.2;tmp7.b = 0.0;
    objet[6] = tmp7;
    Point tmp8;
    tmp8.x = 3.0;tmp8.y = 7.0;tmp8.z = 3.0;
    tmp8.r = 0.4;tmp8.g = 0.2;tmp8.b = 0.0;
    objet[7] = tmp8;

    fobjet[0][0] = 0;fobjet[0][1] = 1;fobjet[0][2] = 3;fobjet[0][3] = 2;
    fobjet[1][0] = 1;fobjet[1][1] = 3;fobjet[1][2] = 7;fobjet[1][3] = 5;
    fobjet[2][0] = 0;fobjet[2][1] = 2;fobjet[2][2] = 6;fobjet[2][3] = 4;
    fobjet[3][0] = 0;fobjet[3][1] = 1;fobjet[3][2] = 5;fobjet[3][3] = 4;
    fobjet[4][0] = 2;fobjet[4][1] = 3;fobjet[4][2] = 7;fobjet[4][3] = 6;
    fobjet[5][0] = 4;fobjet[5][1] = 5;fobjet[5][2] = 7;fobjet[5][3] = 6;
}

void genererPlan(){
    Point tmp;
    tmp.x = -taillePlan;
    tmp.y = 0.0;
    tmp.z = -taillePlan;

    tmp.r = 1.0;
    tmp.g = 1.0;
    tmp.b = 1.0;

    plan[0] = tmp;

    Point tmp2;
    tmp2.x = -taillePlan;
    tmp2.y = 0.0;
    tmp2.z = taillePlan;
    tmp2.r = 1.0;
    tmp2.g = 1.0;
    tmp2.b = 1.0;
    plan[1] = tmp2;

    Point tmp3;
    tmp3.x = taillePlan;
    tmp3.y = 0.0;
    tmp3.z = taillePlan;
    tmp3.r = 1.0;
    tmp3.g = 1.0;
    tmp3.b = 1.0;
    plan[2] = tmp3;

    Point tmp4;
    tmp4.x = taillePlan;
    tmp4.y = 0.0;
    tmp4.z = -taillePlan;
    tmp4.r = 1.0;
    tmp4.g = 1.0;
    tmp4.b = 1.0;
    plan[3] = tmp4;

    fplan[0] = 0;
    fplan[1] = 1;
    fplan[2] = 2;
    fplan[3] = 3;
}


void genererCylindre(){
    int cpt = 0;
    for(int i = 0;i<cranCylindre;i++){
            Point tmp;
            tmp.x = rayonCylidnre*cos(i*(2*M_PI/cranCylindre));
            tmp.y = 0;
            tmp.z = rayonCylidnre*sin(i*(2*M_PI/cranCylindre));

            tmp.r = 1.0;
            tmp.g = 0.0;
            tmp.b = 0.0;

            cylindre[cpt] = tmp;
            cpt++;
    }
    for(int i = 0;i<cranCylindre;i++){
            Point tmp;
            tmp.x = rayonCylidnre*cos(i*(2*M_PI/cranCylindre));
            tmp.y = tailleCylidnre;
            tmp.z = rayonCylidnre*sin(i*(2*M_PI/cranCylindre));

            tmp.r = 0.0;
            tmp.g = 0.0;
            tmp.b = 1.0;

            cylindre[cpt] = tmp;
            cpt++;
    }
};

void genereFaceCylindreQuadra(){
        for(int i=0;i< cranCylindre*2+1;i++){
            fcylindre[i][0] = i;
            fcylindre[i][1] = (i+1)%(cranCylindre*2);
            fcylindre[i][2] = (i+1+cranCylindre)%(cranCylindre*2);
            fcylindre[i][3] = (i+cranCylindre)%(cranCylindre*2);
        }

}

void genererImageDeforme(){
    Point pvprime;
    Point pI;
    Point pv;
    Point pP;
    double p;
    double pprime;
    double pn;
    double delta;
    double a;
    double b;
    double c;
    double k;
    double alpha;

    pv.x = posx;
    pv.y = posy;
    pv.z = posz;

    for(int i=0;i<8;i++){
        pP.x = objet[i].x;
        pP.y = objet[i].y;
        pP.z = objet[i].z;
        Point tmp;
        a = (pv.x-pP.x)*(pv.x-pP.x)+(pv.z+pP.z)*(pv.z+pP.z);
        b = pv.x*pP.x + pv.z*pP.z - pv.x*pv.x - pv.z*pv.z;
        c = pv.x*pv.x + pv.z*pv.z - rayonCylidnre*rayonCylidnre;

        delta = b*b - a*c;

        pprime = (-b+sqrt(delta))/(a);
        pn = (-b-sqrt(delta))/(a);

        if(pn<0){
            p = pprime;
        }else if(pprime<0){
            p = pn;
        }else if(pprime<pn){
            p = pprime;
        } else{
            p = pn;
        }

        printf("%f \n",delta);

        pI.x = pv.x*(1-p)+p*pP.x;
        pI.y = pv.y*(1-p)+p*pP.y;
        pI.z = pv.z*(1-p)+p*pP.z;


        k = (2/(rayonCylidnre*rayonCylidnre))*(pv.x*pI.x+pv.z*pI.z);

        pvprime.x = k*pI.x-pv.x;
        pvprime.z = k*pI.z-pv.z;
        pvprime.y = pv.y*(1-2*p);

        alpha = (pv.y*(1-2*p)+pP.y*p)/(-pv.y*p+p*pP.y);


        tmp.x = pvprime.x-alpha*(pvprime.x-pI.x);
        tmp.y = 0.0;
        tmp.z = pvprime.z-alpha*(pvprime.z-pI.z);

        tmp.r = 0.3;tmp.g = 0.0;tmp.b = 0.4;

        printf("%d : %f \n",i,tmp.x);
        imageReflete[i] = tmp;
    }


}

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
  genererCylindre();
  genereFaceCylindreQuadra();
  genererObjet();
  genererPlan();
  genererImageDeforme();

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
  glutIdleFunc(affichage);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutSpecialFunc(processSpecialKeys);

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
  gluLookAt(posx,posy,posz,orix,oriy,oriz,upx,upy,upz);
  glTranslatef(0.,0.,-5.);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);

    //Génération cylindre
    glBegin(GL_POINTS);
      for (int i=0;i<cranCylindre*2;i++){
          glColor3f(cylindre[i].r,cylindre[i].g,cylindre[i].b);
          glVertex3f(cylindre[i].x,cylindre[i].y,cylindre[i].z);
      }
      glEnd();

    for (i=0;i<cranCylindre*2;i++)
    {
      glBegin(GL_POLYGON);
      for (j=0;j<4;j++){
          glColor3f(cylindre[fcylindre[i][j]].r,cylindre[fcylindre[i][j]].g,cylindre[fcylindre[i][j]].b);
          glVertex3f(cylindre[fcylindre[i][j]].x,cylindre[fcylindre[i][j]].y,cylindre[fcylindre[i][j]].z);
      }
      glEnd();
    }

    //Génération Plan
    glBegin(GL_POINTS);
      for (int i=0;i<4;i++){
          glColor3f(plan[i].r,plan[i].g,plan[i].b);
          glVertex3f(plan[i].x,plan[i].y,plan[i].z);
      }
      glEnd();

      glBegin(GL_POLYGON);
      for (j=0;j<4;j++){
          glColor3f(plan[fplan[j]].r,plan[fplan[j]].g,plan[fplan[j]].b);
          glVertex3f(plan[fplan[j]].x,plan[fplan[j]].y,plan[fplan[j]].z);
      }
      glEnd();

      //Génération Objet
      glBegin(GL_POINTS);
      for (int i=0;i<nbPointImage;i++){
          glColor3f(objet[i].r,objet[i].g,objet[i].b);
          glVertex3f(objet[i].x,objet[i].y,objet[i].z);
      }
      glEnd();

    for(int i = 0;i<6;i++){
      glBegin(GL_POLYGON);
        for (j=0;j<4;j++){
          glColor3f(objet[fobjet[i][j]].r,objet[fobjet[i][j]].g,objet[fobjet[i][j]].b);
          glVertex3f(objet[fobjet[i][j]].x,objet[fobjet[i][j]].y,objet[fobjet[i][j]].z);
        }
      glEnd();
      }

      //Génération Objet
      glBegin(GL_POINTS);
      for (int i=0;i<nbPointImage;i++){
          glColor3f(imageReflete[i].r,imageReflete[i].g,imageReflete[i].b);
          glVertex3f(imageReflete[i].x,imageReflete[i].y,imageReflete[i].z);
      }
      glEnd();

  glFlush();
    gluLookAt(posx,posy,posz,orix,oriy,oriz,upx,upy,upz);
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

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT :
			posx =posx-0.5;
			break;
		case GLUT_KEY_RIGHT :
			posx =posx+0.5;
			break;
		case GLUT_KEY_UP :
			posz =posz-0.5;
			break;
		case GLUT_KEY_DOWN :
			posz =posz+0.5;
			break;
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


