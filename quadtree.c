#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <inttypes.h>
#define RGB_COMPONENT_COLOR 255
typedef struct QuadtreeNode { 
	unsigned char blue,green,red;	
	uint32_t area;
	int32_t top_left,top_right;
	int32_t bottom_left,bottom_right;
 }__attribute__((packed)) QuadtreeNode;
 //acesata de mai sus este structura grid in cerinta
typedef struct pixel{
	unsigned char r,g,b;
}pixel;
//aceasta este una dintre structurile de care am avut nevoie, una de a stoca un pixel din imagine
typedef struct image{
     char type[3]; //P6
     int32_t width, height;
     int32_t rgbcompcolour; //255
     pixel **grid;
} image;
//aici sa zicem ca e imaginea propriu zisa impreuna cu metadatele/header-ul
typedef struct Tree{
	pixel color;
	long long index;	
	uint32_t area;
	struct Tree *top_left,*top_right;
	struct Tree *bottom_left,*bottom_right;
 }__attribute__((packed))Tree;
 //well, aici e arborele propiu zis, pentru ca nu am stiut exact cum sa il parcurg fara sa-l vad ca pe o lista
image read(char *filename)
{
	FILE *in; //fisierul de citire, imaginea ppm
	char typebuffer[3]; //urmeaza o serie de un fel de buffere in care voi citi chestii
    int widthbuffer,heightbuffer;
    int maxcolorbuffer;
    int i;
    char space; //aceasta ajuta la ignorarea (citirea, ca oricum trebuie sa le citesc) a spatiilor goale
    pixel **matrix; //acesta este bufferul pentru imagine
    image img; //acesta e un fel de dummy, un fel de chestie ajutatoare in care sa stochez imaginea
    matrix=(pixel**)malloc(sizeof(pixel*)*heightbuffer); 
	in=fopen(filename,"rb"); //deschid fisierul de intrare
    if (in==NULL)
        exit(1); //daca nu l-a putut deschide, crapa
    //urmeaza citirea buffer-elor de la header/metadate
	fscanf(in,"%s",typebuffer);
	fscanf(in,"%d",&widthbuffer);
    fscanf(in,"%d",&heightbuffer);
    fscanf(in,"%d",&maxcolorbuffer);
    //verific corectitudinea datelor, asta asa, ca sa fie 
    if (strcmp("P6",typebuffer)!=0)
        exit(1);
	if (maxcolorbuffer!=RGB_COMPONENT_COLOR)
        exit(1);
    //dupa ce am citit headerul, citesc spatiile goale
    //si apoi trec la  citirea imaginii binare, alocand pe parcurs memorie pentru stocarea ei
	fread(&space,sizeof(char),1,in);
    for(i=0;i<heightbuffer;i++)
	{	
		matrix[i]=malloc(sizeof(pixel)*widthbuffer);
		fread(matrix[i],sizeof(pixel),widthbuffer,in);
	}
	fclose(in); //inchid fisierul si pun datele in dummy
    strcpy(img.type,typebuffer);
    img.width=widthbuffer;
    img.height=heightbuffer;
    img.rgbcompcolour=maxcolorbuffer;
    img.grid=matrix;
	return img; 
}
void compress(image img,Tree **t,int x,int y,int size,int factor)
{
    //aici este rezolvarea primei cerinte cu compresia imaginii
	int i,j;
	unsigned long long int R;
    unsigned long long int G;
    unsigned long long int B;
    //aici mai sus sunt componentele pentru culoarea medie
    unsigned long long int mean; //aici e media care se va calcula cu formula
    R=0;
	G=0;
	B=0;
	mean=0; //la inceput sumele sunt 0
	(*t)=malloc(sizeof(Tree)); //aloc spatiu pentru nodurile din arbore
	(*t)->area=size*size;
    //in aceste for-uri se calucleaza valoarea medie a culorilor
    //se face suma per total a fiecarei componente de culoare
    //iar acea suma se imparte la dimensiune (ca e patrat, de aceea e size*size)
    //si asa se face culoarea medie pe componente
	for(i=y;i<y+size;i++)
		for(j=x;j<x+size;j++)
		{
			R=R+img.grid[i][j].r;
            G=G+img.grid[i][j].g;
            B=B+img.grid[i][j].b;
		}
        R=R/(size*size);
        G=G/(size*size);
        B=B/(size*size);
        //culorile medii se pun in arbore
	(*t)->color.r=R;
	(*t)->color.g=G;
    (*t)->color.b=B;
    //si se verifica formula cu scorul similaritatii
	for(i=y;i<y+size;i++)
		for(j=x;j<x+size;j++)
		{	
			mean=mean+((R-img.grid[i][j].r)*(R-img.grid[i][j].r))+((G-img.grid[i][j].g)*(G-img.grid[i][j].g))+
            ((B-img.grid[i][j].b)*(B-img.grid[i][j].b));
		}
	mean=mean/(3*size*size);
	if(mean>factor) //daca e mai mare ca pragul, se divide in continuare
	{
		compress(img,&(*t)->top_left,x,y,size/2,factor);
        compress(img,&(*t)->top_right,x+(size/2),y,size/2,factor);
        compress(img,&(*t)->bottom_right,x+(size/2),y+(size/2),size/2,factor);
        compress(img,&(*t)->bottom_left,x,y+(size/2),size/2,factor);
		return;
    }
	else
	{
        //daca nu se mai poate divide, atunci e frunza, prin urmare toate legaturile sunt nule
		(*t)->top_right=NULL;
		(*t)->top_left=NULL;
		(*t)->bottom_left=NULL;
		(*t)->bottom_right=NULL;
		return;
	}
}
/*
functia asta (de mai jos) ma ajuta sa creez un fel de vector de lagaturi ca sa 
stiu ce si unde pun
*/
void createdummy(Tree *t,Tree **v[],unsigned int *i)
{
    //aici fac un vector de legaturi ca sa stiu unde anume pun pixelii
	if(t!= NULL)
	{
        //cat timp mai are de parcurs in arborele de compresie
        if((*i)>0)
      		(*v)=realloc((*v),sizeof(Tree*)*((*i)+1)); //fac loc pentru o noua legatura
        (*v)[(*i)]=t; //pun ce a ramas din arbore acolo
        t->index=(*i); //si spun al catalea nod e
        (*i)++; //si trec la urmatoarul nod
        createdummy(t->top_left,v,i); //parcurg conform cerintei, recursiv, pana la frunze
        createdummy(t->top_right,v,i);
        createdummy(t->bottom_right,v,i);
        createdummy(t->bottom_left,v,i);
	}
}
void createvectorfromtree(Tree **t,QuadtreeNode **v,int i)
{
    //si la asta sa vad ce explic
    //care e faza cu index-ul, nr de ordine?
	unsigned int j;
	for(j=0;j<i;j++) //se parcurge vectorul de compresie
	{
		(*v)[j].red=t[j]->color.r; 
        //al j-ulea nod din arbore primeste datele respective
        (*v)[j].green=t[j]->color.g;
        (*v)[j].blue=t[j]->color.b;
		(*v)[j].area=t[j]->area; 
        //(mai sus)pun culorile care sa formeze ce e comprimat in si dimensiunea blocului
        if(t[j]->top_left != NULL)	 
        //daca nu e frunza -> se ia pe cazuri pentru toate cele 4 sectiuni
			(*v)[j].top_left=t[j]->top_left->index; //ii pun nr de ordine
		else
			(*v)[j].top_left=-1; //altfel il marchez ca fiind frunza
		if(t[j]->top_right!=NULL)	
			(*v)[j].top_right=t[j]->top_right->index;
		else
			(*v)[j].top_right=-1;
		if(t[j]->bottom_right!=NULL)	
			(*v)[j].bottom_right=t[j]->bottom_right->index;
		else
			(*v)[j].bottom_right=-1;
		if(t[j]->bottom_left!=NULL)	
			(*v)[j].bottom_left=t[j]->bottom_left->index;
		else
			(*v)[j].bottom_left=-1;
	}
}
void createtreefromvector(QuadtreeNode *v,Tree **t,int i) //aci e oke
{
	(*t) = malloc(sizeof(Tree)); //aloc arborele de compresie
	(*t)->color.r=v[i].red; //si iau din vector culoarea potrivita sa o pun in arbore
	(*t)->color.g=v[i].green;
	(*t)->color.b=v[i].blue;
	(*t)->area=v[i].area; //inclusiv dimensiunea pe care e
	(*t)->index=i; //si nr de ordine din vector
	if(v[i].top_left!=-1&&v[i].top_right!=-1&&v[i].bottom_left!=-1&&v[i].bottom_right!=-1)
	{
        //daca nu e frunza, mai bine zis atata timp cat nu e frunza, merg mai departe, recursiv
		createvectorfromtree(v,&(*t)->top_left,v[i].top_left); //parcurg nu neaparat in ordine, merge si asa
		createvectorfromtree(v,&(*t)->top_right,v[i].top_right);
		createvectorfromtree(v,&(*t)->bottom_left,v[i].bottom_left);
		createvectorfromtree(v,&(*t)->bottom_right,v[i].bottom_right);
	}
	else
	{
        /*
        daca s-a ajuns la frunza, adica acolo unde e -1 la zona/legatura,
        atunci legaturile din nodul respectiv din arbore sunt nule, 
        nemaiavnd altundeva unde sa se duca
        */
		(*t)->top_left=NULL;
		(*t)->top_right=NULL;
		(*t)->bottom_left=NULL;
		(*t)->bottom_right=NULL;
	}
}
void decompress(Tree *t,image *img,int x,int y,int size)
{
	int i,j;
	if(t->top_left==NULL&&t->top_right==NULL&&t->bottom_right==NULL&&t->bottom_left==NULL)
	{ //atata timp cat nu e frunza, reformeaza imaginea in binar, in matricea in care tine imaginea
		for(i=y;i<y+size;i++)
			for(j=x;j<x+size;j++)
			{
				/*
                i=y=vertical=linii; j=x=orizontal=coloane
                for-urile se deplaseaza cat e dimensiunea imaginii, 
                (area din structura de arbore), mai ales ca de obicei x si y 
                (de unde se porneste) se iau zero, la apel
                */
                (*(img->grid))[i][j].r=t->color.r; //se ia pe culori
                (*(img->grid))[i][j].g=t->color.g;
                (*(img->grid))[i][j].b=t->color.b;
			}
	}
	else
	{
		decompress(t->top_left,img,x,y,size/2); 
        /*
        aici percurge recursiv vbectorul de compresie, refacand 
        imaginea progresiv, mai ales ca  tot asa, se ia pe patrimi, 
        in ordinea specificata in enunt cea a acelor de ceasornic
        => mereu se injumatateste si se aduna, ca imaginea e impartita in patru, 
        deci dimesniunea unei linii, cum ar veni, ar fi jumtate 
        */
        /*
        _____________ etc etc se mai imparte fiecare sfert in sfertiri si tot asa...
        |     |     |
        |_____|_____|
        |     |     | jumatate din latura mare
        |_____|_____|
          ^jumatate din latura mare 

        */
        decompress(t->top_right,img,x+(size/2),y,size/2);
        decompress(t->bottom_right,img,x+(size/2),y+(size/2),size/2);
        decompress(t->bottom_left,img,x,y+(size/2),size/2);
		}
	free(t);
}
void write(image img,char *filename)
{
//aceasta functie scrie imaginea noua in fisierul out
	FILE *out;
    int i;
    out=fopen(filename,"w");
    if(out==NULL)
    exit(1);
	fprintf(out,"P6\n"); //scriu elementele duin imagine in ordine: 
    //1. formatul (stiu deja ca e P6, desi puteam sa-l iau si din structura imagine img)
    //2.dimensiunile (in general, am vazut ca e o imagine-patrat)
    //3.numarul de pixeli rgb, de obicei 255
    fprintf(out,"%d %d\n",img.width,img.width); //imagine patratica, ca sa spun asa;
    //deci, nu conteaza care dintre lungime sau latime iau
	fprintf(out,"255\n");
	for(i=0;i<img.width;i++)
		fwrite(img.grid[i],sizeof(pixel),img.width,out);//scriu linie cu linie imaginea, in binar
	fclose(out); //la final nu uit sa inchid fisierul
}
void vertically(Tree **t)
{
	if((*t)->top_left != NULL && (*t)->top_left != NULL && (*t)->top_left != NULL && (*t)->top_left != NULL)
	{
		 /*
		 daca nu e frunza, sau mai bine zis atata timp cat nu e frunza, 
		 pentru ca aceasta este conditia de oprire la recursivitate
		 */
		//se parcurge pana la frunze, in ordinea din enunt (ca un fel de ace de ceasornic)
		vertically(&(*t)->top_left);
		vertically(&(*t)->top_right);
		vertically(&(*t)->bottom_right);
		vertically(&(*t)->bottom_left);
		Tree * aux;
		/*
		acesta e un nod ajutator care va fi folosit 
		la interschombarea dintre nodurile arborelui, mai exact la interschimbrea 
		nodului stanga sus cu cel stanga-jos si dreapta-jos cu cel drepta-sus pentru ca
		se face schimbare stanga dreapta, fiind o rasucire verticala a imaginii
		*/
		aux=(*t)->top_left;
		(*t)->top_left=(*t)->bottom_left;
		(*t)->bottom_left=aux;
		aux=(*t)->top_right;
		(*t)->top_right=(*t)->bottom_right;
		(*t)->bottom_right=aux;
	}
}
void horizontally(Tree **t)
{
	
    if((*t)->top_left!=NULL&&(*t)->top_left!=NULL&&(*t)->top_left!=NULL&&(*t)->top_left!=NULL)
	{
    //daca nu e frunza, sau mai bine zis atata timp cat nu e frunza, pentru ca aceasta este conditia de oprire la recursivitate
		horizontally(&(*t)->top_left); //se parcurge pana la frunze, in ordinea din enunt (ca un fel de ace de ceasornic)
		horizontally(&(*t)->top_right);
		horizontally(&(*t)->bottom_right);
		horizontally(&(*t)->bottom_left);
		Tree * aux;
		 /*
        acesta e un nod ajutator care va fi folosit 
        la interschimbarea dintre nodurile arborelui, mai exact la interschimbrea 
        nodului stanga sus cu cel drepta-sus si stanga jos cu cel drepta-jos pentru ca
        se face schimbare sus-jos, fiind o rasucire orizontala a imaginii
        */
        //apoi se face schimbul, pe rand, dar se revine din recursivitate de fiecare grid
		aux=(*t)->top_left;
		(*t)->top_left=(*t)->top_right;
		
		(*t)->top_right=aux;
        /*
        o grid se face schimbul cu cele din blocurile de sus (cele 3 randuri anterior)
        abia apoi se face schimbul cu cele din blocurile de jos (vezi mai jos)
        */
		aux=(*t)->bottom_left;
		(*t)->bottom_left=(*t)->bottom_right;
		(*t)->bottom_right=aux;
	}
}
void deallocate(Tree **t)
{
	if((*t)->top_left!=NULL&&(*t)->top_left!=NULL&&(*t)->top_left!=NULL&&(*t)->top_left!=NULL)
	{
        //atata timp cat nu e frunza si mai are de parcurs
		deallocate(&(*t)->top_left); //dau free recursiv la blocuri
		deallocate(&(*t)->top_right);
		deallocate(&(*t)->bottom_right);
		deallocate(&(*t)->bottom_left);
	}
	free((*t)); //se face eliberare incepand de jos-stanga si merge invers pana se elibereaza tot
}
int main(int argc, char **argv)
{
	if (strcmp(argv[1], "-c") == 0)
	{
		unsigned int colors; //numarul de pixeli de culoare din vectorul de compresie
	unsigned int nr; //nr noduri care ar trebui sa fie in arborele cuaternar care are imaginea
    unsigned int i; //asta ma ajuta la parcurgeri 
	int factor; //DA
	image img; //imaginea propriu-zisa
	Tree *t; 
    Tree **v; //aici e vectorul cu legaturile nodurilor din arbore
    QuadtreeNode *vector; //vectorul de compresie
    v=malloc(sizeof(Tree*)); //acesta este vectorul
    t=NULL; //initializez arborele
    nr=0;
    colors=0;
	img=read(argv[3]);
	factor=atoi(argv[2]);
	compress(img,&t,0,0,img.width,factor);
    /*
    img.width=dimensiunea imaginii
    fiind o imagine/matrice patratica, se ia numai o dimensiune. eu am luat latimea
    la inceput e size total pentru ca se ia mai intai intreaga imagine
    si apoi gen pe parcurs se imparte in cate 4 blocuri si tot asa...
    */
	createdummy(t,&v,&colors);
    vector=malloc(sizeof(QuadtreeNode)*colors); 
    //-^ aloc aici pentru ca pe parcurs se modifica dimensiunea, grid prin index-ul de mai sus
	createvectorfromtree(v,&vector,colors);
	for(i=0;i<colors;i++)
	free(v[i]); //elibrerez ce am alocat
	free(v);
    //abia acum scriu imaginea noua
	FILE *out=fopen(argv[4],"wb"); //acesta este fisierul out
	for(i=0;i<colors;i++) 
	{
	if(vector[i].top_left==-1) 
    /*deci:
    1. sunt -1 ca sunt frunze, caci in algoriotmul descris
    in pdf spunea ca este suficienta alegerea nodurilor terminale
    2.daca e frunza, atunci se numara si aia, ca nu merge pana la frunze
	*/
    nr++;
	}
	fwrite(&nr,sizeof(int),1,out);//scriu nr de noduri
	fwrite(&colors,sizeof(int),1,out);//scriu numarul de culori 
	for(i=0;i<colors;i++)
	{
	fwrite(&vector[i],sizeof(QuadtreeNode),1,out); //scriu vectorul de compresie
	if(vector[i].top_left == -1)
		nr++; //si daca e frunza o numar
	}
	free(vector);
    //eliberez din nou ce am alocat anterior
	for(i=0;i<img.height;i++)
	free(img.grid[i]); //eliberez pe linii
	free(img.grid); //eliberez matricea in care era tinuta imaginea
	fclose(out);
	}
	else 
		if (strcmp(argv[1], "-d") == 0)
		{
			unsigned int nr; //numar noduri arbore
    unsigned int i; //ajuta la parcurgeri
	unsigned int colors; //numar culori/pixeli culoare din vectorul de compresie
    uint32_t size; //dimensiune imagine
    QuadtreeNode *vector; //vector de compresie
    Tree *t; //atborele de compresie
    image img; //imaginea propriu zisa
	FILE *in;
    in=fopen(argv[2],"rb");
    if (in==NULL)
    exit(1);
	t=NULL; //initializez arborele
	fread(&colors,sizeof(unsigned int),1,in); //citesc nr de culori 
	fread(&nr,sizeof(unsigned int),1,in); //citesc nr noduri	
	vector=malloc(sizeof(QuadtreeNode)*nr);
	fread(vector,sizeof(QuadtreeNode),nr,in); //aici citesc vectorul de culori
	fclose(in);
	createtreefromvector(vector,&t,0); //aici citesc arborele din vectorul de compresie
	free(vector);//eliberez vectorul
	size=sqrt(t->area);	 //iau dimensiunea imaginii din arbore			
	img.grid=(pixel**)malloc(sizeof(pixel*)*size); //aloc memeorie pentru imagine
	for(i=0;i<size;i++)	
	img.grid[i]=malloc(sizeof(pixel)*size); //e doar un size ca e imagine/matrice patratica
	decompress(t,&img.grid,0,0,size); 
    //0,0 sau mai bine zis x si y, pentru a arata de unde sa inceapa decompresia
	write(img,argv[3]);//abia acum scriu imaginea
	for(i=0;i<size;i++) 
    //aici eliberez ce am mai alocat pe parcurs, adica matricea imaginii stocate in binar
	free(img.grid[i]);
	free(img.grid);			
		}
		else
			if (strcmp(argv[1], "-m") == 0)
			{
				unsigned int i;
	int factor;
	image img1,img2;
	Tree *t;
	t=NULL;
	factor=atoi(argv[3]);
	img1=read(argv[4]);
	compress(img1,&t,0,0,img1.width,factor);
	//(mai jos gen) verific ce fel de oglindire/rotire este
	if(strcmp(argv[2],"h") == 0) //i have so many questions but oke
	horizontally(&t);
	if(strcmp(argv[2], "v") == 0)
	vertically(&t);
    img2.width=img2.height=img1.width;
    img2.grid=(pixel**)malloc(sizeof(pixel*)*img2.width);
	for(i=0;i<img2.width;i++)		
		img2.grid[i]=malloc(sizeof(pixel)*img2.width);
	decompress(t,&img2,0,0,img2.width);
	write(img2,argv[5]); //scriu imaginea noua
		//eliberez ce am alocat, adica matricile de pixeli cu imaginile
				for(i=0;i<img2.width;i++)
					free(img2.grid[i]);
				free(img2.grid);
				for(i=0;i<img1.height;i++)
					free(img1.grid[i]);
				free(img1.grid);
			}
	return 0;
}
