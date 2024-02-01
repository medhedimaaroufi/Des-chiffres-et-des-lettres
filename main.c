#include <stdio.h>
#include<string.h>

typedef struct index_char
{
    char firstChar;
    long int position;
};

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

int main() {
/*     CreateIndex("dictionnaire.txt");
    VerifyIndex("dictionnaire.txt", "index.txt"); 
    char ch[30]="zabres";
    printf("result = %d\n",FindWord("dictionnaire.txt", "index.txt", ch));*/

    

    return 0;
}

