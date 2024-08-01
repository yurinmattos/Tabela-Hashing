#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 53 //Valor primo

typedef struct aluno
{
    int mat;
    char nome[100];
    char curso[50];
    int disp;
} Aluno;

int hash(int key, int size)
{
    return key % size; // retorna resto da divisao do valor por TAM
}

void inicializar(char *nomeArquivo)
{
    FILE *arq = fopen(nomeArquivo, "wb");
    if (!arq)
    {
        printf("Arquivo nao encontrado.");
    }
    Aluno aluno;
    int i;
    aluno.disp = 1;
    for (i = 0; i < TAM; i++)
    {
        fwrite(&aluno, sizeof(Aluno), 1, arq);
    }
    fclose(arq);
}

int acharPosicao(FILE* arq, int mat)
{
    int pos = hash(mat, TAM);
    Aluno a;

    fseek(arq, pos * sizeof(Aluno), SEEK_SET);
    fread(&a, sizeof(Aluno), 1, arq);

    while (a.disp == 0 && a.mat != mat)
    {
        pos = (pos + 1) % TAM;
        fseek(arq, pos * sizeof(Aluno), SEEK_SET);
        fread(&a, sizeof(Aluno), 1, arq);
    }
    return pos;
}

void inserir(char *nomeArquivo)
{
    FILE *arq = fopen(nomeArquivo, "r+b");
    if (!arq)
    {
        printf("Arquivo nao encontrado.");
    }

    Aluno a;

    printf("Matricula: ");
    scanf("%d", &a.mat);

    getchar();

    printf("Nome: ");
    fgets(a.nome, sizeof(a.nome), stdin);
    a.nome[strcspn(a.nome, "\n")] = '\0'; //Substitui o \n por \0

    printf("Curso: ");
    fgets(a.curso, sizeof(a.curso), stdin);
    a.curso[strcspn(a.curso, "\n")] = '\0'; //Substitui o \n por \0

    int pos = acharPosicao(arq, a.mat);

    a.disp = 0;

    fseek(arq, pos * sizeof(Aluno), SEEK_SET);
    fwrite(&a, sizeof(Aluno), 1, arq);
    fclose(arq);
}

void imprimeAluno(char *nomeArquivo, int x)
{
    FILE *arq = fopen(nomeArquivo, "rb");
    if (!arq)
    {
        printf("Arquivo nao encontrado.");
    }

    Aluno a;
    int pos = acharPosicao(arq, x);

    fseek(arq, pos * sizeof(Aluno), SEEK_SET);
    fread(&a, sizeof(Aluno), 1, arq);

    if (a.disp == 0)
    {
        printf("\nMatricula: %d\nNome: %s\nCurso: %s\n", a.mat, a.nome, a.curso);
    }
    else
    {
        printf("\nAluno não encontrado.\n");
    }
    fclose(arq);
}

void imprimirHash(char *nomeArquivo)
{
    FILE *arq = fopen(nomeArquivo, "rb");
    if (!arq)
    {
        printf("Arquivo nao encontrado.");
    }
    Aluno a;
    for (int i = 0; i < TAM; i++)
    {
        fseek(arq, i * sizeof(Aluno), SEEK_SET);
        fread(&a, sizeof(Aluno), 1, arq);
        char estado[15];

        if (a.disp == 1)
        {
            strcpy(estado, "Disponivel");
        }
        else
        {
            strcpy(estado, "Indisponivel");
        }

        printf("M:%d\tN:%s\tC:%s\tD:%s\n", a.mat, a.nome, a.curso, estado);
    }
    fclose(arq);
}

int main()
{
    int aux = 0;
    inicializar("hashing.txt");

    while (aux != 4)
    {
        printf("1-Inserir um novo aluno\n2-Imprimir informacoes do aluno\n3-Imprimir tabela hash\n4-Sair");
        scanf("%d", &aux);

        switch (aux)
        {
        case 1:
            inserir("hashing.txt");
            break;
        case 2:
            int x;
            scanf("%d", &x);
            imprimeAluno("hashing.txt", x);
            break;
        case 3:
            imprimirHash("hashing.txt");
        default:
            break;
        }
    }

    return 0;
}