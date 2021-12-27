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

#define rayonCylidnre 4.0
#define tailleCylidnre 15.0
#define cranCylindre 50
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

double posx = 0.0;
double posy = -20.0;
double posz = 12.0;
double orix = 0.0;
double oriy = 0.0;
double oriz = 1.0;
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
int fobjet[2][4];

Point imageReflete[nbPointImage];
//int fimageReflete[4];
int fimageReflete[2][4];

void genererObjet(){

    Point tmp;
    tmp.x = -3.0;tmp.y = 0.0;tmp.z = 3.0;
    tmp.r = 0.4;tmp.g = 0.2;tmp.b = 0.0;
    objet[0] = tmp;
    Point tmp2;
    tmp2.x = 3.0;tmp2.y = 0.0;tmp2.z = 3.0;
    tmp2.r = 0.4;tmp2.g = 0.2;tmp2.b = 0.0;
    objet[1] = tmp2;
    Point tmp3;
    tmp3.x = -3.0;tmp3.y = 0.0;tmp3.z = 9.0;
    tmp3.r = 0.4;tmp3.g = 0.2;tmp3.b = 0.0;
    objet[2] = tmp3;
    Point tmp4;
    tmp4.x = 3.0;tmp4.y = 0.0;tmp4.z = 9.0;
    tmp4.r = 0.4;tmp4.g = 0.2;tmp4.b = 0.0;
    objet[3] = tmp4;
    tmp4.x = 0.0;tmp4.y = 0.0;tmp4.z = 11.0;
    tmp4.r = 0.4;tmp4.g = 0.2;tmp4.b = 0.0;
    objet[4] = tmp4;
    tmp4.x = 0.0;tmp4.y = 0.0;tmp4.z = 3.0;
    tmp4.r = 0.4;tmp4.g = 0.2;tmp4.b = 0.0;
    objet[5] = tmp4;

    fobjet[0][0] = 0;fobjet[0][1] = 5;fobjet[0][2] = 4;fobjet[0][3] = 2;
    fobjet[1][0] = 1;fobjet[1][1] = 5;fobjet[1][2] = 4;fobjet[1][3] = 3;
}

void genererPlan(){
    Point tmp;
    tmp.x = -taillePlan;
    tmp.y = -taillePlan;
    tmp.z = 0.0;

    tmp.r = 1.0;
    tmp.g = 1.0;
    tmp.b = 1.0;

    plan[0] = tmp;

    Point tmp2;
    tmp2.x = -taillePlan;
    tmp2.y = taillePlan;
    tmp2.z = 0.0;
    tmp2.r = 1.0;
    tmp2.g = 1.0;
    tmp2.b = 1.0;
    plan[1] = tmp2;

    Point tmp3;
    tmp3.x = taillePlan;
    tmp3.y = taillePlan;
    tmp3.z = 0.0;
    tmp3.r = 1.0;
    tmp3.g = 1.0;
    tmp3.b = 1.0;
    plan[2] = tmp3;

    Point tmp4;
    tmp4.x = taillePlan;
    tmp4.y = -taillePlan;
    tmp4.z = 0.0;
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
            tmp.y = rayonCylidnre*sin(i*(2*M_PI/cranCylindre));
            tmp.z = 0;

            tmp.r = 1.0;
            tmp.g = 1.0;
            tmp.b = 1.0;

            cylindre[cpt] = tmp;
            cpt++;
    }
    for(int i = 0;i<cranCylindre;i++){
            Point tmp;
            tmp.x = rayonCylidnre*cos(i*(2*M_PI/cranCylindre));
            tmp.y = rayonCylidnre*sin(i*(2*M_PI/cranCylindre));
            tmp.z = tailleCylidnre;

            tmp.r = 1.0;
            tmp.g = 1.0;
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

    //for(int i=0;i<8;i++){
    for(int i=0;i<6;i++){
        pv.x = posx;
        pv.y = posy;
        pv.z = posz;
        pP.x = objet[i].x;
        pP.y = objet[i].y;
        pP.z = objet[i].z;
        Point tmp;
        a = (pv.x-pP.x)*(pv.x-pP.x)+(pv.y+pP.y)*(pv.y+pP.y);
        b = pv.x*pP.x + pv.y*pP.y - pv.x*pv.x - pv.y*pv.y;
        c = pv.x*pv.x + pv.y*pv.y - rayonCylidnre*rayonCylidnre;

        delta = (2*b)*(2*b) - 4*a*c;

        pprime = (-b*2+sqrt(delta))/(2*a);
        pn = (-b*2-sqrt(delta))/(2*a);

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


        k = (2/(rayonCylidnre*rayonCylidnre))*(pv.x*pI.x+pv.y*pI.y);

        pvprime.x = k*pI.x-pv.x;
        pvprime.z = k*pI.y-pv.y;
        pvprime.y = pv.z*(1-2*p);

        alpha = (pv.z*(1-2*p)+pP.z*p)/(-pv.z*p+p*pP.z);


        tmp.x = pvprime.x-alpha*(pvprime.x-pI.x);
        tmp.y = pvprime.y-alpha*(pvprime.y-pI.y);
        tmp.z = 0.1;


        tmp.r = objet[i].r;tmp.g = objet[i].g;tmp.b = objet[i].b;

        printf("%d : %f \n",i,tmp.y);
        imageReflete[i] = tmp;
    }
    fimageReflete[0][0] = 0;fimageReflete[0][1] = 5;fimageReflete[0][2] = 4;fimageReflete[0][3] = 2;
    fimageReflete[1][0] = 1;fimageReflete[1][1] = 5;fimageReflete[1][2] = 4;fimageReflete[1][3] = 3;

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
  glutCreateWindow("Anamorphoses");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(4.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    //Génération cylindre
    glBegin(GL_POINTS);
      for (int i=0;i<cranCylindre*2;i++){
          glColor4f(cylindre[i].r,cylindre[i].g,cylindre[i].b,0.5);
          glVertex3f(cylindre[i].x,cylindre[i].y,cylindre[i].z);
      }
      glEnd();

    for (i=0;i<cranCylindre*2;i++)
    {
      glBegin(GL_POLYGON);
      for (j=0;j<4;j++){
          glColor4f(cylindre[fcylindre[i][j]].r,cylindre[fcylindre[i][j]].g,cylindre[fcylindre[i][j]].b,0.5);
          glVertex3f(cylindre[fcylindre[i][j]].x,cylindre[fcylindre[i][j]].y,cylindre[fcylindre[i][j]].z);
      }
      glEnd();
    }

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
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

    for(int i = 0;i<2;i++){
      glBegin(GL_POLYGON);
        for (j=0;j<4;j++){
          glColor3f(objet[fobjet[i][j]].r,objet[fobjet[i][j]].g,objet[fobjet[i][j]].b);
          glVertex3f(objet[fobjet[i][j]].x,objet[fobjet[i][j]].y,objet[fobjet[i][j]].z);
        }
      glEnd();
      }

      //Génération image reflete
      glBegin(GL_POINTS);
      for (int i=0;i<nbPointImage;i++){
          glColor3f(imageReflete[i].r,imageReflete[i].g,imageReflete[i].b);
          glVertex3f(imageReflete[i].x,imageReflete[i].y,imageReflete[i].z);
      }
      glEnd();

      for(int i = 0;i<2;i++){
      glBegin(GL_POLYGON);
        for (j=0;j<4;j++){
          glColor3f(imageReflete[fimageReflete[i][j]].r,imageReflete[fimageReflete[i][j]].g,imageReflete[fimageReflete[i][j]].b);
          glVertex3f(imageReflete[fimageReflete[i][j]].x,imageReflete[fimageReflete[i][j]].y,imageReflete[fimageReflete[i][j]].z);
        }
      glEnd();
      }

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
			posy =posy-0.5;
			break;
		case GLUT_KEY_DOWN :
			posy =posy+0.5;
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


