#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

struct disciplina {
  int codigo;
  char *nome;
  float nota_final;
};

struct No {
    int matricula;
    char *nome;
    int cpf;
    int quant_disciplinas;

    struct disciplina *ptr_disc;
    struct No *esq;
    struct No *dir;
};

typedef struct No No;
typedef struct disciplina disciplina;

No *RAIZ = NULL;

No *buscar(int valor, No* percorredor) {
    if (valor == percorredor->matricula)
        return percorredor;

    else if (valor < percorredor->matricula) {

        if (percorredor->esq == NULL)
            return percorredor;

        buscar(valor, percorredor->esq);

    } else if (valor > percorredor->matricula) {

        if (percorredor->dir == NULL)
            return percorredor;

        buscar(valor, percorredor->dir);

    } else
        return NULL;

}

void adicionar(int valor, No *RAIZ_ADD, char *nome, int cpf, int quant_disciplinas, disciplina *disciplinas) {

    No *NOVO = malloc(sizeof(No));

    NOVO->matricula = valor;
    NOVO->nome = nome;
    NOVO->cpf = cpf;
    NOVO->quant_disciplinas = quant_disciplinas;
    NOVO->ptr_disc = disciplinas;

    NOVO->esq = NULL;
    NOVO->dir = NULL;


    if (RAIZ == NULL)
        RAIZ = NOVO;

    else {
        No *RESULTADO_DA_BUSCA = buscar(valor, RAIZ_ADD);

        if (RESULTADO_DA_BUSCA == NULL || RESULTADO_DA_BUSCA->matricula == valor)
            printf("\nMatricula duplicada!\n");

        else {
            if (valor > RESULTADO_DA_BUSCA->matricula)
                RESULTADO_DA_BUSCA->dir = NOVO;

            else if (valor < RESULTADO_DA_BUSCA->matricula)
                RESULTADO_DA_BUSCA->esq = NOVO;
        }

    }
}

void in_order(No* percorredor) {

    if (percorredor == NULL)
        printf("\nUau, que vazio!\n");

    else {

        if (percorredor->esq != NULL)
            in_order(percorredor->esq);

        printf("\nAluno: %s (MATRICULA %d) | CPF : %d\n", percorredor->nome, percorredor->matricula, percorredor->cpf);
        printf("DISCIPLINAS: ");

        for (int i = 0; i < percorredor->quant_disciplinas; i++)
            printf("%s (Nota Final: %.2f)| ", percorredor->ptr_disc[i].nome, percorredor->ptr_disc[i].nota_final);

        printf("\n");

        if (percorredor->dir != NULL)
            in_order(percorredor->dir);
    }

}
No *menor_dos_maiores(No *percorredor) {
    if (percorredor->esq != NULL)
        return menor_dos_maiores(percorredor->esq);
    return percorredor;
}

No * remover(int valor, No *percorredor) {

    if (percorredor == NULL) {
        printf("\nSem resultados!\n");

        return NULL;
    }

    else {
        if (percorredor->matricula == valor) {

            if (percorredor->dir == NULL && percorredor->esq == NULL) {

                // FOLHA
                if (percorredor == RAIZ) {
                    RAIZ = NULL;
                }

                printf("\nO aluno %s (%d) foi removido!\n", percorredor->nome, percorredor->matricula);

                free(percorredor);
                return NULL;


            } else if (percorredor->dir != NULL && percorredor->esq != NULL) {

                // DOIS FILHOS
                No *backup = malloc(sizeof(No));
                backup->cpf = percorredor->cpf;
                backup->matricula = percorredor->matricula;
                backup->nome = percorredor->nome;
                backup->ptr_disc->codigo = percorredor->ptr_disc->codigo;
                backup->ptr_disc->nome = percorredor->ptr_disc->nome;
                backup->ptr_disc->nota_final = percorredor->ptr_disc->nota_final;


                No *elemento_menor_dos_maiores = menor_dos_maiores(percorredor->dir);

                percorredor->matricula = elemento_menor_dos_maiores->matricula;
                percorredor->cpf = elemento_menor_dos_maiores->cpf;
                percorredor->nome = elemento_menor_dos_maiores->nome;
                percorredor->ptr_disc->codigo = elemento_menor_dos_maiores->ptr_disc->codigo;
                percorredor->ptr_disc->nome = elemento_menor_dos_maiores->ptr_disc->nome;
                percorredor->ptr_disc->nota_final = elemento_menor_dos_maiores->ptr_disc->nota_final;

                elemento_menor_dos_maiores->matricula = valor;
                elemento_menor_dos_maiores->cpf = backup->cpf;
                elemento_menor_dos_maiores->nome = backup->nome;
                elemento_menor_dos_maiores->ptr_disc->codigo = backup->ptr_disc->codigo;
                elemento_menor_dos_maiores->ptr_disc->nome = backup->ptr_disc->nome;
                elemento_menor_dos_maiores->ptr_disc->nota_final = backup->ptr_disc->nota_final;


                free(backup);

                percorredor->dir = remover(valor, percorredor->dir);


            }
            else {

                // UM FILHO

                No *filho = (percorredor->dir == NULL)? percorredor->esq : percorredor->dir;
                 if (percorredor == RAIZ)
                    RAIZ = filho;

                printf("O aluno %s (%d) foi removido!", percorredor->nome, percorredor->matricula);

                free(percorredor);

                return filho;
            }


        }
        else if (valor > percorredor->matricula) {
            percorredor->dir = remover(valor, percorredor->dir);
            return percorredor;
        }

        else if (valor < percorredor->matricula) {
            percorredor->esq = remover(valor, percorredor->esq);
            return percorredor;
        }

    }
}

No *alterar(int matricula, No *aux){

    if (aux == NULL) {
        return NULL;
    }
    int escolha, cpf;
    char nome[255];
    char *addNome;

    No *alterador = buscar(matricula,aux);

    if(alterador->matricula != matricula){
        printf("\nSem registros para %d\n", matricula);
        return aux;
    }

    printf("\nO que deseja alterar?\n1 - Nome\n2 - CPF\n");
    scanf("%d", &escolha);

    switch(escolha){
        case 1:
            printf("\nDigite um novo nome: ");
            scanf("%s", &nome);

            addNome = malloc(sizeof(char) * strlen(nome));

                for (int i = 0; i < strlen(nome); i++)
                    addNome[i] = nome[i];

            free(alterador->nome);
            alterador->nome = addNome;
            break;

        case 2:
            printf("Digite um novo CPF: ");
            scanf("%d", &cpf);
            alterador->cpf = cpf;
            break;

        default:
            printf("Invalido\n");

    }

    return aux;

}


int particionarVetor (disciplina *vetor, int posicaoInicio, int posicaoFinal) {
    int esquerda, direita;

    disciplina *pivo = malloc(sizeof(disciplina));
    disciplina *aux_troca = malloc(sizeof(disciplina));

    esquerda = posicaoInicio;
    direita = posicaoFinal;

    pivo->nota_final = vetor[posicaoInicio].nota_final;
    pivo->nome = vetor[posicaoInicio].nome;
    pivo->codigo = vetor[posicaoInicio].codigo;


    while (esquerda < direita) {

        while (esquerda <= posicaoFinal && vetor[esquerda].nota_final <= pivo->nota_final) {
            esquerda++;
        }

        while (direita >= 0 && vetor[direita].nota_final > pivo->nota_final) {
            direita--;
        }

        if (esquerda < direita) {
            aux_troca->nota_final = vetor[esquerda].nota_final;
            aux_troca->nome = vetor[esquerda].nome;
            aux_troca->codigo = vetor[esquerda].codigo;

            vetor[esquerda].nota_final = vetor[direita].nota_final;
            vetor[esquerda].nome = vetor[direita].nome;
            vetor[esquerda].codigo = vetor[direita].codigo;

            vetor[direita].nota_final = aux_troca->nota_final;
            vetor[direita].nome = aux_troca->nome;
            vetor[direita].codigo = aux_troca->codigo;


        }
    }
    vetor[posicaoInicio].nota_final = vetor[direita].nota_final;
    vetor[posicaoInicio].nome = vetor[direita].nome;
    vetor[posicaoInicio].codigo = vetor[direita].codigo;

    vetor[direita].nota_final = pivo->nota_final;
    vetor[direita].nome = pivo->nome;
    vetor[direita].codigo = pivo->codigo;

    free(pivo);
    free(aux_troca);
    return direita;
}

void organizadorQuickSort(disciplina *vetor, int inicio, int fim) {

    disciplina *pivo = malloc(sizeof(disciplina));

    if (fim > inicio) {
        pivo->nota_final = particionarVetor(vetor, inicio, fim);

        organizadorQuickSort(vetor, inicio, pivo->nota_final-1);
        organizadorQuickSort(vetor, pivo->nota_final+1, fim);
    }

    free(pivo);

}

int main() {
    setlocale(LC_ALL, "");
    int entrada = 1;

    char nome[255];
    char *addNome;

    char nome_disciplina[254];
    char *ptr_nome_disciplina;
    int cod_disciplina;
    float nota_final;

    int matricula;
    int cpf;
    int quant_disciplinas;

    do {
        printf("\n0 - Sair\n1 - Adicionar\n2 - Listar todos\n3 - Remover aluno\n4 - Alterar\n5 - Buscar aluno\n");
        scanf("%d", &entrada);

        switch(entrada) {
            case 0:
                printf("\nSaindo...\n");
                break;
            case 1:
                printf("\nDigite o nome: \n");
                scanf("%s", &nome);

                addNome = malloc(sizeof(char) * strlen(nome));

                for (int i = 0; i < strlen(nome); i++)
                    addNome[i] = nome[i];

                printf("\nDigite o CPF: \n");
                scanf("%d", &cpf);

                printf("Digite a matricula: \n");
                scanf("%d", &matricula);

                printf("Digite quantas disciplinas o aluno ira cursar: \n");
                scanf("%d", &quant_disciplinas);

                disciplina *minhas_disciplinas = malloc(sizeof(disciplina) * quant_disciplinas);

                for (int i = 0; i < quant_disciplinas; i++) {
                    printf("Digite o nome da disciplina de num. %d: ", i+1);
                    scanf("%s", &nome_disciplina);

                    ptr_nome_disciplina = malloc(sizeof(char) * strlen(nome_disciplina));
                    int num = strlen(nome_disciplina);

                    printf("Digite o codigo da disciplina de %s: ", nome_disciplina);
                    scanf("%d", &cod_disciplina);

                    printf("Digite o valor da Nota Final da disciplina de %s: ", nome_disciplina);
                    scanf("%f", &nota_final);


                    for (int j = 0; j < num; j++)
                        ptr_nome_disciplina[j] = nome_disciplina[j];

                    minhas_disciplinas[i].nome = ptr_nome_disciplina;

                    minhas_disciplinas[i].codigo = cod_disciplina;
                    minhas_disciplinas[i].nota_final = nota_final;

                }

                adicionar(matricula, RAIZ, addNome, cpf, quant_disciplinas, minhas_disciplinas);

                break;
            case 2:
                in_order(RAIZ);
                break;
            case 3:
                in_order(RAIZ);

                printf("Digite a matricula do aluno p/ excluir: ");
                scanf("%d", &matricula);

                remover(matricula, RAIZ);

                break;
            case 4:
                in_order(RAIZ);

                printf("Digite a matricula do aluno p/ alterar: \n");
                scanf("%d", &matricula);

                alterar(matricula, RAIZ);

                break;
            case 5:

                if (RAIZ == NULL)
                    printf("\nSem registros!\n");
                else {
                    printf("\nDigite a matricula do aluno p/ buscar: \n");
                    scanf("%d", &matricula);

                    No *RESULTADO_DA_BUSCA = buscar(matricula, RAIZ);

                    if (RESULTADO_DA_BUSCA->matricula == matricula) {

                        printf("Aluno encontrado:\n%s (%d)\n", RESULTADO_DA_BUSCA->nome, RESULTADO_DA_BUSCA->matricula);

                        organizadorQuickSort(RESULTADO_DA_BUSCA->ptr_disc, 0, RESULTADO_DA_BUSCA->quant_disciplinas-1);

                        printf("Disciplinas (ordenadas): ");

                        for (int i = RESULTADO_DA_BUSCA->quant_disciplinas-1; i >= 0; i--)
                            printf("%s (Nota Final: %.2f)| ", RESULTADO_DA_BUSCA->ptr_disc[i].nome, RESULTADO_DA_BUSCA->ptr_disc[i].nota_final);


                    } else {
                        printf("\nSem registros!\n");
                    }



                }
                break;
            default:
                printf("\nOpcao Invalida!\n");

                break;
        }

    } while (entrada != 0);

    return 0;
}
