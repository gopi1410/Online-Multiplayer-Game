#include<GL/gl.h>
#include <GL/glut.h>
#include<stdio.h>
#include<math.h>

void change_r(int);
static GLfloat tilt=0.0,phi=0.0,theta=0.0,r=0.01,alpha=0.0;
static GLfloat car[2][2][2][3];
GLboolean pressed[27],first=1;
int counter=0;
float iterator,iterator2;
const GLfloat PI=3.141592;
const GLfloat WHITE[] = {1.0,1.0,1.0,1.0}, BLACK[] = {0.0,0.0,0.0,1.0}, RED[] = {1.0,0.0,0.0,1.0};

GLfloat carAmbDiff[] = {1.0,0.0,1.0,1.0};
GLfloat carSpecular[] = {0.0,1.0,0.0,1.0};
GLfloat groundAmbDiff[] = {0.5,0.5,0.5,1.0};
GLfloat wallAmbDiff[] = {1.0,1.0,0.0,1.0};
GLfloat carShine = 5.0;
GLfloat groundShine = 5.0;
GLfloat light1Direction[] = {0.0,-0.4,1.0};
GLfloat light1Pos[] = {0.0,-5.0,-6.5,1.0},headlight_radius=0.1,angle;
GLfloat wallColor1[] = {0.0,0.2,0.2,1.0};
GLfloat wallColor2[] = {0.0,0.1,0.1,1.0};
GLint lightAngle = 0;

void createLights()
{       GLfloat light0Pos[] = {0.0,1.0,1.0,0.0};
        GLfloat light0Diffuse[] = {0.3,0.3,0.3,1.0};
        GLfloat light0Specular[] = {1.0,1.0,0.0,1.0};
        GLfloat ambLight[] = {0.0,0.0,0.0,1.0};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambLight);
        glLightfv(GL_LIGHT0,GL_POSITION,light0Pos);
        glLightfv(GL_LIGHT0,GL_SPECULAR,light0Specular);
        //glLightfv(GL_LIGHT0,GL_POSITION,light0Pos);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,light0Diffuse);
        glLightfv(GL_LIGHT1,GL_SPECULAR,light0Specular);
        glLightfv(GL_LIGHT1,GL_AMBIENT,light0Diffuse);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
        //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
        glLightfv(GL_LIGHT1,GL_POSITION,light1Pos);
        glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1Direction);
        glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,35.0);
        glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,50.0);
        glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,0.3);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_AUTO_NORMAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);
   glDisable(GL_CULL_FACE);
   glShadeModel (GL_SMOOTH);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 1000.0);
   glMatrixMode (GL_MODELVIEW);
   int i,j,k;
   for(i=0;i<2;++i)
     for(j=0;j<2;++j)
           for(k=0;k<2;++k)
              {
               car[i][j][k][0] = i-0.5;
               car[i][j][k][1] = j-0.5;
               car[i][j][k][2] = k-0.5;
              }
    car[1][0][1][2]+=0.5;
    car[0][0][1][2]+=0.5;
   for(counter=1;counter<=26;counter+=1)
        pressed[counter]=0;
    createLights();
}
void transform_world()
{
    glTranslatef(0.0,-0.0,-5.0);
    glRotatef(-phi,0.0,1.0,0.0);
    glTranslatef(-r*sin(alpha*PI/180),0.0,-r*cos(alpha*PI/180));
}
void drawCar()
{   glPushMatrix();

    glTranslatef(0.0,-5.0,-5.0);
        //transform_world();
glRotatef(-180,0.0,1.0,0.0);
      light1Pos[0] = light1Pos[2] = 0.0;
      light1Pos[1] = 0.75;
      light1Pos[3] = 1.0;
      glLightfv(GL_LIGHT1,GL_POSITION,light1Pos);
      glRotatef(lightAngle,1.0,0.0,0.0);
      glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1Direction);
      glRotatef(-lightAngle,1.0,0.0,0.0);

    glBegin(GL_QUADS);
       glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,carAmbDiff);
       glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,carShine);
       glNormal3f(0,0,-1);                                                  //TO BE CORRECTED
       glVertex3fv(car[0][0][0]);
       glVertex3fv(car[0][1][0]);
       glVertex3fv(car[1][1][0]);
       glVertex3fv(car[1][0][0]);

       glNormal3f(0,0,1);
       glVertex3fv(car[1][1][1]);
       glVertex3fv(car[1][0][1]);
       glVertex3fv(car[0][0][1]);
       glVertex3fv(car[0][1][1]);

       glNormal3f(-1,0,0);
       glVertex3fv(car[0][0][0]);
       glVertex3fv(car[0][0][1]);
       glVertex3fv(car[0][1][1]);
       glVertex3fv(car[0][1][0]);


       glNormal3f(1,0,0);
       glVertex3fv(car[1][0][0]);
       glVertex3fv(car[1][0][1]);
       glVertex3fv(car[1][1][1]);
       glVertex3fv(car[1][1][0]);


       glNormal3f(0,-1,0);
       glVertex3fv(car[1][0][0]);
       glVertex3fv(car[1][0][1]);
       glVertex3fv(car[0][0][1]);
       glVertex3fv(car[0][0][0]);

       glNormal3f(0,1,0);
       glVertex3fv(car[0][1][0]);
       glVertex3fv(car[0][1][1]);
       glVertex3fv(car[1][1][1]);
       glVertex3fv(car[1][1][0]);

       glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,RED);
       glVertex3f(-0.45,-0.25,-0.51);
       glVertex3f(-0.45,-0.30,-0.51);
       glVertex3f(-0.35,-0.30,-0.51);
       glVertex3f(-0.35,-0.25,-0.51);

       glVertex3f(0.45,-0.25,-0.51);
       glVertex3f(0.45,-0.30,-0.51);
       glVertex3f(0.35,-0.30,-0.51);
       glVertex3f(0.35,-0.25,-0.51);
       glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,BLACK);


   glEnd();
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,WHITE);
    glVertex3f(0,0,car[0][0][1][2]-0.25);
    glTranslatef(0.0,0.0,0.76);
    glRotatef(atan(0.5)*180/PI,-1.0,0,0);
   glBegin(GL_TRIANGLE_FAN);                                        //create headlight (pointless)
 for(angle=0;angle<360;angle+=10)
 glVertex3f(sin(angle) * headlight_radius,cos(angle) * headlight_radius,0.0);
 glEnd();
 GLfloat translucent[] = {1.0,1.0,1.0,0.005};
 glMaterialfv(GL_FRONT,GL_EMISSION,translucent);
 glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,translucent);
 glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0.0);
 glRotatef(230.0+lightAngle,1.0,0.0,0.0);
 glTranslatef(0.0,0.0,-1.7);glDepthMask(GL_FALSE);
 for(iterator=0;iterator<10;iterator+=1)
 {  glutSolidCone(0.5,0.1*iterator+0.5,10,10);
 }

 glDepthMask(GL_TRUE);
 glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,BLACK);
    glPopMatrix();

}

void drawGround()
{
    glPushMatrix();
    transform_world();
    glTranslatef(0.0,-5.0,0.0);
    glBegin(GL_QUADS);
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,groundAmbDiff);
        glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,groundShine);
        glNormal3f(0,1,0);
        for(iterator2=1000;iterator2>-1000;iterator2-=0.25)
        {for(iterator=-5;iterator<5;iterator+=0.25)
        {   glVertex3f(iterator,-0.5,iterator2);
            glVertex3f(iterator,-0.5,iterator2-0.25);
            glVertex3f(iterator+0.25,-0.5,iterator2-0.25);
            glVertex3f(iterator+0.25,-0.5,iterator2);
        }
        }


        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,wallAmbDiff);
        glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,wallColor1);
        glNormal3f(1,0,0);
        for(iterator2=0;iterator2<1000;iterator2+=0.5)
        {   for(iterator=-0.5;iterator<3;iterator+=0.5)
            {
                if(((int)iterator2)%2==0)
                glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,wallColor2);

                glVertex3f(-5.0,iterator,-iterator2);
                glVertex3f(-5.0,iterator,-iterator2-0.5);
                glVertex3f(-5.0,iterator+0.5,-iterator2-0.5);
                glVertex3f(-5.0,iterator+0.5,-iterator2);

                glVertex3f(-5.5,iterator+1,-iterator2);
                glVertex3f(-5.5,iterator+1,-iterator2-0.5);
                glVertex3f(-5.5,iterator+1.5,-iterator2-0.5);
                glVertex3f(-5.5,iterator+1.5,-iterator2);

                glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,wallColor1);

            }
        }

        glNormal3f(-1,0,0);
           for(iterator2=0;iterator2<1000;iterator2+=0.5)
        {   for(iterator=-0.5;iterator<3;iterator+=0.5)
            {

                 if(((int)iterator2)%2==0)
                glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,wallColor2);

                glVertex3f(5.0,iterator,-iterator2);
                glVertex3f(5.0,iterator,-iterator2-0.5);
                glVertex3f(5.0,iterator+0.5,-iterator2-0.5);
                glVertex3f(5.0,iterator+0.5,-iterator2);

                glVertex3f(5.5,iterator+1,-iterator2);
                glVertex3f(5.5,iterator+1,-iterator2-0.5);
                glVertex3f(5.5,iterator+1.5,-iterator2-0.5);
                glVertex3f(5.5,iterator+1.5,-iterator2);

                 if(((int)iterator2)%2==0)
                glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,wallColor1);
            }
        }

    glEnd();
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,groundAmbDiff);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,BLACK);
    glTranslatef(0.0,5.0,-5.0);
    glutSolidSphere(0.0,40,40);
    glPopMatrix();

}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();             /* clear the matrix */
           /* viewing transformation  */
   gluLookAt (0.0, 0.0, 0.0, 0.0, theta-2.2, -5.0, 0.0, 1.0, 0.0);
   drawGround();
   drawCar();
   glFlush ();
   glutSwapBuffers();
}

void update()

{
    if(pressed['A'-65])
        tilt = tilt + 1;
    else if(pressed['S'-65])
        tilt = tilt - 1;
    else if(pressed['J'-65])
        phi = phi + 3.0;
    else if(pressed['K'-65])
        phi = phi - 3.0;
    else if(pressed['O'-65])
        theta = theta + 0.1;
    else if(pressed['L'-65])
        theta = theta - 0.1;
    else if(pressed['H'-65])
        change_r(+1);
    else if(pressed['Y'-65])
        change_r(-1);
    display();

}
void change_r(int flag_mov_dir)
{
    GLfloat rn;
    rn=r;
    int t,d_alpha_sign;
    float phi_modulo_360,d_alpha;
    if(phi>0)
    {
        t=(int)(phi/360);
        phi_modulo_360=phi-t*360;
    }
    else
    {
        t = -phi/360 + 1;
        phi_modulo_360=phi+t*360;
    }
    if(alpha>360.0)
    {
        alpha = alpha - 360;
    }
    else if(alpha<0.0)
    {
        alpha = 360.0 + alpha;
    }

    if(flag_mov_dir==1)                                                             // if car is being moved forward
    {
        r=sqrt((float)(rn*rn+0.01+2*rn*0.1*cos((float)((phi_modulo_360-alpha)*PI/180))));
        d_alpha=180*acos((float)((rn*rn+r*r-0.01)/(2*r*rn)))/PI;
        if(alpha<=180)
        {
            if(phi_modulo_360<=(alpha+180)&&phi_modulo_360>=alpha)
                {
                    d_alpha_sign=1;
                }
                else
                {
                    d_alpha_sign=-1;
                }
        }
        else
        {
                if(phi_modulo_360>=(alpha-180)&&phi_modulo_360<=alpha)
                {
                    d_alpha_sign=-1;
                }
                else
                {
                    d_alpha_sign=1;
                }
        }

    }
    if(flag_mov_dir==-1)                                                           //if car is being moved backward
    {
        r=sqrt((float)(rn*rn+0.01-2*rn*0.1*cos((float)((phi_modulo_360-alpha)*PI/180))));
        d_alpha=180*acos((float)((rn*rn+r*r-0.01)/(2*r*rn)))/PI;
        if(alpha<=180)
            {
                if(phi_modulo_360<=(alpha+180.0)&&phi_modulo_360>=alpha)
                    {
                        d_alpha_sign=-1;
                    }
                else
                    {
                        d_alpha_sign=1;
                    }
            }
        else
            {
                if(phi_modulo_360>(alpha-180.0)&&phi_modulo_360<alpha)
                {
                    d_alpha_sign=+1;
                    printf("C ");
                }
                else
                {
                    d_alpha_sign=-1;
                }
        }
    }
    alpha+=d_alpha_sign*d_alpha;
     if(fabs(r*sin(alpha*PI/180))>4.0)          //collision detection
           {r=rn;alpha-=d_alpha_sign*d_alpha;}
    if(alpha<0)
        alpha = 360 + alpha;
    else if(alpha>360)
        alpha = alpha - 360;
    if(r<=0.2)
        r==0.2;
 printf("alpha=%f r=%f phi=%f \n",(float)alpha,r,phi_modulo_360);

}
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 1000.0);
   glMatrixMode (GL_MODELVIEW);
}
void kbup(unsigned char key,int x,int y)
{
    if(key=='A'||key=='a')
        pressed['A'-65] = 0;
    else if(key=='S'||key=='s')
        pressed['S'-65] = 0;
    else if(key=='J'||key=='j')
        pressed['J'-65] = 0;
    else if(key=='K'||key=='k')
        pressed['K'-65] = 0;
    else if(key=='o'||key=='O')
        pressed['O'-65] = 0;
    else if(key=='l'||key=='L')
        pressed['L'-65] = 0;
    else if(key=='y'||key=='Y')
        pressed['Y'-65] = 0;
    else if(key=='h'||key=='H')
        pressed['H'-65] = 0;
    update();
}
void kb(unsigned char key,int x,int y)
{
    if(key=='A'||key=='a')
        pressed['A'-65] = 1;
    else if(key=='S'||key=='s')
        pressed['S'-65] = 1;
    else if(key=='J'||key=='j')
        pressed['J'-65] = 1;
    else if(key=='K'||key=='k')
        pressed['K'-65] = 1;
    else if(key=='o'||key=='O')
        pressed['O'-65] = 1;
    else if(key=='l'||key=='L')
        pressed['L'-65] = 1;
    else if(key=='y'||key=='Y')
        pressed['Y'-65] = 1;
    else if(key=='h'||key=='H')
        pressed['H'-65] = 1;
    else if(key==' ')
        glDisable(GL_LIGHT1);
    else if(key=='q')
        lightAngle-=5;
    else if(key=='w')
        lightAngle+=5;
    else if(key==27)
        exit(0);
    update();
}
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1000, 1000);
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(kb);
   glutKeyboardUpFunc(kbup);
   glutMainLoop();
   return 0;
}
