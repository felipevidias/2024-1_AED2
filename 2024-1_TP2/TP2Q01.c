#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALTERNATIVE_NAMES 10
#define MAX_ALTERNATIVE_ACTORS 10
#define MAX_LINE_LENGTH 1024

// Struct para armazenar uma lista de strings
typedef struct {
    char *elementos[MAX_ALTERNATIVE_NAMES];
    int tamanho;
} Lista;

// Struct para representar um personagem
typedef struct {
    char id[50];
    char name[100];
    Lista alternativeNames;
    char house[50];
    char ancestry[50];
    char species[50];
    char patronus[50];
    char hogwartsStaff[5];
    char hogwartsStudent[5]; 
    char actorName[100];
    Lista alternativeActors;
    char alive[5]; 
    char dateOfBirth[11];
    char yearOfBirth[10];
    char eyeColour[20];
    char gender[20];
    char hairColour[20];
    char wizard[5]; 
} Personagem;
 
// Função para inicializar uma lista
void inicializarLista(Lista *lista)
{
    lista->tamanho = 0;
} // end inicalizarLista

// Função para adicionar um elemento à lista
void adicionarElemento(Lista *lista, char *elemento) 
{
    if (lista->tamanho < MAX_ALTERNATIVE_NAMES) 
    {
        lista->elementos[lista->tamanho] = strdup(elemento);
        lista->tamanho++;
    } else {
        printf("Limite de elementos na lista excedido.\n");
    } // end if 
} // end adicionarElemento

// Função para liberar a memória alocada para os elementos da lista
void liberarLista(Lista *lista)
{
    for (int i = 0; i < lista->tamanho; i++) 
    {
        free(lista->elementos[i]);
    } // end for 
    lista->tamanho = 0;
} // end liberarLista

void ler(Personagem *personagem, char *line) 
{
    char *token;
    token = strtok(line, ";");

    if (token == NULL) return;

    strcpy(personagem->id, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->name, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    inicializarLista(&(personagem->alternativeNames));
    char *alternateNames = strtok(token, ",");
    while (alternateNames != NULL) {
        // Removendo os espaços e os caracteres '[' ']' e as aspas simples dos nomes alternativos
        while (*alternateNames == ' ' || *alternateNames == '[' || *alternateNames == '\'') // Ignora espaços e aspas simples no início do nome
            alternateNames++;
        int len = strlen(alternateNames);
        while (len > 0 && (alternateNames[len - 1] == ']' || alternateNames[len - 1] == '\'')) // Remove aspas simples no final do nome
            alternateNames[--len] = '\0';
        adicionarElemento(&(personagem->alternativeNames), alternateNames);
        alternateNames = strtok(NULL, ",");
    } // while

    // ERRO ROLANDO DAQUI PRA BAIXO
    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->house, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->ancestry, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->species, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->patronus, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->hogwartsStaff, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->hogwartsStudent, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->actorName, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->alive, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->dateOfBirth, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->yearOfBirth, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->eyeColour, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->gender, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->hairColour, token);

    token = strtok(NULL, ";");
    if (token == NULL) return;
    strcpy(personagem->wizard, token);
} // end ler 




// Função para imprimir os dados de um personagem
void imprimir(Personagem *personagem) 
{
    printf("[%s ## %s ## {", personagem->id, personagem->name);
    for (int i = 0; i < personagem->alternativeNames.tamanho - 1; i++)
    {
        printf("%s, ", personagem->alternativeNames.elementos[i]);
    }
    if (personagem->alternativeNames.tamanho > 0) 
    {
        printf("%s", personagem->alternativeNames.elementos[personagem->alternativeNames.tamanho - 1]);
    }
    printf("} ## %s ## %s ## %s ## %s ## false ## false ## %s ## false ## %s ## %s ## %s ## %s ## %s ## false]\n",
           personagem->house, personagem->ancestry, personagem->species, personagem->patronus,
           personagem->actorName, personagem->dateOfBirth, personagem->yearOfBirth,
           personagem->eyeColour, personagem->gender, personagem->hairColour);
}

int main() {
    FILE *file = fopen("characters.csv", "r");
    if (file == NULL) 
    {
        printf("NAO TEM ARQUIVO\n");
        return 1;
    } // end if

    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file); // Lê e descarta a primeira linha (cabeçalho)

    Personagem personagens[1000]; // Supondo que haverá no máximo 1000 personagens
    int num_personagens = 0;

    while (fgets(line, sizeof(line), file)) 
    {
        ler(&personagens[num_personagens], line);
        num_personagens++;
    } // end while

    fclose(file);

    char idBusca[50];
    while (1) 
    {
        scanf("%s", idBusca);
        if (strcmp(idBusca, "FIM") == 0)
            break; // end if 
        int encontrado = 0;
        for (int i = 0; i < num_personagens; i++) 
        {
            if (strcmp(personagens[i].id, idBusca) == 0) 
            {
                imprimir(&personagens[i]);
                encontrado = 1;
                break;
            } // end if 
        } // end for 
        if (!encontrado) 
        {
            printf("Personagem não encontrado.\n");
        } // end if
    } // end while

    // Liberar memória alocada para elementos das listas
    for (int i = 0; i < num_personagens; i++) 
    {
        liberarLista(&(personagens[i].alternativeNames));
    } // end for 

    return 0;
} // end main
