#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
//de testat tot!
//dar de testat pe bucati
typedef struct node //structura
{
struct node *prev;
struct node *next;
double timestamp;
double value;
}node, *DList;
void listinsert1(DList s,double val)
{
//functie ce insereaza intr-o lista valori la inceputul ei
//se presupune ca lista a fost deja initializata
   node *newnode;
    while(s->next!=NULL)
        s=s->next;
    newnode=(node*)malloc(sizeof(node));
    newnode->value=val;
    newnode->prev=s;
    s->next=newnode;
    newnode->next=NULL;
}
void listinsert2(DList s,double time,double val)
{
//functie ce insereaza intr-o lista valori la inceputul ei
//se presupune ca lista a fost deja initializata
    node *newnode;
    while(s->next!=NULL)
        s=s->next;
    newnode=(node*)malloc(sizeof(node));
    newnode->timestamp=time;
    newnode->value=val;
    newnode->prev=s;
    s->next=newnode;
    newnode->next=NULL;
}
void listdel(DList s,double val)
{
    //aici este o functie care doreste sa elimine un nod din lista, cu o anumita valoare
    DList todel;
    while(s->next!=NULL)
    {
    //se parcurge lista pana la final
        if(s->next->value==val)
        {
        //daca s-a gasit un nod cu valoarea cautata
            todel=s->next; //acesta se sterge cu ajutorul unei variabile ajutatoare
            s->next=todel->next;
            todel->next->prev=s;
            todel->next=NULL;
            todel->prev=NULL;
            free(todel);
        }
        s=s->next; //se trece mai departe in cautarea unui nod care ar putea avea valoarea cautata
    }
}
void listfree(DList s)
{
//eliberare memorie stergand toate nodurile dintr-o lista
    DList todel;
    todel=s;
    while(todel->next!=NULL)
            {
            s->next=todel->next; //se sterge de fiecare data primul nod
            todel->next->prev=NULL;
            free(todel);
            todel=s;
            }
}
void listprint1(DList s)
{
//prima versiune de afisare a listei, asa cum este prezentata in 2.1
int numarnoduri;
numarnoduri=0;
node *cursor;
cursor=s;
    while(cursor!=NULL)
    {
        cursor=cursor->next; //se parcurge lista cu ajutorul unui cursor
        numarnoduri++; //se numara cate noduri sunt in lista si se afiseaza
    }
    numarnoduri--;
    printf("%d\n",numarnoduri);
    cursor=s;
    cursor=cursor->next;
    while(cursor!=NULL)
    {
    printf("%.2lf\n",cursor->value); //valorile se afiseaza pe ecran (stdout)
        cursor=cursor->next;
    }
}
void listprint(DList s)
{
//prima versiune de afisare a listei, asa cum este prezentata in 2.1
int numarnoduri;
numarnoduri=0;
node *cursor;
cursor=s;
    while(cursor!=NULL)
    {
        cursor=cursor->next; //se parcurge lista cu ajutorul unui cursor
        numarnoduri++; //se numara cate noduri sunt in lista si se afiseaza
    }
    numarnoduri--;
    printf("%d\n",numarnoduri);
    cursor=s;
    cursor=cursor->next;
    while(cursor!=NULL)
    {
    printf("%.0lf ",cursor->timestamp); //valorile se afiseaza pe ecran (stdout)
    printf("%.2lf\n",cursor->value); //valorile se afiseaza pe ecran (stdout)
        cursor=cursor->next;
    }
}
void listprint2(DList s)
{
//o a doua varianta de afisare a listei, asa cum este aratat in 2.2
node *cursor;
cursor=s;
printf("[");
    while(cursor!=NULL)
    {
    printf("%lf,",cursor->value); //valorile se afiseaza pe ecran (stdout)
        cursor=cursor->next; //se parcurge lista cu ajutorul unui cursor
    }
    printf("]");
}
void listprint3(DList s)
{
//prima versiune de afisare a listei, asa cum este prezentata in 2.3
int numarnoduri;
numarnoduri=0;
node *cursor;
cursor=s;
    while(cursor!=NULL)
    {
        cursor=cursor->next; //se parcurge lista cu ajutorul unui cursor
        numarnoduri++; //se numara cate noduri sunt in lista si se afiseaza
    }
    printf("%d\n",numarnoduri);
    cursor=s;
    while(cursor!=NULL)
    {
    printf("%lf %lf\n",cursor->timestamp,cursor->value); //valorile se afiseaza pe ecran (stdout)
        cursor=cursor->next;
    }
}
void one(DList s)
{
//rezolvarea lui 2.1 -> e1
//mere
node *cursor;
node *elementinsine;
node *finish;
node *q;
DList start;
int contor;
int i,n;
double xm,x;
double num;
double sigma;
double elementinsinev;
double st,dr; //pentru a calcula mai usor intervalul in care sa se verifice apartenenta numarului
s=(DList)malloc(sizeof(node));
    s->next=NULL;
    s->prev=NULL;
    scanf("%d",&n);
    for (i=0;i<n;i++){
        scanf("%lf",&x);
        listinsert1(s,x);
    }
q=s->next;
while (q->next->next->next->next!=NULL)
{
    xm=q->value+q->next->value+q->next->next->value+q->next->next->next->value+q->next->next->next->next->value;
    xm=xm/5;
    sigma=(q->value-xm)*(q->value-xm)+(q->next->value-xm)*(q->next->value-xm)
    +(q->next->next->value-xm)*(q->next->next->value-xm)+(q->next->next->next->value-xm)*(q->next->next->next->value-xm)
    +(q->next->next->next->next->value-xm)*(q->next->next->next->next->value-xm);
    sigma=sigma/5;
    sigma=sqrt(sigma);
    if(q->next->next->value<xm-sigma||q->next->next->value>xm+sigma)
        listdel(s,q->next->next->value);
        q=q->next;
}
listprint1(s); //afiseaza rezultatul
//listfree(s); //elibereaza memoria
}
void twoone(DList s)
{
node *q;
node *ceva;
DList newnewlist,newlist;
int k,n;
double x;
int ok;
node *i;
double aux;
s=(DList)malloc(sizeof(node));
    s->next=NULL;
    s->prev=NULL;
    scanf("%d",&n);
    for (k=0;k<n;k++){
        scanf("%lf",&x);
        listinsert1(s,x);
    }
    newlist=(DList)malloc(sizeof(node));
    newlist->next=NULL;
    newlist->prev=NULL;
double toadd;
//din nou ia cate cinci si sorteaza alea cate cinci si apoi ia mijlocul
q=s->next;
while (q->next->next->next->next!=NULL)
{
    newnewlist=(DList)malloc(sizeof(node));
    newnewlist->next=NULL;
    newnewlist->prev=NULL;
    listinsert1(newnewlist,q->value);
    listinsert1(newnewlist,q->next->value);
    listinsert1(newnewlist,q->next->next->value);
    listinsert1(newnewlist,q->next->next->next->value);
    listinsert1(newnewlist,q->next->next->next->next->value);
    //pana aci stiu ca merge
do
{
  ok = 1;
  for(i = newnewlist->next ; i->next!=NULL ; i=i->next)
    if(i->value > i->next->value)
    {
      aux = i->value;
      i->value= i->next->value;
      i->next->value = aux;
      ok = 0;
    }
}
while(!ok);
toadd=newnewlist->next->next->next->value;
listinsert1(newlist,toadd);
q=q->next;
//listfree(newnewlist);
}
listprint1(newlist);
//listfree(s);
//listfree(newlist);
}

void twotwo(DList s)
{
node *q;
node *ceva;
DList newnewlist,newlist;
int k,n;
double x;
double sum;
node *i;
double aux;
s=(DList)malloc(sizeof(node));
    s->next=NULL;
    s->prev=NULL;
    scanf("%d",&n);
    for (k=0;k<n;k++){
        scanf("%lf",&x);
        listinsert1(s,x);
    }
    newlist=(DList)malloc(sizeof(node));
    newlist->next=NULL;
    newlist->prev=NULL;
double toadd;
//din nou ia cate cinci si sorteaza alea cate cinci si apoi ia mijlocul
q=s->next;
while (q->next->next->next->next!=NULL)
{
    newnewlist=(DList)malloc(sizeof(node));
    newnewlist->next=NULL;
    newnewlist->prev=NULL;
    listinsert1(newnewlist,q->value);
    listinsert1(newnewlist,q->next->value);
    listinsert1(newnewlist,q->next->next->value);
    listinsert1(newnewlist,q->next->next->next->value);
    listinsert1(newnewlist,q->next->next->next->next->value);
    //pana aci stiu ca merge
    sum=0;
for (i=newnewlist->next;i!=NULL;i=i->next)
    sum=sum+i->value;
    sum=sum/5;
listinsert1(newlist,sum);
q=q->next;
//listfree(newnewlist);
}
listprint1(newlist);
//listfree(s);
//listfree(newlist);
}

void three(DList s)
{
node *cursor;
int k,n;
double x,y;
double tdiff; //diferenta de timp
s=(DList)malloc(sizeof(node));
    s->next=NULL;
    s->prev=NULL;
    scanf("%d",&n);
    for (k=0;k<n;k++){
        scanf("%lf",&x);
        scanf("%lf",&y);
        listinsert2(s,x,y);
    }
cursor=s->next->next;
while (cursor!=NULL)
{
    printf("ok1\n");
tdiff=cursor->timestamp-cursor->prev->timestamp;
if(tdiff>=100||tdiff<=1000)
{
    printf("ok2\n");
cursor->timestamp=(cursor->prev->timestamp+cursor->timestamp)/2;
cursor->value=(cursor->prev->value+cursor->value)/2;
}
cursor=cursor->next;
}
listprint(s);
//listfree(s);
}
/*
void four(DList s,double prag)
{
//de facut
node *cursor;
node *st,*dr;
node *lft,*rght;
double i;
double w;
double C;
double temp;
double sum1,sum2;
int j,l;
listinit(s);
listcreate2(s);
cursor=s;
while(cursor->next!=NULL)
{
if (cursor->next->timestamp-cursor->timestamp>prag*1000)
{
i=cursor->timestamp;
i=i+prag*1000/5;
while (i<=cursor->next->timestamp)
{
listinsert3(s,i-prag*1000/5,i);
st=s;
dr=s;
while(st->next->timestamp!=i)
st=st->next;
dr=st;
for (j=0;j<3;j++)
dr=dr->next;
C=(i-st->timestamp)/(dr->timestamp-st->timestamp);
lft=s;
rght=s;
while(rght!=NULL)
rght=rght->next;
for (j=0;j<3;j++)
rght=rght->prev;
for (j=0;j<3;j++)
{
w=(((j/(3-1))*(j/(3-1)))*0.9+0.1); //w(j,k)
temp=0;
for (l=j;l<=3;l++)
temp=temp+(((j/(3-1))*(j/(3-1)))*0.9+0.1);
w=w/temp;
for (l=j;l<=3;l++)
{
//parcurgere ptr lft si rght in fct de l ul din for
//w(i,k) in fct de l-ul din for
}
}
}
}
}
listprint3(s);
listfree(s);
}
*/
void five(DList s,double delta)
{
node *cursor;
double stop;
double first,last;
double nr;
int k,n;
double x,y;
cursor=(node*)malloc(1*sizeof(node));
s=(DList)malloc(sizeof(node));
    s->next=NULL;
    s->prev=NULL;
    scanf("%d",&n);
    for (k=0;k<n;k++){
        scanf("%lf",&x);
        scanf("%lf",&y);
        listinsert2(s,x,y);
    }
cursor=s->next;
while(cursor->next!=NULL)
{
    cursor=cursor->next;
}
stop=cursor->timestamp;
first=0;
last=delta;
while(last<=stop)
{
cursor=s->next;
nr=0;
while (cursor!=NULL)
{
    if (cursor->timestamp>=first&&cursor->timestamp<=last)
nr++;
cursor=cursor->next;
}
printf("[%.0lf,%.0lf] %.0lf\n",first,last,nr);
first=first+delta;
last=last+delta;
}
//listfree(s);
}
int main(int argc,char **argv)
{
    int i,n;
    //int j,k;
    DList s;
    //char num[4];
    //double delta;
    //double x;
//    k=0; j=1;
    /*for(i=1;i<argc;i++)
    if(strcmp(argv[i],"--e1")==0)
    one(s);
    else
    if(strcmp(argv[i],"--e2")==0)
    twoone(s);
    else
    if(strcmp(argv[i],"--e3")==0)
    twotwo(s);
    else
    if(strcmp(argv[i],"--u")==0)
    three(s);
    else
    if(strcmp(argv[i],"--c")==0)
    four(s);
    else
    if(strchr(argv[i],'s')==0)
    {
    num[k++]=s[j++];
    for(j=0;j<strlen(num);j++)
    delta=delta*10+num[j];
    five(s,delta);
    }
    */
    five(s,250);
    /*

    listprint1(s);
    */
    return 0;
}
