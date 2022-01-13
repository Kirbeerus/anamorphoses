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

//Variable pour gérer la position de la caméra
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


//Variable pour stocker la forme du cylindre
Point cylindre[cranCylindre*2];
int fcylindre[cranCylindre*2+1][4];

//Variable pour stocker le plan
Point plan[4];
int fplan[4];

//Variable pour stocker l'image non déformer
Point objet[nbPointImage];
int fobjet[2][4];

//Variable pour stocker l'image deformer
Point imageReflete[nbPointImage];
int fimageReflete[2][4];


//Fonction qui génère l'image non déformer
void genererObjet(){

    //On créer les points
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

    //On enregistre les faces de l'objet
    fobjet[0][0] = 0;fobjet[0][1] = 5;fobjet[0][2] = 4;fobjet[0][3] = 2;
    fobjet[1][0] = 1;fobjet[1][1] = 5;fobjet[1][2] = 4;fobjet[1][3] = 3;
}

//Fonction pour génerer le plan 2D
void genererPlan(){
    //On créer les 4 coin du Plan
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

    //On créer la faces pour faire le plan
    fplan[0] = 0;
    fplan[1] = 1;
    fplan[2] = 2;
    fplan[3] = 3;
}


//Fonction pour generer le cylindre
void genererCylindre(){
    int cpt = 0;
    //On calcule tout les point du cercle du bas
    for(int i = 0;i<cranCylindre;i++){
            Point tmp;
            tmp.x = rayonCylidnre*cos(i*(2*M_PI/cranCylindre));//Formule d'un cercle
            tmp.y = rayonCylidnre*sin(i*(2*M_PI/cranCylindre));
            tmp.z = 0;

            tmp.r = 1.0;
            tmp.g = 1.0;
            tmp.b = 1.0;

            cylindre[cpt] = tmp;
            cpt++;
    }
    //On calcule tout les points du cercle du haut
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

//Génération des faces du cylindre
void genereFaceCylindreQuadra(){
        //On relie les les points du cercle du haut et du bas pour créer le cylindre
        for(int i=0;i< cranCylindre*2+1;i++){
            fcylindre[i][0] = i;
            fcylindre[i][1] = (i+1)%(cranCylindre*2);
            fcylindre[i][2] = (i+1+cranCylindre)%(cranCylindre*2);
            fcylindre[i][3] = (i+cranCylindre)%(cranCylindre*2);
        }

}

//Fonction qui génère l'image deformer
void genererImageDeforme(){
    //On initialise toutes les variables nécéssaire au différents calcules

    Point pv; //Point correspondant à la caméra
    Point pP; //Point correspondant à un point de l'image
    Point pI;  //Interserction entre le cylindre et le vecteur pPpv
    Point pvprime;  //Symétrique de pv par rapport à la normale en N
    double p;   //Coefficient permmettant de calculer pI dans le formule (1-p)*pv+p*pP
    double pprime;  //Une des deux solutions du polynome du second degré
    double pn;  //Une des deux solutions du polynome du second degré
    double delta;
    double a;
    double b;
    double c;
    double k;
    double alpha;

    //On utilise une boucle pour parcourir tout les points de l'image normale
    for(int i=0;i<nbPointImage;i++){
        pv.x = posx;
        pv.y = posy;
        pv.z = posz;
        pP.x = objet[i].x;  //On récupère les coordonnées d'un point de l'image non déformer que l'on injecte dans pP
        pP.y = objet[i].y;
        pP.z = objet[i].z;
        Point tmp;  //On créer un point qui contiendra les coordonnées finale
        //On calcule les trois éléments du polynome du second degré.
        a = (pv.x-pP.x)*(pv.x-pP.x)+(pv.y+pP.y)*(pv.y+pP.y);
        b = pv.x*pP.x + pv.y*pP.y - pv.x*pv.x - pv.y*pv.y;
        c = pv.x*pv.x + pv.y*pv.y - rayonCylidnre*rayonCylidnre;
        //On calcule delta pour trouver les solutions
        delta = (2*b)*(2*b) - 4*a*c;
        //On calcule les solutions
        pprime = (-b*2+sqrt(delta))/(2*a);
        pn = (-b*2-sqrt(delta))/(2*a);
        //On garde la solution positive la plus petites
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
        //On calcule les coordonnées de pI le point d'instersection entre le cylindre et le vecteur pPpV
        pI.x = pv.x*(1-p)+p*pP.x;
        pI.y = pv.y*(1-p)+p*pP.y;
        pI.z = pv.z*(1-p)+p*pP.z;

        //On récupère k qui sera nécéssaire au calcul de v',
        k = (2/(rayonCylidnre*rayonCylidnre))*(pv.x*pI.x+pv.y*pI.y);
        //On calcule pvprime le point symétrique de pv par rapport à la normal au mirroir en I
        pvprime.x = k*pI.x-pv.x;
        pvprime.z = k*pI.y-pv.y;
        pvprime.y = pv.z*(1-2*p);
        //On calcule alpha
        alpha = (pv.z*(1-2*p)+pP.z*p)/(-pv.z*p+p*pP.z);

        //On calcule finalement le point de l'objet déformer
        tmp.x = pvprime.x-alpha*(pvprime.x-pI.x);
        tmp.y = pvprime.y-alpha*(pvprime.y-pI.y);
        tmp.z = 0.1;


        tmp.r = objet[i].r;tmp.g = objet[i].g;tmp.b = objet[i].b;

        printf("%d : %f \n",i,tmp.y);
        imageReflete[i] = tmp;
    }
    //On créer les faces de l'objet deformer
    //Il suffit juste de prendre les point de l'image deformer et les les lier entre  eux comme pour l'image non deformer
    fimageReflete[0][0] = 0;fimageReflete[0][1] = 5;fimageReflete[0][2] = 4;fimageReflete[0][3] = 2;
    fimageReflete[1][0] = 1;fimageReflete[1][1] = 5;fimageReflete[1][2] = 4;fimageReflete[1][3] = 3;
}

int main(int argc,char **argv)
{
  /* initialisation de glut et creation
     de la fenetre */
     //On génère tout les points
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
  glutIdleFunc(affichage);  //De cette façon on modifie l'affichage lordque l'on déplace la caméra
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutSpecialFunc(processSpecialKeys);  //Fonction de déplacement de la caméra

  glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
   gluPerspective(60 ,1,.1,30.);


  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}

//Fonction d'affichage des différents figure
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
    gluLookAt(posx,posy,posz,orix,oriy,oriz,upx,upy,upz);   //On place la caméra
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

//Touche pour deplacer la caméra
void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT :
			posx =posx-0.5; //On déplace la caméra à gauche
			break;
		case GLUT_KEY_RIGHT :   //On déplace la caméra à droite
			posx =posx+0.5;
			break;
		case GLUT_KEY_UP :  //On avance la caméra
			posy =posy-0.5;
			break;
		case GLUT_KEY_DOWN :    //On éloigne la caméra
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


