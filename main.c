#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Définir les constantes pour les symboles
#define program 1
#define id 2
#define pv 3
#define pt 4
#define var 5
#define dp 6
#define integer 7
#define chart 8
#define begin 9
#define end 10
#define v 11
#define aff 12
#define iff 13
#define then 14
#define elsee 15
#define read 16
#define readln 17
#define write 18
#define writeln 19
#define po 20
#define pf 21
#define doo 22
#define whilee 23
#define opadd 24
#define oprel 25
#define opmul 26
#define nb 27
#define ppe 28
#define dif 29
#define ppq 30
#define pgq 31
#define pge 32
#define ega 33
char test[34][34]={"0","program","id","pv","pt","var","dp","integer","chart","begin","end","v","aff","iff","then","elsee","read","readln","write","writeln","po","pf","doo","whilee","opadd","oprel","opmul","nb","ppe","dif","ppq","pqq","pge","ega"};
// Structure pour représenter un symbole lexical
typedef struct unilex {
    int ul;
    int att;
} unilex;
int *c = 0;
FILE* fp;
FILE* fo;
FILE* fan;
int mot_cle[20]={1,9,5,7,8,10,14,13,15,16,17,18,19,22,23};
char tab_mot_cle[20][20] = { "program", "begin", "var", "integer", "chart", "end", "then", "if", "else", "read", "readln", "write", "writeln", "do", "while" };
char tab_iden[100][20]; // Ajustez la taille du tableau au besoin
char car;
int alpha=0;
unilex symbole;
char ch[20]; // Assurez-vous que la taille de ch est suffisante pour stocker des identifiants
int z = 0;
void reculer(int k) {
    fseek(fp, -k, SEEK_CUR);
}

char carsuivant() {
    return fgetc(fp);
}
int unilexid() {
    int k = 0;
    int i = 0;
    while (i < 14 && (!k)) {
        if (strcmp(tab_mot_cle[i], ch) == 0) {
            //printf("mot cle: %s \n",tab_mot_cle[i]);
            /*int x;
            scanf("%d",&x);*/
            k = 1;
        } else {
            i++;
        }
    }
    if (k == 1){
        //printf("code mot cle : %d \n",mot_cle[i]);
        return mot_cle[i];
    }

    else {
        return id;
    }
}

int rangerid(int k, int* c) {
    int s = 0;

    if (k == id) {
            while ((strcmp(tab_iden[s], ch) != 0) && (s < (*c)))
                s++;
            if (s < *c) {

                return s;

            } else {
                (*c)++;
                strcpy(tab_iden[*c], ch);

                return *c;
            }}

    else{
        return 0;
    }


}

unilex analex() {
    int etat = 0;
    int colonne = 0, ligne = 1;


    while (1) {
        //printf("etat %d \n", etat);
        switch (etat) {
            case 0:
                car = carsuivant();
                if (car == ' ') {
                    colonne++;
                    etat = 0;


                } else if (car == '\n') {
                    ligne++;
                    colonne = 0;
                    etat = 0;

                }else if(car=='\t'){
                    colonne+=3;
                    etat=0;
                }
                else if (isalpha(car)){
                    z=0;

                    ch[0]=car;
                    /* printf("%c",car);
                     printf("coucou");
                     printf("%s",ch);
                     printf("coucou");*/
                    etat=1;
                }
                else if (isdigit(car)) {
                    etat = 3;
                } else if (car == '<') {
                    etat = 5;
                } else if (car == '>') {
                    etat = 10;
                } else if (car == '=') {
                    etat = 9;
                } else if (car == '.') {
                    //printf(" (end of file)\n");
                    etat = 13;
                }else if(car==';'){
                    etat=15;
                }
                else if (car==',')
                    etat=16;
                else if (car=='(')
                    etat=17;
                else if (car==':')
                    etat=18;
                else if (car==')')
                    etat=19;

                else {
                    etat = 14;
                }
                break;
            case 1:


                car = carsuivant();

                if (isalpha(car) || isdigit(car)) {
                    z++;
                    ch[z] = car;
                } else {
                    etat = 2;
                    ch[z + 1] = '\0';
                    //printf("this is the chaine : %s  \n",ch);
                    z = 0;
                }
                break;
            case 2:
                reculer(1);
                symbole.ul = unilexid();
                symbole.att = rangerid(symbole.ul, &c);
                return symbole;
            case 3:
                if (z==0) reculer(1);
                car = carsuivant();
                //printf("\n %s \n",&ch);
                if (isdigit(car)) {
                    z++;
                    ch[z] = car;
                } else {
                    etat = 4;
                    ch[z + 1] = '\0';
                    z = 0;
                }
                break;
            case 4:
                reculer(1);
                symbole.ul = nb;
                symbole.att = atoi(ch);
                return symbole;
            case 5:

                car = carsuivant();
                switch (car) {
                    case '=':
                        etat = 6;
                        break;
                    case '>':
                        etat = 7;
                        break;
                    default:
                        etat = 8;
                }
                break;
            case 9:
                symbole.ul = oprel;
                symbole.att = ega;

                return symbole;
            case 10:
                reculer(1);
                car = carsuivant();
                if (car == '=') {
                    etat = 11;
                } else {
                    symbole.ul = oprel;
                    symbole.att = pge;
                    return symbole;
                }
            case 11:
                symbole.ul = oprel;
                symbole.att = pgq;
                return symbole;
            case 6:
                symbole.ul = oprel;
                symbole.att = ppe;
                return symbole;
            case 7:
                symbole.ul = oprel;
                symbole.att = dif;
                return symbole;
            case 8:
                reculer(1);
                symbole.ul = oprel;
                symbole.att = ppq;
                return symbole;
            case 12:

                symbole.ul = oprel;
                symbole.att = pgq;
                return symbole;
            case 13:
                symbole.ul = 100;
                symbole.att = 0;
                return symbole;
            case 14:
                printf("Erreur : caractère inattendu à la ligne %d, colonne %d\n", ligne, colonne);
                etat=0;
                break;
            case 15:
                symbole.ul=pv;
                symbole.att=0;
                return symbole;
            case 16:
                symbole.ul=v;
                symbole.att=0;
                return symbole;
            case 17:
                symbole.ul=po;
                symbole.att=0;
                return symbole;
            case 18:
                ch[0]=car;
                car=carsuivant();
                if (car=='='){
                    symbole.ul=aff;
                    symbole.att=0;
                    return symbole;
                } else{
                    etat=20;
                }
                break;
            case 19:
                symbole.ul=pf;
                symbole.att=0;
                return symbole;
            case 20:
                reculer(1);
                symbole.ul=dp;
                symbole.att=0;
                return symbole;
        }
    }
}

//////////////////////////analyse_synatxique///////////////////////////////
int lignee=1,coll=1;
int symbole1,ty=0;
void dcl();
void inst_comp();
void d1();
int symbole_suivant();
void erreur();
void accepter(int);
void p();
void l_id();
void l1();
void type();
void inst();
void l_inst();
void l_inst1();
void i();
void exp();
void exp_simple();
void exp1();
void terme();
void exp_simple1();
void terme1();
void facteur();
int symbole_suivant()
{
	int s;
	fscanf(fan,"%d",&s);
	coll++;
	//printf("%d \n",s);
	return s;
}
///
void erreur()
{
	printf("symbole non accepte !! ligne: %d, colonne: %d \n",lignee, coll);
	ty+=1;
}
///
void accepter(int t)
{
    if (t==pv || t==begin){lignee++;coll=0;}
	if (symbole1 == t) symbole1=symbole_suivant();
	else
    {
        erreur();
        printf("expected %s found %s \n",test[t] ,test[symbole1]);
	}
}
///
void p()
{
	if(symbole1==program)
	{
		accepter(program);
		accepter(id);
		accepter(pv);
		dcl();
		inst_comp();
	    if (symbole1 == pt && ty==0) printf("algorithme correcte...:)");
	    else printf("algorithme incorrecte...:((");

	}
	else
		printf("erreur syntaxique...\n");
}
///
void dcl()
{
	d1();
}
///
void d1()
{
	if(symbole1==var)
	{
		accepter(var);
		l_id();
		accepter(dp);
		type();
		accepter(pv);
		d1();
	}

}
///
void l_id()
{
	if(symbole1==id)
	{
		accepter(id);
		l1();
	}
			else
	{
	printf("erreur : pas de variable.. \n");	ty+=1;}

}
///
void l1()
{
	if(symbole1==v)
	{
		accepter(v);
		accepter(id);
		l1();
	}

}
///
void type()
{
		if(symbole1==integer) accepter(integer);
		else {
            if (symbole1==chart) accepter(chart);
			else
            {
                printf("type invalide ..: %d  \n",symbole1);ty+=1;
            }
}

}
///
void inst_comp()
{
	if(symbole1==begin)
	{
		accepter(begin);
		inst();
		accepter(end);
	}
		else
	{
	printf("erreur de begin end \n");ty+=1;
	}

}
///
void inst()
{
l_inst();
}
///
void l_inst()
{
		i();
		l_inst1();

}
///
void l_inst1()
{
	if(symbole1==pv)
	{
		accepter(pv);
		i();
		l_inst1();
	}

}
///
void i()
{
	if(symbole1==id)
	{
		accepter(id);
		accepter(aff);
		exp_simple();
	}
	else{
		if (symbole1==iff)
		{
            accepter(iff);
            exp();
            accepter(then);
            i();
            accepter(elsee);
            i();
		}
			else{
				if (symbole1==whilee)
		{
            accepter(whilee);
            exp();
            accepter(doo);
            i();
		}
			else{
		if (symbole1==read)
		{
            accepter(read);
            accepter(po);
            accepter(id);
            accepter(pf);
			}
			else{
			 if (symbole1==readln)
		{
            accepter(readln);
            accepter(po);
            accepter(id);
            accepter(pf);
			}
			else{
			if (symbole1==write)
		{
            accepter(write);
            accepter(po);
            accepter(id);
            accepter(pf);
			}
			else{
			if (symbole1==writeln)
		{
            accepter(writeln);
            accepter(po);
            accepter(id);
            accepter(pf);
			}


	}}}}}
}

}

///
void exp()
{

		exp_simple();
		exp1();

}
///
void exp1()
{
	if(symbole1==oprel)
	{
		accepter(oprel);
		exp_simple();
	}

			else
	{
	printf("erreur pas de addition \n");ty+=1;
	}

}
///
void exp_simple()
{
		terme();
		exp_simple1();

}
///
void exp_simple1()
{
	if(symbole1==opadd)
	{
		accepter(opadd);
		terme();
		exp_simple1();
	}


}
///
void terme()
{
	facteur();
	terme1();
}
///
void terme1()
{
		if(symbole1==opmul)
	{
		accepter(opmul);
		facteur();
		terme1();
	}

}
///
void facteur()
{
		if(symbole1==id)

		accepter(id);

		else
        {
            if(symbole1==nb) accepter(nb);
            else
            {
                if(symbole1==po)
                {
                    accepter(po);
                    exp_simple();
                    accepter(pf);
                }
                else
                {
                printf("erreur de id \n");
                }
            }
        }
}




////////////main/////////////////////////////

int main() {
    char fichier[50];
    printf("Entrez le nom du fichier source : ");
    scanf("%s", fichier);

    fp = fopen(fichier, "r");
    fo = fopen("filout.txt", "w");
    printf("\n");
    if (fp == NULL) {
        printf("Impossible d'ouvrir le fichier %s.\n", fichier);
        return 1;
    }
    do {
        symbole = analex();

        if (symbole.ul==100) fprintf(fo,"%d ",4);
        else fprintf(fo,"%d ",symbole.ul);
        if(symbole.ul==3){fprintf(fo,"\n"); }
        if(symbole.ul==9){fprintf(fo,"\n"); }


    } while (symbole.ul != 100 );
    fclose(fp);
    fclose(fo);
    fan = fopen("filout.txt", "r");
    if (fan == NULL) {
        printf("Impossible d'ouvrir le fichier filout.\n");
        return 1;
    }
	fscanf(fan,"%d",&symbole1);
	p();
    fclose(fan);
    printf("\n\n");
    const char *nom_fichier = "filout.txt";
    remove(nom_fichier);



    return 0;
}
