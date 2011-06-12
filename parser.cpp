//importing a blender model in opengl
#include<stdio.h>

int main()
{
    FILE* fptr;
    fptr=fopen("car1.obj","r");
    int i=326;
    int faces[318][4],v1,v2,v3,v4;
    float vert[326][3];
    char g;
    double vertx,verty,vertz;
    if(fptr)
    {

    while(i)
    {
            fscanf(fptr,"%c",&g);
            fscanf(fptr,"%c",&g);
            printf("%c",g);
            fscanf(fptr,"%lf",&vertx);
            fscanf(fptr,"%lf",&verty);
            fscanf(fptr,"%lf",&vertz);
            vert[326-i][0]=vertx;
            vert[326-i][1]=verty;
            vert[326-i][2]=vertz;
            printf("(%lf,",vert[326-i][0]);
            printf("%lf,",vert[326-i][1]);
            printf("%lf)\n",vert[326-i][2]);
            i--;
    }
    i=318;
       while(i)
    {
            fscanf(fptr,"%c",&g);
            fscanf(fptr,"%c",&g);
            printf("%c",g);
            fscanf(fptr,"%d",&v1);
            fscanf(fptr,"%d",&v2);
            fscanf(fptr,"%d",&v3);
            fscanf(fptr,"%d",&v4);
            faces[318-i][0]=v1;
            faces[318-i][1]=v2;
            faces[318-i][2]=v3;
            faces[318-i][3]=v4;
            printf("(%d,",faces[318-i][0]);
            printf("%d,",faces[318-i][1]);
            printf("%d,",faces[318-i][2]);
            printf("%d)\n",faces[318-i][3]);
            i--;
    }
    }
}
