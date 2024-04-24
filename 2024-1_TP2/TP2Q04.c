#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALTERNATIVE_NAMES 10
#define MAX_ALTERNATIVE_ACTORS 10
#define MAX_LINE_LENGTH 1024
#define MAX_NAME_LENGTH 100
#define MAX_TOKENS 20
#define MAX_PERSONAGENS 1000

typedef struct {
    char elementos[MAX_ALTERNATIVE_NAMES][MAX_NAME_LENGTH];
    int tamanho;
} Lista;

typedef struct {
    char id[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    Lista alternativeNames;
    char house[MAX_NAME_LENGTH];
    char ancestry[MAX_NAME_LENGTH];
    char species[MAX_NAME_LENGTH];
    char patronus[MAX_NAME_LENGTH];
    int hogwartsStaff;
    int hogwartsStudent;
    char actorName[MAX_NAME_LENGTH];
    Lista alternativeActors;
    int alive;
    char dateOfBirth[11];
    char yearOfBirth[50];
    char eyeColour[50];
    char gender[50];
    char hairColour[50]; 
    int wizard;
} Personagem;

void inicializarLista(Lista *lista) {
    lista->tamanho = 0;
}

void adicionarElemento(Lista *lista, const char *elemento) {
    if (lista->tamanho < MAX_ALTERNATIVE_NAMES) {
        strncpy(lista->elementos[lista->tamanho], elemento, MAX_NAME_LENGTH - 1);
        lista->elementos[lista->tamanho][MAX_NAME_LENGTH - 1] = '\0';
        lista->tamanho++;
    }
}

void liberarLista(Lista *lista) {
    lista->tamanho = 0;
}

int split(char *str, char delim, char *tokens[]) {
    int count = 0;
    char *token = strtok(str, &delim);

    while (token != NULL && count < MAX_TOKENS) {
        tokens[count++] = token;
        token = strtok(NULL, &delim);
    }

    return count;
}

void formatarData(char *data) {
    int dia, mes, ano;
    sscanf(data, "%d-%d-%d", &dia, &mes, &ano);
    sprintf(data, "%02d-%02d-%d", dia, mes, ano);
}

void ler(Personagem *personagem, char *line) {
    char *tokens[MAX_TOKENS];
    int num_tokens = split(line, ';', tokens);

    strcpy(personagem->id, tokens[0]);
    strcpy(personagem->name, tokens[1]);

    inicializarLista(&(personagem->alternativeNames));
    int i = 0;
    char *alternativeNames_token = strtok(tokens[2], ",");
    while (alternativeNames_token != NULL && i < MAX_ALTERNATIVE_NAMES) {
        adicionarElemento(&(personagem->alternativeNames), alternativeNames_token);
        alternativeNames_token = strtok(NULL, ",");
        i++;
    }

    strcpy(personagem->house, tokens[3]);
    strcpy(personagem->ancestry, tokens[4]);
    strcpy(personagem->species, tokens[5]);
    strcpy(personagem->patronus, tokens[6]);
    personagem->hogwartsStaff = strcmp(tokens[7], "Sim") == 0;
    personagem->hogwartsStudent = strcmp(tokens[8], "Sim") == 0;
    strcpy(personagem->actorName, tokens[9]);

    inicializarLista(&(personagem->alternativeActors));
    i = 0;
    char *alternativeActors_token = strtok(tokens[10], ",");
    while (alternativeActors_token != NULL && i < MAX_ALTERNATIVE_ACTORS) {
        adicionarElemento(&(personagem->alternativeActors), alternativeActors_token);
        alternativeActors_token = strtok(NULL, ",");
        i++;
    }
    
    personagem->alive = strcmp(tokens[11], "Sim") == 0;
    strcpy(personagem->dateOfBirth, tokens[12]);
    formatarData(personagem->dateOfBirth);
    strcpy(personagem->yearOfBirth, tokens[13]);
    strcpy(personagem->eyeColour, tokens[14]);
    strcpy(personagem->gender, tokens[15]);
    strcpy(personagem->hairColour, tokens[16]);
    personagem->wizard = strcmp(tokens[17], "Sim") == 0;
}

void imprimir(Personagem *personagem) {
    printf("[%s ## %s ## {", personagem->id, personagem->name);
    for (int i = 0; i < personagem->alternativeNames.tamanho; i++) {
        printf("%s,", personagem->alternativeNames.elementos[i]);
    }
    printf("} ## ");
    printf("%s ## ", personagem->house);
    printf("%s ## ", personagem->ancestry);
    printf("%s ## ", personagem->species);
    printf("%s ## ", personagem->patronus);
    printf("%s ## ", personagem->hogwartsStaff ? "Sim" : "Nao");
    printf("%s ## ", personagem->hogwartsStudent ? "Sim" : "Nao");
    printf("%s ## ", personagem->actorName);
    printf("%s ## ", personagem->alive ? "Sim" : "Nao");
    printf("%s ## ", personagem->dateOfBirth);
    printf("%s ## ", personagem->yearOfBirth);
    printf("%s ## ", personagem->eyeColour);
    printf("%s ## ", personagem->gender);
    printf("%s ## ", personagem->hairColour);
    printf("%s]\n", personagem->wizard ? "Sim" : "Nao");
}

void selectionSort(char nomes[MAX_PERSONAGENS][MAX_NAME_LENGTH], int num_nomes)
{
    for(int i = 0; i < num_nomes - 1; i++)
    {
        int menor = i;
        for(int j = i + 1; j < num_nomes; j++)
        {
            if(strcmp(nomes[j], nomes[menor]) < 0)
            {
                menor = j;
            }
        }
        // SWAP
        if(menor != i)
        {
        char tmp[MAX_NAME_LENGTH];
        strcpy(tmp, nomes[i]);
        strcpy(nomes[i], nomes[menor]);
        strcpy(nomes[menor], tmp);  
        }
    }
}

int binarySearch(char nome[], char nomes[MAX_PERSONAGENS][MAX_NAME_LENGTH], int num_nomes) {
    int esq = 0;
    int dir = num_nomes - 1;

    while (esq <= dir) {
        int meio = (esq + dir) / 2;
        int comparacao = strcmp(nome, nomes[meio]);
        if (comparacao == 0) {
            return meio;
        } else if (comparacao < 0) {
            dir = meio - 1;
        } else {
            esq = meio + 1;
        }
    }
    return -1;
}

int main() {
    FILE *file = fopen("characters.csv", "r");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file); // Ignorar cabeçalho

    Personagem personagens[MAX_PERSONAGENS];
    int num_personagens = 0;

    while (fgets(line, sizeof(line), file)) {
        ler(&personagens[num_personagens], line);
        num_personagens++;
        if (num_personagens >= MAX_PERSONAGENS) {
            printf("Número máximo de personagens atingido.\n");
            break;
        }
    }

    for(int i = 0; i < num_personagens; i++)
    {
        printf("Nome lido: %s\n", personagens[i].name);
    }

    fclose(file);

    // PEGAR VETOR DE NOMES 
    char nomes[MAX_PERSONAGENS][MAX_NAME_LENGTH];
    int num_nomes = 0;
    char id[MAX_NAME_LENGTH];
    while (1)
    {
        scanf("%s", id);
        if (strcmp(id, "FIM") == 0)
            break;
        for (int i = 0; i < num_personagens; i++)
        {
            if (strcmp(personagens[i].id, id) == 0)
            {
                strcpy(nomes[num_nomes], personagens[i].name);
                num_nomes++;
                break;
            }
        }
    }

   
    // Ordenar os nomes antes de realizar a busca binária
    selectionSort(nomes, num_nomes);
    

    char nome[MAX_NAME_LENGTH];
    while (1)
    {
        scanf("%s", nome);
        if (strcmp(nome, "FIM") == 0)
            break;

        int posicao = binarySearch(nome, nomes, num_nomes);

    }
    return 0;
}
