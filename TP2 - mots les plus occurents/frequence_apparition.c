#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 40
#define M 200
#define NEG_INFTY -10000000

char motActuel[N];
char avant[N];
char ligneActuelle[M];
int sizeActuel = 0;
int minimum = 0;

typedef struct Abr {
    char lettre;
    int nbIterations;

    struct Abr *enfant;
    struct Abr *suivant;
} abr ;

typedef struct Couple {
    char mot[N];
    int nbIterations;

    int sizeMot;
} couple ;

int est_un_char_valide(char x)
{
    return ((x >= '0' && x <= '9') || (x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'));
}

int printMotActuel(int sizeActuel) // affiche le mot courant - utile pour debug
{
    for (int i = 0 ; i < sizeActuel ; i++)
    {
        printf("%c", motActuel[i]);
    }
    return 0;
}

void ajouterNoeud(abr **arbre, char lettreCourante)
{
    abr *nouveauNoeud = malloc(sizeof(abr));
    nouveauNoeud->lettre = lettreCourante;
    nouveauNoeud->nbIterations = 0;
    nouveauNoeud->enfant = NULL;
    nouveauNoeud->suivant = NULL;

    abr *tmpNoeud;
    abr *tmpArbre = *arbre;
    if(tmpArbre != NULL)
    {
        while(tmpArbre != NULL)
        {
            tmpNoeud = tmpArbre;
            tmpArbre = tmpNoeud->suivant;
            if(tmpArbre == NULL) 
            {
                tmpNoeud->suivant = nouveauNoeud;
            }
        }
    }
    else 
    {
        *arbre = nouveauNoeud;
    }
}

int existe(abr **arbre, char lettreCourante)
{
    abr *tmpNoeud;
    abr *tmpArbre = *arbre;
    if(tmpArbre != NULL)
    {
        while(tmpArbre != NULL)
        {
            if (tmpArbre->lettre == lettreCourante)
            {
                //printf("La lettre %c existe !\n", lettreCourante);
                return 1;
            }
            tmpNoeud = tmpArbre;
            tmpArbre = tmpNoeud->suivant;
        }
    }
    return 0;
}

void printArbreLargeurNbIterations(abr **arbre)
{
    printf("()\n");
    while (*arbre != NULL)
    {
        printf("%c : %d et ses enfants :\n", (*arbre)->lettre, (*arbre)->nbIterations);
        printArbreLargeurNbIterations(&((*arbre)->enfant));
        printf("Fin des enfants de %c.\n", (*arbre)->lettre);
        arbre = &((*arbre)->suivant);
    }
}

void afficheTousLesMots(abr **arbre, int sizeAvant)
{
    abr *a = *arbre;
    if (a->nbIterations > 0)
    {
        for (int i = 0 ; i < sizeAvant ; i++)
        {
            printf("%c", avant[i]);
        }
        printf("%c : %d\n", a->lettre, a->nbIterations);
    }
    if (a->enfant != NULL)
    {
        avant[sizeAvant] = a->lettre;
        afficheTousLesMots(&(a->enfant), sizeAvant + 1);
    }
    if (a->suivant != NULL)
    {
        afficheTousLesMots(&(a->suivant), sizeAvant);
    }
}


void afficheMots(abr **arbre, int sizeAvant, couple **motsEtIterations, int nbMots)
{
    abr *a = *arbre;
    if (a != NULL)
    {
        if (a->nbIterations > minimum)
        {
            int sizeMot = sizeAvant + 1;
            char mot[sizeAvant + 1];
            for (int i = 0 ; i < sizeAvant ; i++)
            {
                mot[i] = avant[i];
            }
            mot[sizeAvant] = (a->lettre);
            //printf("on va ajouter %s , a noter que sizeAvant = %d!\n", mot, sizeAvant + 1);
            int nbIterations = (a->nbIterations);
            char motToChange[N];
            int nbIterToChange = 0;
            int sizeMotToChange = 0;
            char motToChangeTmp[N];
            int nbIterToChangeTmp = 0;
            int sizeMotToChangeTmp = 0;
            int yet = 0;
            for (int i = 0 ; i < nbMots ; i++)
            {
                if (((*motsEtIterations)[i]).nbIterations < nbIterations && !(yet))
                {
                    yet = 1;
                    strcpy(motToChange,(*motsEtIterations)[i].mot);
                    nbIterToChange = (*motsEtIterations)[i].nbIterations;
                    sizeMotToChange = (*motsEtIterations)[i].sizeMot;
                    strcpy((*motsEtIterations)[i].mot,mot);
                    (*motsEtIterations)[i].nbIterations = nbIterations;
                    (*motsEtIterations)[i].sizeMot = sizeMot;
                } else {
                    if (yet)
                    {
                        strcpy(motToChangeTmp,(*motsEtIterations)[i].mot);
                        nbIterToChangeTmp = (*motsEtIterations)[i].nbIterations;
                        sizeMotToChangeTmp = (*motsEtIterations)[i].sizeMot;
                        strcpy((*motsEtIterations)[i].mot,motToChange);
                        (*motsEtIterations)[i].nbIterations = nbIterToChange;
                        (*motsEtIterations)[i].sizeMot = sizeMotToChange;
                        strcpy(motToChange,motToChangeTmp);
                        nbIterToChange = nbIterToChangeTmp;
                        sizeMotToChange = sizeMotToChangeTmp;
                    }
                }
                if (i == nbMots - 1)
                {
                    minimum = nbIterToChange;
                }
            }
        }
        if (a->enfant != NULL)
        {
            avant[sizeAvant] = a->lettre;
            afficheMots(&(a->enfant), sizeAvant + 1, motsEtIterations, nbMots);
        }
        if (a->suivant != NULL)
        {
            afficheMots(&(a->suivant), sizeAvant, motsEtIterations, nbMots);
        }
    }
}


void printMot(int sizeActuel)
{
    for (int i = 0 ; i < sizeActuel ; i++)
    {
        printf("%c", motActuel[i]);
    }
    printf("\n");
}

// si stopWords == 1 on enleve les mots de stop_words.txt, sinon on remplit notre arbre
void parcours(abr **arbre, int sizeActuel, int stopWords)
{
    //printf("arbre vide ?\n");
    for (int i = 0 ; i < sizeActuel ; i++)
    {
        char caractereActuel = motActuel[i];
        if (!existe(arbre, caractereActuel)) // ne fait rien si la lettre existe deja, et l'ajoute sinon
        {
            ajouterNoeud(arbre, caractereActuel);
            //printf("on a ajoute %c\n", caractereActuel);
        }
        while ((*arbre)->lettre != caractereActuel) // s'arrete quand tmpNoeud->lettre == caractereActuel
        {
            arbre = &((*arbre)->suivant);
        }
        if (i == sizeActuel - 1)
        {
            if (stopWords)
            {
                (*arbre)->nbIterations = (*arbre)->nbIterations + NEG_INFTY;
            } else {
                (*arbre)->nbIterations = (*arbre)->nbIterations + 1;
            }
        } else {
            arbre = &((*arbre)->enfant); // on prend l'enfant de la lettre actuelle
        }            
    }
}

int main(int argc, char *argv[]) // TODO : faire en sorte que les fichiers texte soient dans argv, ainsi que n
{
    if (argc != 3)
    {
        printf("\nTo make this program work you need to provide two arguments : a text file and a number n.\nThis program will then compute the n words that appears the most in the text file (except the words in stop_words.txt and the worlds of one letter).\n\n");
        return 0;
    }
    // ouverture du fichier
    FILE* fichier = NULL;
    fichier = fopen(argv[1], "r");
    int n = atoi(argv[2]);

    // creation de l arbre
    abr *arbre = NULL;
    int step = 0;

    if (fichier != NULL)
    {
        while (fgets(ligneActuelle, M, fichier) != NULL) // On lit le fichier ligne par ligne, et on stocke cela dans ligneActuelle
        {
            for (int i = 0 ; i < strlen(ligneActuelle) ; i++)
            {
                char caractereActuel = ligneActuelle[i];
                if (est_un_char_valide(caractereActuel))
                {
                    if (caractereActuel >= 'A' && caractereActuel <= 'Z')
                    {
                        caractereActuel += 'a' - 'A';
                    }
                    motActuel[sizeActuel] = caractereActuel;
                    sizeActuel++;
                    //printf("%c", caractereActuel); // On l'affiche
                } else {
                    //printMot(sizeActuel);
                    if (sizeActuel > 1) 
                    {
                        parcours(&arbre, sizeActuel, 0);
                    }
                    //printArbreLargeur(&arbre);
                    sizeActuel = 0;
                }
            }
            //printf("%s", ligneActuelle);
        }
        fclose(fichier);
        // au cas ou le fichier termine sur un caractere :
        if (sizeActuel > 1) 
        {
            parcours(&arbre, sizeActuel, 0);
        }
        //printArbreLargeur(&arbre);
        sizeActuel = 0;
    }

    // "suppression" des mots interdits
    fichier = fopen("stop_words.txt", "r");

    if (fichier != NULL)
    {
        while (fgets(ligneActuelle, M, fichier) != NULL) // Une seule ligne ici
        {
            for (int i = 0 ; i < strlen(ligneActuelle) ; i++)
            {
                char caractereActuel = ligneActuelle[i];
                if (est_un_char_valide(caractereActuel))
                {
                    if (caractereActuel >= 'A' && caractereActuel <= 'Z')
                    {
                        caractereActuel += 'a' - 'A';
                    }
                    motActuel[sizeActuel] = caractereActuel;
                    sizeActuel++;
                } else {
                    if (sizeActuel > 0) 
                    {
                        parcours(&arbre, sizeActuel, 1);
                    }
                    sizeActuel = 0;
                }
            }
        }
        fclose(fichier);
        // au cas ou le fichier termine sur un caractere :
        if (sizeActuel > 0) 
        {
            parcours(&arbre, sizeActuel, 1);
        }
        //printArbreLargeur(&arbre);
        sizeActuel = 0;
    }

    //printArbreLargeurNbIterations(&arbre);

    // on cherche les mots avec la plus grande occurence.
    //afficheTousLesMots(&arbre, 0);
    strcpy(avant, "");
    //printf("\n\n\n");
    couple *l = NULL;
    l = malloc(n * sizeof(couple));
    if (l != NULL)
    {
        for (int i = 0 ; i < n ; i++)
        {
            strcpy(l[i].mot, "");
            l[i].nbIterations = 0;
            l[i].sizeMot = 0;
        }
        afficheMots(&arbre, 0, &l, n);
    }
    for (int i = 0 ; i < n ; i++)
    {
        for (int j = 0 ; j < l[i].sizeMot ; j++)
        {
            printf("%c", l[i].mot[j]);
        }
        printf(" : %d\n", l[i].nbIterations);
    }
    return 0;
}