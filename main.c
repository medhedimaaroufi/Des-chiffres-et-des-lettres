////// INCLUSION DES BIBLIOTHEQUES //////
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include <conio.h>


////// DECLARATION DES CONSTANTES  //////
#define delay 30
#define vowel "aeiouy"
#define consonant "bcdfghjklmnpqrstvwxz"

typedef struct index_char
{
    char firstChar;
    long int position;
} index_char;

////// FONCTION POUR CREER LE FICHIER INDEX //////
void CreateIndex(char *path_dict){
    FILE *dict=fopen(path_dict,"r");
    FILE *index=fopen("index.txt","wb");
    struct index_char ind_char;
    char firstChar=96;
    char buffer[30];
    while(fgets(buffer, sizeof(buffer), dict)){
        if (*buffer!=firstChar){
            firstChar++;
            if (*buffer==firstChar){
                ind_char.firstChar=firstChar;
                ind_char.position=ftell(dict)-strlen(buffer);
                fwrite ( &ind_char, sizeof(struct index_char),1, index );
                }
        }
    }
    fclose(dict);
    fclose(index);
}

////// FONCTION POUR VERIFICATION DE PREMIER MOT DE CHAQUE LETTRE //////
void VerifyIndex(char *path_dict , char *path_index){
    FILE *dict=fopen(path_dict, "r");
    FILE *index=fopen(path_index, "rb");
    struct index_char ind_char;
    char buffer[30];
    while(fread (&ind_char, sizeof(struct index_char), 1, index)){
        printf("char: %c\t", ind_char.firstChar);
        fseek(dict,ind_char.position,SEEK_SET);
        fgets(buffer, sizeof(buffer), dict);
        printf("mot: %s\n", buffer);
    }
}

////// FONCTION POUR CHERCHER UNE MOT //////
int FindWord(char *path_dict, char *path_index, char *word){
    FILE *dict=fopen(path_dict, "r");
    FILE *index=fopen(path_index, "rb");
    struct index_char ind_char;
    char buffer[30];
    while(fread (&ind_char, sizeof(struct index_char), 1, index))
        if (*word==ind_char.firstChar){
            fseek(dict, ind_char.position, SEEK_SET);
            break;
        }
    while (fgets(buffer, sizeof(buffer), dict) && *word==*buffer){
        buffer[strchr(buffer, '\n')-buffer]='\0';
        if (!strcmp(word, buffer))
            return 1;
    }
    return 0;
}

////// FONCTION POUR GENERER 10 LETTRES ALEATOIREMENT AVEC LE NBRE DE VOYELLES CHOISI //////
char *Generate10Char(int nb_vowel){
    char *word=malloc(11*sizeof(char));
    int i=0,j=0;
    if(word!=NULL){
        srand(time(NULL));
        while (j<10)
        {
            if (i<nb_vowel){
                word[j]=vowel[rand()%6];
                i++;
                j++;
            }
            else{
                word[j]=consonant[rand()%20];
                j++;
            }
        }
        word[10]='\0';
        return word;
    }
}

////// FONCTION POUR COMPTER LE TEMPS //////
int timer() {
     int i;
    for (i = delay; i >= 0; --i) {
        printf("\rTime remaining: %d seconds", i);
        fflush(stdout);
        sleep(1);
    }
    printf("\nCountdown finished!\n");
    return 0;
}






