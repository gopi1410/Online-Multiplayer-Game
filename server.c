#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#include<time.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

//function prototypes
void change_r(int);
void error(const char*);
void createLights(void);
void init(void);
void init_net(void);
void send_recieve(void);

static GLfloat tilt=0.0;
static GLfloat car[2][2][2][3];
GLboolean pressed[27],first=1;
int counter=0;
float iterator,iterator2;
const GLfloat PI=3.141592;GLfloat theta=0.0;
const GLfloat WHITE[] = {1.0,1.0,1.0,1.0}, BLACK[] = {0.0,0.0,0.0,1.0}, RED[] = {1.0,0.0,0.0,1.0};

GLfloat carAmbDiff[] = {1.0,0.0,1.0,1.0};
GLfloat remote_car_amb_diff[] = {1.0,0.0,1.0,1.0};
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

int sockfd, newsockfd;
socklen_t clilen;
typedef struct sendfloat
{
	float alpha1;float phi1;float r1;
}x;
x buffer;
x* bptr;
x user_car,remote_car;
float zero=0.0,pt1=0.1;       
float a,b,c;
struct sockaddr_in serv_addr, cli_addr;
struct sockaddr_in* from;
struct hostent *dest;
char DEST_IP[16];
int n,DEST_PORT=5000;

void createLights()
{       GLfloat light0Pos[] = {0.0,1.0,1.0,0.0};
        GLfloat light0Diffuse[] = {0.3,0.3,0.3,1.0};
        GLfloat light0Specular[] = {1.0,1.0,0.0,1.0};
        GLfloat ambLight[] = {0.0,0.0,0.0,1.0};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambLight);
        glLightfv(GL_LIGHT0,GL_POSITION,light0Pos);
        glLightfv(GL_LIGHT0,GL_SPECULAR,light0Specular);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,light0Diffuse);
        glLightfv(GL_LIGHT1,GL_SPECULAR,light0Specular);
        glLightfv(GL_LIGHT1,GL_AMBIENT,light0Diffuse);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
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
void init_net(void)
{
	
	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	
     if (sockfd < 0)
        error("ERROR opening socket");

	dest=gethostbyname(DEST_IP);
	if(dest==NULL)
	{
	fprintf(stderr,"ERROR!! Invalid IP");
	exit(0);
	}
	bzero((char *) &cli_addr, sizeof(cli_addr));
	cli_addr.sin_family=AF_INET;
	cli_addr.sin_port=htons(DEST_PORT);
	cli_addr.sin_addr.s_addr=inet_addr(DEST_IP);
	memset(&(cli_addr.sin_zero),'\0',8);

     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY; //uses self ip address
     serv_addr.sin_port = htons(DEST_PORT);
	 memset(&(serv_addr.sin_zero),'\0',8);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	int nonBlocking = 1;
    if ( fcntl(sockfd, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
    {
        error( "failed to set non-blocking socket sockfd\n" );
    }
	
}
void init_car_model(void)
{
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
}
void init_key_press(void)
{
	for(counter=1;counter<=26;counter+=1)
	        pressed[counter]=0;
}
void init(void)
{
	printf("Enter the ip address of other machine:  ");
	scanf("%s",DEST_IP);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDisable(GL_CULL_FACE);
	glShadeModel (GL_SMOOTH);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 1000.0);
	glMatrixMode (GL_MODELVIEW);
	init_net();
	init_car_model();
	init_key_press();
	createLights();
}
void transform_world()
{
    glTranslatef(0.0,-0.0,-5.1);
    glRotatef(-user_car.phi1,0.0,1.0,0.0);
    glTranslatef(-user_car.r1*sin(user_car.alpha1*PI/180),0.0,-user_car.r1*cos(user_car.alpha1*PI/180));
}
void draw_remote_car()
{
	glPushMatrix();
	glTranslatef(0.0,-5.0,-0.0);
	transform_world();
	glTranslatef(remote_car.r1*sin(remote_car.alpha1*PI/180),0.0,remote_car.r1*cos(remote_car.alpha1*PI/180));
	glRotatef(remote_car.phi1,0.0,1.0,0.0);
	glRotatef(-180,0.0,1.0,0.0);
	int n1=rand()%100;int n2=rand()%70;int n3=rand()%80;
       remote_car_amb_diff[0]=0.5;
       remote_car_amb_diff[1]=1.1;
       remote_car_amb_diff[2]=1.2;
       glBegin(GL_QUADS);
       glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,remote_car_amb_diff);
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
	glEnd();

	glPopMatrix();
}
void drawCar()
{   glPushMatrix();

      glTranslatef(0.0,-5.0,-5.1);
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

          /*      glVertex3f(-5.5,iterator+1,-iterator2);
                glVertex3f(-5.5,iterator+1,-iterator2-0.5);
                glVertex3f(-5.5,iterator+1.5,-iterator2-0.5);
                glVertex3f(-5.5,iterator+1.5,-iterator2);*/

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

                /*glVertex3f(5.5,iterator+1,-iterator2);
                glVertex3f(5.5,iterator+1,-iterator2-0.5);
                glVertex3f(5.5,iterator+1.5,-iterator2-0.5);
                glVertex3f(5.5,iterator+1.5,-iterator2);*/

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
   draw_remote_car();
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
        user_car.phi1 = user_car.phi1 + 3.0;
    else if(pressed['K'-65])
        user_car.phi1 = user_car.phi1 - 3.0;
    else if(pressed['O'-65])
        theta = theta + 0.1;
    else if(pressed['L'-65])
        theta = theta - 0.1;
    else if(pressed['H'-65])
        change_r(+1);
    else if(pressed['Y'-65])
        change_r(-1);
	send_recieve();
	glutPostRedisplay();
	
}

void change_r(int flag_mov_dir)
{
    GLfloat rn;
    rn=user_car.r1;
    int t,d_alpha_sign;
    float phi_modulo_360,d_alpha;
    if(user_car.phi1>0)
    {
        t=(int)(user_car.phi1/360);
        phi_modulo_360=user_car.phi1-t*360;
    }
    else
    {
        t = -user_car.phi1/360 + 1;
        phi_modulo_360=user_car.phi1+t*360;
    }
    if(user_car.alpha1>360.0)
    {
        user_car.alpha1 = user_car.alpha1 - 360;
    }
    else if(user_car.alpha1<0.0)
    {
        user_car.alpha1 = 360.0 + user_car.alpha1;
    }

    if(flag_mov_dir==1)                                                             // if car is being moved forward
    {
        user_car.r1=sqrt((float)(rn*rn+0.01+2*rn*0.1*cos((float)((phi_modulo_360-user_car.alpha1)*PI/180))));
        d_alpha=180*acos((float)((rn*rn+user_car.r1*user_car.r1-0.01)/(2*user_car.r1*rn)))/PI;
	if(isnan(d_alpha))
	{
		if((rn*rn+user_car.r1*user_car.r1-0.01)/(2*user_car.r1*rn)>0.9)
			d_alpha=0;
	}
        if(user_car.alpha1<=180)
        {
            if(phi_modulo_360<=(user_car.alpha1+180)&&phi_modulo_360>=user_car.alpha1)
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
                if(phi_modulo_360>=(user_car.alpha1-180)&&phi_modulo_360<=user_car.alpha1)
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
        user_car.r1=sqrt((float)(rn*rn+0.01-2*rn*0.1*cos((float)((phi_modulo_360-user_car.alpha1)*PI/180))));
        d_alpha=180*acos((float)((rn*rn+user_car.r1*user_car.r1-0.01)/(2*user_car.r1*rn)))/PI;
	if(isnan(d_alpha))
	{
		if((rn*rn+user_car.r1*user_car.r1-0.01)/(2*user_car.r1*rn)>0.9)
			d_alpha=0;
	}
        if(user_car.alpha1<=180)
            {
                if(phi_modulo_360<=(user_car.alpha1+180.0)&&phi_modulo_360>=user_car.alpha1)
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
                if(phi_modulo_360>(user_car.alpha1-180.0)&&phi_modulo_360<user_car.alpha1)
                {
                    d_alpha_sign=+1;
                    
                }
                else
                {
                    d_alpha_sign=-1;
                }
        }
    }
    user_car.alpha1+=d_alpha_sign*d_alpha;
     if(fabs(user_car.r1*sin(user_car.alpha1*PI/180))>4.0)          //collision detection
           {user_car.r1=rn;user_car.alpha1-=d_alpha_sign*d_alpha;}
    if(user_car.alpha1<0)
        user_car.alpha1 = 360 + user_car.alpha1;
    else if(user_car.alpha1>360)
        user_car.alpha1 = user_car.alpha1 - 360;
    if(user_car.r1<=0.2)
       user_car.r1==0.2;
 //printf("alpha=%f r=%f phi=%f \n",(float)user_car.alpha1,user_car.r1,phi_modulo_360);

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
    else if(key=='<')
        glDisable(GL_LIGHT1);
      else if(key=='>')
        glDisable(GL_LIGHT1);
    else if(key=='q')
        lightAngle-=5;
    else if(key=='w')
        lightAngle+=5;
    else if(key==27)
        exit(0);
    update();
}
void send_recieve()
{
		
	a=0.0;b=0.0;c=0.0;
		clilen = sizeof(cli_addr);
		buffer.alpha1=0.0;
		buffer.phi1=0.0;
		buffer.r1=0.0;
		bptr=&buffer;
		from=&cli_addr;
		socklen_t fromLength = sizeof( from );
		while(1)
	{
		n=recvfrom(sockfd,bptr,255,0,(struct sockaddr*)from,&fromLength);
		if(n>=0)
		{
			//printf("Here is the message: %f  %f  %f\n",bptr->alpha1,bptr->phi1,bptr->r1);
			remote_car.alpha1=bptr->alpha1;
			remote_car.phi1=bptr->phi1;
			remote_car.r1=bptr->r1;
	
		 }   
		else 
		break;
	}
		
	
		buffer.alpha1=0.0;
		buffer.phi1=0.0;
		buffer.r1=0.0;
		buffer.alpha1=user_car.alpha1;
		buffer.phi1=user_car.phi1;
		buffer.r1=user_car.r1;
		bptr=&buffer;
		n=sendto(sockfd,bptr,sizeof(buffer),0,(struct sockaddr*)from,sizeof(struct sockaddr_in));
		if(n<0);
		
}
void timerCallback(int value)
{
	send_recieve();
	glutPostRedisplay();
	glutTimerFunc (250,timerCallback,value);
}
int main(int argc, char** argv)
{
		
	           remote_car.phi1=zero;
        	   remote_car.r1=pt1*10;
        	   remote_car.alpha1=-90;
		   user_car.phi1=zero;
        	   user_car.r1=pt1*10;
        	   user_car.alpha1=90;
		   glutInit(&argc, argv);
		   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		   glutInitWindowSize (500, 500);
		   glutInitWindowPosition (0, 0);
		   glutCreateWindow (argv[0]);
		   init ();
		   glutTimerFunc (250,timerCallback,0);
		   glutDisplayFunc(display);
		   glutReshapeFunc(reshape);
		   glutKeyboardFunc(kb);
		   glutKeyboardUpFunc(kbup);
		   glutMainLoop();
		   close(sockfd);
       		   return 0;
}