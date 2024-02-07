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
#define path_cleanDict "cleanDict.txt"
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

//////EFFACER LES MOTS DE LONGUEUR SUPERIEURE A 10 //////
void CleanDict(){
    FILE *dict=fopen(path_dict, "r");
    FILE *cleanDict=fopen(path_cleanDict, "w");
    char buffer[17];
    while(fgets(buffer, sizeof(buffer), dict)){
        if (strlen(buffer)<11)
            fputs(buffer, cleanDict);

    }
    fclose(dict);
    fclose(cleanDict);
}


////// FONCTION POUR CREER LE FICHIER INDEX //////
void CreateIndex(char *path){
    FILE *dict=fopen(path,"r");
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
void VerifyIndex(char *path ){
    FILE *dict=fopen(path, "r");           // OUVRIR LE FICHIER DICT EN MODE LECTURE
    FILE *index=fopen(path_index, "rb");   // OUVRIR LE FICHIER INDEX EN MODE LECTURE BINAIRE
    struct index_char ind_char;                           // STRUCTURE POUR STOCKER LES INDEX DE DICTIONNAIRE
    char buffer[30];

    // BOUCLE POUR LIRE CHAQUE ENTREE DE L'INDEX
    while(fread (&ind_char, sizeof(struct index_char), 1, index)){
        printf("char: %c\t", ind_char.firstChar);
        fseek(dict,ind_char.position,SEEK_SET);     // POSITIONNER LE CHARIOT DU LECTURE DE DICT A LA POSITION INDIQUEE PAR L'INDEX
        fgets(buffer, sizeof(buffer), dict);
        printf("mot: %s\n", buffer);
    }
}

////// ENTRER LE NOM DU JOUEUR //////
void EnterName(struct player *player){
    printf("Player_%d name : ",player->id);
    scanf("%s",player->name);
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

////// FONCTION DE CONTROLE DE SAISI //////
int Input(struct player *player,char *control){
    int i;
    char *S,*P;

    printf("Answer %s : ", player->name);
    scanf("%s",player->word);

    S= strchr(player->word,'\n');
    if(S!=NULL) *S='\0';

    S=(char *)malloc(11);
    strcpy(S,control);

    for (i=0;i< strlen(player->word);i++){
        P=strchr(S,player->word[i]);
        if (P==NULL) {
            printf("Wrong input\n");
            return Input(player,control);
        }
        else{
            *P=' ';
        }
    }

    free(S);

    return 0;
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

////// FONCTION POUR CHERCHER UNE MOT //////
int FindWord(char *word){
    FILE *dict=fopen(path_cleanDict, "r");
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

//////  FONCTION POUR CALCULER LE SCORE //////
void Score(struct player *player1,struct player *player2){
    int longWord1,longWord2,FindWord1,FindWord2;

    FindWord1= FindWord(player1->word);     //VERIFIER L'EXISTANCE DE WORD1
    FindWord2= FindWord(player2->word);     //VERIFIER L'EXISTANCE DE WORD2

    longWord1=strlen(player1->word);      //LA LONGUEUR DU 1ER MOT
    longWord2=strlen(player2->word);      //LA LONGUEUR DU 2EME MOT

    if(longWord1>longWord2)                 //MOT JOUEUR 2 PLUS LONGUE
        if (FindWord1)                      //JOUEUR 1 GAGNE LA MANCHE
            player1->score+=longWord1;
        else                                //JOUEUR 2 GAGNE LA MANCHE
            player2->score+=longWord1;
    else if(longWord1 < longWord2)          //MOTS JOUEUR 2 PLUS LONGUE
        if (FindWord2)                      //JOUEUR 2 GAGNE LA MANCHE
            player2->score+=longWord2;
        else                                //JOUEUR 1 GAGNE LA MANCHE
            player1->score+=longWord2;
    else                                    //EGALITE DES MOTS
    {
        if (FindWord1 && FindWord2) {       //EGALITE SCORE DE LA MANCHE
            player1->score += longWord1;
            player2->score+= longWord1;
        }
        else if (FindWord1 && !FindWord2)   // JOUEUR 1 GAGNE LA MANCHE
            player1->score+=longWord1;
        else if (!FindWord1 && FindWord2)   // JOUEUR 2 GAGNE LA MANCHE
            player2->score+=longWord2;
    }
}

////// FONCTION POUR LANCER UNE MANCHE //////
void Manche(struct player *player1,struct player *player2,int round){
    int nbVowel=0;                              //NBRE DE VOYELLES CHOISI PAR L'UTILISATEUR
    char *control;                              //LA PLUS LONGUE MOT DE DICTIONNAIRE A 15 LETTRES
    EnterName(player1);
    EnterName(player2);

    printf("--%s chooses a number of vowels ", round ? player2->name : player1->name );  //SAISI DU NBRE DE VOYELLES
    scanf("%d",&nbVowel);

    control=Generate10Char(nbVowel);   //GENERATION DES 10 LETTRES ALEATOIREMENT
    printf("Generated Chars : %s\n", control);

    timer();

    Input(round ? player2 : player1 ,control);             //SAISI DES 10 LETTRES DE JOUEUR 1
    Input(round ? player1 : player2 ,control);             //SAISI DES 10 LETTRES DE JOUEUR 2

    Score(player1,player2);                    //METTRE A JOUR LE SCORE

    printf("\nName\t %s %s \n",player1->name,player2->name);
    printf("Score\t   %d    %d \n",player1->score,player2->score);

}

int main(){
    struct player player1={1}; //STRUCTURE POUR LE JOUEUR 1
    struct player player2={2}; //STRUCTURE POUR LE JOUEUR 2
    //CleanDict();
    //CreateIndex(path_cleanDict); //CREATION DU FICHIER INDEX
    //VerifyIndex(path_cleanDict); //VERIFICATION DU FICHIER INDEX
    Manche(&player1,&player2,0);


    return 0;
}