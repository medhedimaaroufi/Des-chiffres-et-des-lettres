////// INCLUSION DES BIBLIOTHEQUES //////
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>


////// DECLARATION DES CONSTANTES  //////
#define delay 3
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
    FILE *index=fopen("index.bin","wb");
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
        free(word);return word;
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

////// FONCTION DE CONTROLE DE SAISI //////
void Input(char *word,char *control,int player){
    int i;

    printf("Answer Player_%d : ", player+1);
    scanf("%s",word);

    for (i=0;i< strlen(word);i++){
        if (strchr(control,word[i])==NULL) return Input(word,control,player);
    }
}

//////  FONCTION POUR CALCULER LE SCORE //////
void Score(int *score,char *path_dict, char *path_index, char *word1, char *word2){
    int longWord1,longWord2,FindWord1,FindWord2;

    FindWord1= FindWord(path_dict,path_index,word1); //VERIFIER L'EXISTANCE DE WORD1
    FindWord2= FindWord(path_dict,path_index,word2); //VERIFIER L'EXISTANCE DE WORD2

    longWord1=strlen(word1);
    longWord2=strlen(word2);

    if(longWord1>longWord2)
        if (FindWord1)
            *score+=longWord1;
        else
            *(score+1)+=longWord1;
    else if(longWord1< longWord2)
        if (FindWord2)
            *(score+1)+=longWord2;
        else
            *score+=longWord2;
    else{
        if (FindWord1 && FindWord2) {
            *score += longWord1;
            *(score+1) += longWord1;
        }
        else if (FindWord1 && !FindWord2)
            *score+=longWord1;
        else if (!FindWord1 && FindWord2)
            *(score+1)+=longWord2;
    }
}




int main(){
    char path_dict[]="dictionnaire.txt";
    char path_index[]="index.bin";
    char *word1,*word2,control[]="abrez";
    int nbVowel=0;
    int player=1;
    int *score[2];


    //CreateIndex(path_dict); //CREATION DU FICHIER INDEX

    printf("Number of vowel: "); //SAISI DU NBRE DE VOYELLES
    scanf("%d",&nbVowel);

    //control=Generate10Char(nbVowel); //GENERATION DES 10 LETTRES ALEATOIREMENT
    printf("Generated Chars : %s\n", control);

    timer();

    Input(word1,control,!player); //SAISI DES 10 LETTRES DE JOUEUR 1
    Input(word2,control,player);  //SAISI DES 10 LETTRES DE JOUEUR 2
    printf("hello world\n");
    Score(score,path_dict,path_index,word1,word2); //CALCUL DU SCORE

    printf("Score Player_1 : %d\n",*score);
    printf("Score Player_2 : %d\n",*(score+1));



    return 0;
}
