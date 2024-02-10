////// INCLUSION DES BIBLIOTHEQUES //////
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>


////// DECLARATION DES CONSTANTES  //////
#define delay 30
#define vowel "aeiouy"
#define consonant "bcdfghjklmnpqrstvwxz"
#define path_dict "dictionnaire.txt"
#define path_cleanDict "cleanDict.txt"
#define path_index "index.dat"
#define path_ctrl "control.txt"

////// DECLARATION DES STRUCTURES //////
typedef struct index_char
{
    char firstChar;
    long int position;
} index_char;  //STRUCTURE POUR L'INDEXATION DE DICTIONNAIRE

typedef struct player{
    int id;
    int score;
    char name[16];
    char word[11];
}player;  //STRUCTURE POUR LES JOUEURS


////// Bienvenue //////
void Hello(){
     int i;
    char *HelloWorld ="\n\n\n\n      _   _      _ _         __        __         _     _\n"
                      "     | | | | ___| | | ___    \\ \\      / /__  _ __| | __| |\n"
                      "     | |_| |/ _ \\ | |/ _ \\    \\ \\ /\\ / / _ \\| '__| |/ _` |\n"
                      "     |  _  |  __/ | | (_) |    \\ V  V / (_) | |  | | (_| |\n"
                      "     |_| |_|\\___|_|_|\\___/      \\_/\\_/ \\___/|_|  |_|\\__,_|\n";

    for ( i = 0; HelloWorld[i] != '\0'; i++) {
        putchar(HelloWorld[i]);
        fflush(stdout);
        usleep(10 * 1000);
    }

    putchar('\n');
    sleep(3);
    system("cls");

}


//////EFFACER LES MOTS DE LONGUEUR SUPERIEURE A 10 //////
void CleanDict(){
    FILE *dict=fopen(path_dict, "r");
    FILE *cleanDict=fopen(path_cleanDict, "w");
    char buffer[17];
    while(fgets(buffer, sizeof(buffer), dict)){
        if (strlen(buffer)<12)
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
    printf("--> Player_%d name : ",player->id);
    scanf("%s",player->name);
    player->name[15] = '\0';
}

////// FONCTION POUR GENERER 10 LETTRES ALEATOIREMENT AVEC LE NBRE DE VOYELLES CHOISI //////
void Generate10Char(int nb_vowel,char *control){
    int i=0,j=0;
    while (j<10)
    {
        if (i<nb_vowel){
            control[j]=vowel[rand()%6];
            i++;
            j++;
        }
        else{
            control[j]=consonant[rand()%20];
            j++;
        }
    }
    control[10]='\0';
    return;
}

////// VERIFIER SI LES LETTRES CHOISIES SONT PRESENTES //////
int checkLetters(char *word, char *control ){
    int i;
    char *P,S[11];

    strcpy(S,control);
    for (i=0;i< strlen(word);i++){
        P=strchr(S,word[i]);
        if (P==NULL) {
            return 0;
        }
        else{
            *P=' ';
        }
    }
    return 1;
}


////// FONCTION DE CONTROLE DE SAISI //////
int Input(struct player *player,char *control){
    char *S;

    printf("Answer %s : ", player->name);
    scanf("%s",player->word);

    S= strchr(player->word,'\n');
    if(S!=NULL) *S='\0';
    if(!checkLetters(player->word,control)){
        printf("Wrong word !\n");
        return Input(player,control);
    }
    return 0;
}

////// FONCTION POUR COMPTER LE TEMPS //////
int timer() {
    int i;
    for (i = delay; i >= 0; --i) {
        printf("\rTime remaining: %d seconds...", i);
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
    char buffer[12];
    
    
    while(fread (&ind_char, sizeof(struct index_char), 1, index)){
	    if (*word==ind_char.firstChar){
            fseek(dict, ind_char.position, SEEK_SET);
            break;
        }
    }
    while (fgets(buffer, sizeof(buffer), dict)){
        if (*word==*buffer){
			buffer[strchr(buffer, '\n')-buffer]='\0';
	        if (!strcmp(word, buffer)){
	        	fclose(dict);
	        	fclose(index);
	            return 1;
			}	
		}
            
    }
    fclose(dict);
    fclose(index);
    return 0;
}

//////  FONCTION POUR CALCULER LE SCORE //////
void Score(struct player *player1,struct player *player2){
    int longWord1,longWord2,FindWord1,FindWord2;

    FindWord1= FindWord(player1->word);     //VERIFIER L'EXISTANCE DE WORD1
    FindWord2= FindWord(player2->word);     //VERIFIER L'EXISTANCE DE WORD2

    longWord1=strlen(player1->word);    //LA LONGUEUR DU 1ER MOT
    longWord2=strlen(player2->word);    //LA LONGUEUR DU 2EME MOT
    if(longWord1>longWord2)                 //MOT JOUEUR 2 PLUS LONGUE
        if (FindWord1){                     //JOUEUR 1 GAGNE LA MANCHE
            printf("/\\/\\/ This round's winner is %s with %d points \\/\\/\\",player1->name,longWord1);
            player1->score += longWord1;
        }
        else{                               //JOUEUR 2 GAGNE LA MANCHE
            if (FindWord2) {
                printf("/\\/\\/ This round's winner is %s with %d points \\/\\/\\",player2->name,longWord1);
                player2->score += longWord1;
            }
        }
    else if(longWord1 < longWord2)            //MOTS JOUEUR 2 PLUS LONGUE
        if (FindWord2) {                      //JOUEUR 2 GAGNE LA MANCHE
            printf("/\\/\\/ This round's winner is %s with %d points \\/\\/\\",player2->name,longWord2);
            player2->score += longWord2;
        }
        else{                               //JOUEUR 1 GAGNE LA MANCHE
            if (FindWord1) {
                printf("/\\/\\/ This round's winner is %s with %d points \\/\\/\\",player1->name,longWord2);
                player1->score += longWord2;
            }
        }
    else                                    //EGALITE DES MOTS
    {
        if (FindWord1 && FindWord2) {       //EGALITE SCORE DE LA MANCHE
            printf("/\\/\\/ This round, it's a draw with %d points for each player \\/\\/\\",longWord1);
            player1->score += longWord1;
            player2->score+= longWord1;
        }
        else if (FindWord1 && !FindWord2) { // JOUEUR 1 GAGNE LA MANCHE
            printf("/\\/\\/ This round's winner is %s with %d points \\/\\/\\",player1->name,longWord1);
            player1->score += longWord1;
        }
        else if (!FindWord1 && FindWord2) {   // JOUEUR 2 GAGNE LA MANCHE
            printf("/\\/\\/ This round's winner is %s with %d points \\/\\/\\",player2->name,longWord2);
            player2->score += longWord2;
        }
        else
            printf("/\\/\\/ This round, it's a draw with 0 point for each player  \\/\\/\\");
            
    }
    printf("\n\n");
}

void centerWord(char *word, char *result) {
    int wordLength;

    int i;
    strcpy(result, "     ");
    strcat(result,word);
    wordLength = strlen(result);
    for ( i = wordLength; i < 15; i++) {
        *(result+i) = ' ';
    }

    result[i] = '\0';
}

////// AFFICHER SCORE APRES LA MANCHE i //////
void ShowScore(struct player *player1,struct player *player2){
    char name1[16],name2[16];

    centerWord(player1->name,name1);
    centerWord(player2->name,name2);

    printf("\n        +----------------------------------------------------+\n");
    printf("        | Name   |\t %s  %s    |\n",name1,name2);
    printf("        | Score  |\t\t%d\t\t  %d\t     |\n",player1->score,player2->score);
    printf("        +----------------------------------------------------+\n\n\n");
}



int Solver(char *control,char *solution){
    FILE *Dict = fopen(path_cleanDict, "r");
    FILE *index = fopen(path_index, "rb");
    struct index_char ind_char;
    char buffer[12], buffer_max[12];
    int max = 0;

    if (Dict == NULL || index == NULL) {
        printf("Error opening files.\n");
        return 0;
    }

    while (fread(&ind_char, sizeof(struct index_char), 1, index)) {
        if (strchr(control, ind_char.firstChar) != NULL) {
            fseek(Dict, ind_char.position, SEEK_SET);
            while (fgets(buffer, sizeof(buffer), Dict) && ind_char.firstChar == *buffer && max < 10) {
                buffer[strchr(buffer, '\n')-buffer] = '\0';
                if (checkLetters(buffer, control) && strlen(buffer) > max) {
                    max = strlen(buffer);
                    strcpy(buffer_max, buffer);
                }
            }
            if (max == 10) {
                strcpy(solution, buffer_max);
                fclose(Dict);
                fclose(index);
                return 1;
            }
        }
    }
    fclose(Dict);
    fclose(index);
    if(max){
        strcpy(solution, buffer_max);
        return 1;
    }
    else {
        return 0;
    }

}


////// FONCTION POUR LANCER UNE MANCHE //////
void Manche(struct player *player1,struct player *player2,int round){
    int nbVowel=0,found;                                              //NBRE DE VOYELLES CHOISI PAR L'UTILISATEUR
    char control[11],solution[11];                                    //LA PLUS LONGUE MOT DE DICTIONNAIRE A 15 LETTRES



    printf("---------- ROUND %d ----------\n",round+1);
    do{
        printf("<> %s chooses a number of vowels : ", round%2 ? player2->name : player1->name );  //SAISI DU NBRE DE VOYELLES
        scanf("%d",&nbVowel);
    }while(nbVowel>10 || nbVowel<2);

    do{
        Generate10Char(nbVowel,control);                    //GENERATION DES 10 LETTRES ALEATOIREMENT
        found=Solver(control,solution);                              //SOLVER LE MOT
    }while(!found);
    printf("Generated Chars : %s\n", control);

    timer();

    Input(round%2 ? player2 : player1 ,control);              //SAISI DES 10 LETTRES DE JOUEUR 1
    Input(round%2 ? player1 : player2 ,control);              //SAISI DES 10 LETTRES DE JOUEUR 2


    printf("\n\n==> Example of long word using these letters: %s\n\n", solution);

    Score(player1,player2);                                         //METTRE A JOUR LE SCORE
    ShowScore(player1,player2);

    sleep(8);
    printf("\033[2J\033[1;1H");
}

////// GAME OVER //////
void GameOver(struct player player1,struct player player2){
     int i;
    const char *message = "      _____          __  __ ______    ______      ________ _____  \n"
                          "     / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ \n"
                          "    | |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |\n"
                          "    | | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  / \n"
                          "    | |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\ \n"
                          "     \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\\n"
                          "\n               Copyright 2024 © KTM | All Rights Reserved .\n"
                          "      Karimen Ben Romdhane | Tasnim Meriah | Mohamed Hedi Maaroufi \n\n";
    const int delay_ms = 15 * 1000;

    for ( i = 0; message[i] != '\0'; i++) {
        putchar(message[i]);
        fflush(stdout);
        usleep(delay_ms);
    }

    putchar('\n');
    sleep(1);
    ShowScore(&player1,&player2);

    player1.score-player2.score ? printf("\t\t/\\/\\/\\ The winner is %s /\\/\\/\\\n",player1.score>player2.score ? player1.name : player2.name ) : printf("\t\t      /\\/\\/\\ DRAW /\\/\\/\\\n");

    sleep(5);
    exit(0);
}




int main(){
    struct player player1={1,0,}; //STRUCTURE POUR LE JOUEUR 1
    struct player player2={2,0,}; //STRUCTURE POUR LE JOUEUR 2
    int i;

    system("cls");

    srand(time(NULL));

    Hello();

    CleanDict();
    CreateIndex(path_cleanDict);           //CREATION DU FICHIER INDEX
    //VerifyIndex(path_cleanDict);           //VERIFICATION DU FICHIER INDEX

    EnterName(&player1);
    EnterName(&player2);

    system("cls");

    for ( i = 0; i < 8; ++i )
        Manche(&player1,&player2,i);      //LANCEMANT DE MANCHE i

    system("cls");

    GameOver(player1,player2);
    return 0;
}


