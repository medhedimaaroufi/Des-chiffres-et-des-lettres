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
#define path_dict "dictionnaire.txt"
#define path_index "index.dat"

////// DECLARATION DES STRUCTURES //////
typedef struct index_char
{
    char firstChar;
    long int position;
} index_char;  //STRUCTURE POUR L'INDEXATION DE DICTIONNAIRE
typedef struct player{
    int id;
    char name[20];
    char word[11];
    int score;
}player;  //STRUCTURE POUR LES JOUEURS

////// FONCTION POUR CREER LE FICHIER INDEX //////
void CreateIndex(){
    printf("inCreateIndex");
    FILE *dict=fopen(path_dict,"r");
    FILE *index=fopen(path_index,"wb");
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
void VerifyIndex(){
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
int FindWord(char *word){
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


////// FONCTION DE CONTROLE DE SAISI //////
int Input(struct player *player,char *control){
    int i;
    char *S;
    printf("Answer %s : ", player->name);
    scanf("%s",player->word);

    S= strchr(player->word,'\n');
    if(S!=NULL) *S='\0';

    for (i=0;i< strlen(player->word);i++){
        if (strchr(control,player->word[i])==NULL) {
            printf("Wrong input\n");
            return Input(player,control);
        }
    }
    return 0;
}

//////  FONCTION POUR CALCULER LE SCORE //////
void Score(struct player *player1,struct player *player2){
    int longWord1,longWord2,FindWord1,FindWord2;
    printf("inScore");

    FindWord1= FindWord(player1->word); //VERIFIER L'EXISTANCE DE WORD1
    FindWord2= FindWord(player2->word); //VERIFIER L'EXISTANCE DE WORD2

    longWord1=strlen(player1->word);
    longWord2=strlen(player2->word);

    if(longWord1>longWord2)
        if (FindWord1)
            player1->score+=longWord1;
        else
            player2->score+=longWord1;
    else if(longWord1< longWord2)
        if (FindWord2)
            player2->score+=longWord2;
        else
            player1->score+=longWord2;
    else{
        if (FindWord1 && FindWord2) {
            player1->score += longWord1;
            player2->score+= longWord1;
        }
        else if (FindWord1 && !FindWord2)
            player1->score+=longWord1;
        else if (!FindWord1 && FindWord2)
            player2->score+=longWord2;
    }
}

////// ENTRER LE NOM DU JOUEUR //////
void EnterName(struct player *player){
    printf("Player_%d name : ",player->id);
    scanf("%s",player->name);
}

void Manche(struct player *player1,struct player *player2){
    int nbVowel=0; //NBRE DE VOYELLES CHOISI PAR L'UTILISATEUR
    char *control;  //LA PLUS LONGUE MOT DE DICTIONNAIRE A 15 LETTRES
    EnterName(player1);
    EnterName(player2);


    printf("Number of vowel: "); //SAISI DU NBRE DE VOYELLES
    scanf("%d",&nbVowel);

    control=Generate10Char(nbVowel); //GENERATION DES 10 LETTRES ALEATOIREMENT
    printf("Generated Chars : %s\n", control);

    timer();

    Input(&player1,control); //SAISI DES 10 LETTRES DE JOUEUR 1
    Input(&player2,control);  //SAISI DES 10 LETTRES DE JOUEUR 2

    Score(&player1,&player2); //METTRE A JOUR LE SCORE

    printf("Score Player_1 : %d\n",player1->score);
    printf("Score Player_2 : %d\n",player2->score);

}

int main(){
    struct player player1={1}; //STRUCTURE POUR LE JOUEUR 1
    struct player player2={2}; //STRUCTURE POUR LE JOUEUR 2

    CreateIndex(path_dict, path_index); //CREATION DU FICHIER INDEX
    VerifyIndex( path_dict, path_index); //VERIFICATION DU FICHIER INDEX


    return 0;
}
