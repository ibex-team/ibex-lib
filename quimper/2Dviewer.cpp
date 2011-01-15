/**
 * --------- A very basic 2D viewer for Quimper --------------
 *
 * Each line of the input file must correspond to a 2D box [x]x[y].
 * A line must be a sequence of 7 float numbers, corresponding in order to:
 * inf([x]) sup([x]) inf([y]) sup([y]) R1 G1 B1 R2 G2 B2
 * where (R1 G1 B1) is the color code of the interior of the box
 *       (R2 G2 B2) is the color code of the border of the box
 *
 * Chabert Jan 2008
 */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define WIN_WIDTH  500
#define WIN_HEIGHT 500

/*-------------------- BOXES ---------------------*/
struct box2D {
  double x1, x2, y1, y2;
  int ctc;           // subpaving (contractor) number
};

std::vector<struct box2D> data;
/*------------------------------------------------*/


/*----------------- CONTRACTORS ------------------*/
int N=0; // number of contractors
int B=0; // number of boxes

struct color {
  double red1, green1, blue1; // box interior
  double red2, green2, blue2; // box border
};

int current_ctc=0; // 0 means "all contractors"
std::vector<struct color> ctc; // array of contractor colors.

/*------------------------------------------------*/


/* ---- FRAME -----*/
double xmin=1e10;
double xmax=-1e10;
double ymin=1e10;
double ymax=-1e10;


/* --- SCALE -------*/
double midx,midy,hx,hy;
/*------------------*/


void drawbox2D(const box2D& box) {
  /* Dessin du polygone */

  glLoadIdentity();
//--- without using "rescale": ----
//   glScalef(hx,hy,1);
//   glTranslatef(-midx,-midy,0.0);
// -------------------------------
  color& color = ctc[box.ctc];
  
  glBegin(GL_POLYGON);
  glColor3f(color.red1,color.green1,color.blue1);
  glVertex2f(box.x1,box.y1);
  glVertex2f(box.x1,box.y2);
  glVertex2f(box.x2,box.y2);
  glVertex2f(box.x2,box.y1);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glColor3f(color.red2,color.green2,color.blue2);
  glVertex2f(box.x1,box.y1);
  glVertex2f(box.x1,box.y2);
  glVertex2f(box.x2,box.y2);
  glVertex2f(box.x2,box.y1);
  glEnd();
}

void error() {
  fprintf(stderr,"bad file format\n");
  exit(-1);
}

void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else 
    glViewport((x-y)/2,0,y,y);
}

void update_frame(struct box2D& box) {
  if (box.x1 < xmin) xmin = box.x1;
  if (box.x2 > xmax) xmax = box.x2;
  if (box.y1 < ymin) ymin = box.y1;
  if (box.y2 > ymax) ymax = box.y2;   
}

void read_file(char* filename) {
  
  FILE *f;
  int code;

  if ((f = fopen(filename, "r")) == NULL) {
    fprintf(stderr,"unknown file %s\n", filename);
    exit(-1);
  }
  int i=0;
  
  while(1) {
    int n,b;
    code=fscanf(f,"%d",&n);
    if (code==EOF) break;
    if (fscanf(f,"%d",&b)==EOF) error();
    
    for (int i=0; i<n; i++) {
      color c;
      if (fscanf(f,"%lf",&c.red1)==EOF) error();
      if (fscanf(f,"%lf",&c.green1)==EOF) error();
      if (fscanf(f,"%lf",&c.blue1)==EOF) error();
      if (fscanf(f,"%lf",&c.red2)==EOF) error();
      if (fscanf(f,"%lf",&c.green2)==EOF) error();
      if (fscanf(f,"%lf",&c.blue2)==EOF) error();
      ctc.push_back(c);
    }
    
    box2D box;
    for (int i=0; i<b; i++) {
      if (fscanf(f,"%lf",&box.x1)==EOF) error();
      if (fscanf(f,"%lf",&box.x2)==EOF) error();
      if (fscanf(f,"%lf",&box.y1)==EOF) error();
      if (fscanf(f,"%lf",&box.y2)==EOF) error();
      if (fscanf(f,"%d",&box.ctc)==EOF) error();
      box.ctc+=N;
      data.push_back(box);
      update_frame(box);
    }
    N+=n;
  }

  fclose(f);
}

void rescale() {
  for (std::vector<box2D>::iterator it=data.begin(); it!=data.end(); it++) {
    it->x1 -= midx;
    it->x2 -= midx;
    it->y1 -= midy;
    it->y2 -= midy;

    it->x1 *= hx;
    it->x2 *= hx;
    it->y1 *= hy;
    it->y2 *= hy;
  }
}

void display() {
  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT);

  for (std::vector<box2D>::iterator it=data.begin(); it!=data.end(); it++) {
    if (current_ctc==0 || it->ctc==current_ctc-1)
      drawbox2D(*it);
  }
  glFlush();
  glutSwapBuffers(); 
}

void keyboard(unsigned char key, int x, int y)
{
  if (key>='0' && key<='9') {
    current_ctc=key-'0';
    if (current_ctc > N) current_ctc=0;
    printf("current ctc=%d\n",current_ctc);
    glutPostRedisplay();
  }

  else {
    switch (key)
      {
    case 'w': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 'W': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'q' : 
      exit(0);
      }
  }
}

int main(int argc,char **argv) {

  if (argc==1) {
    fprintf(stderr, "usage: viewer filename\n");
    exit(-1);
  }

  read_file(argv[1]);

  midx = (xmax+xmin)/2.0;
  midy = (ymax+ymin)/2.0;
  hx = 2.0/(xmax-xmin);
  hy = 2.0/(ymax-ymin);

  printf("Read %d boxes.\n", data.size());

  rescale();

  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // GLUT_DOUBLE-> use swap buffers
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("2D viewer");
  printf("frame=%f %f %f %f\n", xmin, ymin, xmax-xmin, ymax-ymin);
//  glLoadIdentity();
//  gluLookAt(0,0, 10, 0,0,0,0,0,1); //xmax-xmin, ymax-ymin, 0, 1, 0, -1);

  /* Initialisation d'OpenGL */
  //  glClearColor(0.0,0.0,0.0,0.0);
  glClearColor(1.0,1.0,1.0,1.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  
  /* Entree dans la boucle principale glut */
  glutMainLoop();


  return 0;
}
