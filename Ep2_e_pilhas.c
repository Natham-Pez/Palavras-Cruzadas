
/***************************************************************/
/**                                                           **/
/**   Natham Sanchez Pez                            13680470  **/
/**   Exercício-Programa 02                                   **/
/**   Professor: Carlinhos                                    **/
/**   Turma: 01                                               **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int posicao_palavra;
  int palavra_disponivel;
} item;

typedef struct
{
  item * v;
  int topo; 
  int max; 
} pilha; 

void empilha (pilha * p, item x); 
item desempilha (pilha * p); 
int pilhaVazia (pilha * p); 
pilha * cria(); 
void destroi (pilha * p);
void resize (pilha * p); 

int pilhaVazia (pilha * p){
  return (p->topo == 0); 
}

item desempilha (pilha * p){
  if (!pilhaVazia(p)){
    p->topo = p->topo - 1;
    return (p->v[p->topo]);
  }
}

pilha * cria (){
  pilha * p = malloc (sizeof(pilha)); 
  p->v = malloc (5 * sizeof(item));
  p->max = 5;
  p->topo = 0;
  return p;
}

void empilha (pilha * p, item x){
  if (p->topo == p->max)
    resize(p);
  p->v[p->topo] = x;
  p->topo = p->topo + 1;
}

void resize(pilha * p){
  int i; 
  item * w = malloc (2 * p->max * sizeof(item));
  for (i = 0; i < p->max; i++) 
    w[i] = p->v[i];
  p->max = 2 * p->max;
  free (p->v); 
  p->v = w;
}

void destroi (pilha * p){
  free (p->v);
  free (p);
}

typedef struct
{
    char palavra[50];
    int tam;
} palavra;

typedef struct
{
    int posicao_m;
    int posicao_n;
    int dir;
} posicoes;

void imprime_Cruzadas(char **palavra_Cruzada, int m, int n);
int pode_Colocar(int **matriz_Tabuleiro, int m, int n, posicoes *guarda, int num_palavras);
void coloca_Palavra(char **palavra_Cruzada, int m, int n, int lin, int col, int dir, palavra *lista_Palavra, int k, int **ocupadas);
void apaga_Palavra(char **palavra_Cruzada, int m, int n, int lin, int col, int dir, int **ocupadas);
int cruzadas(char **palavra_Cruzada, int **matriz_Tabuleiro, int m, int n, int num_palavras, palavra *lista_palavra);
int seleciona_Palavra(int **matriz_Tabuleiro, char **palavra_Cruzada, palavra *lista_Palavra, int m, int n, posicoes guarda, int num_palavras, int *palavra_disponivel, int comeco);
int encaixa(char **palavra_Cruzada, int m, int n, int lin, int col, int dir, palavra *lista_palavra, int k);
int verifica(char **palavra_Cruzada, int lin, int col, palavra *lista_palavra, int k);

int main()
{
    int t;
    int w;
    int i, j, k;
    int m, n;
    int num_palavras;
    int conta;
    palavra *lista_Palavra;
    int **matriz_Tabuleiro;
    char **palavra_Cruzada;

    w = 1;
    while(m != 0 && n != 0){
        scanf("%d %d", &m, &n);
        matriz_Tabuleiro = malloc((m) * sizeof(int *));
        for (i = 0; i < m; i++)
        {
            matriz_Tabuleiro[i] = malloc((n) * sizeof(int));
        }
        /*Scanf do tabuleiro*/
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                scanf("%d", &matriz_Tabuleiro[i][j]);
            }
        }

        scanf("%d", &num_palavras);

        lista_Palavra = malloc((num_palavras) * sizeof(palavra));

        for (conta = 0; conta < num_palavras; conta++)
        {
            k = 0;
            scanf("%s", lista_Palavra[conta].palavra);

            while (lista_Palavra[conta].palavra[k] != '\0')
            {
                k++;
            }
            lista_Palavra[conta].tam = k;
        }
        /*Criar o tabuleiro das palavras cruzadas*/
        palavra_Cruzada = malloc((m) * sizeof(char *));
        for (i = 0; i < m; i++)
        {
            palavra_Cruzada[i] = malloc((n) * sizeof(char));
        }
        /*Preencher o tabuleiro das palavras cruzadas*/
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (matriz_Tabuleiro[i][j] == 0)
                {
                    palavra_Cruzada[i][j] = '0';
                }
                if (matriz_Tabuleiro[i][j] == -1)
                {
                    palavra_Cruzada[i][j] = '*';
                }
            }
        }

        if (cruzadas(palavra_Cruzada, matriz_Tabuleiro, m, n, num_palavras, lista_Palavra) == 0)
        {
            printf("Instancia %d\n", w);
            printf("Nao ha solucao\n");
        }
        else
        {
            printf("\n");
            printf("Instancia %d\n", w);
            imprime_Cruzadas(palavra_Cruzada, m, n);
        }

        for (t = 0; t < m; t++)
            free(matriz_Tabuleiro[t]);
        free(matriz_Tabuleiro);

        for (t = 0; t < m; t++)
            free(palavra_Cruzada[t]);
        free(palavra_Cruzada);

        free(lista_Palavra);

        w++;
    }
    return 0;
}

void imprime_Cruzadas(char **palavra_Cruzada, int m, int n)
{
    int i, j;

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%3c ", palavra_Cruzada[i][j]);
        }
        printf("\n");
    }
}

int pode_Colocar(int **matriz_Tabuleiro, int m, int n, posicoes *guarda, int num_palavras)
{
    int i, j, k;

    k = 0;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (matriz_Tabuleiro[i][j] == 0)
            {
                /*Vê se pode colocar uma palavra na horizontal*/
                if ((j - 1 < 0 || matriz_Tabuleiro[i][j - 1] != 0) && (j + 1 < n && matriz_Tabuleiro[i][j + 1] == 0))
                {
                    guarda[k].posicao_m = i;
                    guarda[k].posicao_n = j;
                    guarda[k].dir = 0;
                    k++;
                }
                /*Vê se pode colocar uma palavra na vertical*/
                if ((i - 1 < 0 || matriz_Tabuleiro[i - 1][j] != 0) && (i + 1 < m && matriz_Tabuleiro[i + 1][j] == 0))
                {
                    guarda[k].posicao_m = i;
                    guarda[k].posicao_n = j;
                    guarda[k].dir = 1;
                    k++;
                }
            }
        }
    }
    return k;
}

int verifica(char **palavra_Cruzada, int lin, int col, palavra *lista_palavra, int k)
{
    if (palavra_Cruzada[lin][col] == lista_palavra[k].palavra[0] || palavra_Cruzada[lin][col] == '0')
        return 1;
    return 0;
}

int seleciona_Palavra(int **matriz_Tabuleiro, char **palavra_Cruzada, palavra *lista_Palavra, int m, int n, posicoes guarda, int num_palavras, int *palavra_disponivel, int comeco)
{
    int i, j;
    int k;
    int zeros;
    int en = 0;
    int selecionada = -1;
    int dir = guarda.dir, lin = guarda.posicao_m, col = guarda.posicao_n;

    if (dir == 0)
    {
        zeros = 0;
        /*conta o numero de zeros na linha até achar algo != de '0'*/
        for (j = col; j < n && matriz_Tabuleiro[lin][j] != -1; j++)
        {
            if (matriz_Tabuleiro[lin][j] == 0)
            {
                zeros++;
            }
        }
        for (k = comeco; k < num_palavras && en == 0; k++)
        {
            if (lista_Palavra[k].tam == zeros && palavra_disponivel[k] == 1 && (palavra_Cruzada[lin][col] == lista_Palavra[k].palavra[0] || palavra_Cruzada[lin][col] == '0'))
            {
                en = encaixa(palavra_Cruzada, m, n, lin, col, dir, lista_Palavra, k);
                if (en)
                {
                    selecionada = k;
                    palavra_disponivel[k] = 0;
                }
            }
        }
    }

    if (dir == 1)
    {
        zeros = 0;
        /*conta o numero de zeros na linha até achar algo != de '0'*/
        for (i = lin; i < m && matriz_Tabuleiro[i][col] != -1; i++)
        {
            if (matriz_Tabuleiro[i][col] == 0)
            {
                zeros++;
            }
        }
        for (k = comeco; k < num_palavras && en == 0; k++)
        {
            if (lista_Palavra[k].tam == zeros && palavra_disponivel[k] == 1 && (palavra_Cruzada[lin][col] == lista_Palavra[k].palavra[0] || palavra_Cruzada[lin][col] == '0'))
            {
                en = encaixa(palavra_Cruzada, m, n, lin, col, dir, lista_Palavra, k);
                if (en)
                {
                    selecionada = k;
                    palavra_disponivel[k] = 0;
                }
            }
        }
    }
    return selecionada;
}

int encaixa(char **palavra_Cruzada, int m, int n, int lin, int col, int dir, palavra *lista_Palavra, int k)
{
    int i, j;

    if (dir == 0)
    {
        for (j = col; j < n && palavra_Cruzada[lin][j] != '*'; j++)
        {
            if ((palavra_Cruzada[lin][j] != lista_Palavra[k].palavra[j - col]) && palavra_Cruzada[lin][j] != '0')
            {
                return 0;
            }
        }
    }
    if (dir == 1)
    {
        for (i = lin; i < m && palavra_Cruzada[i][col] != '*'; i++)
        {
            if ((palavra_Cruzada[i][col] != lista_Palavra[k].palavra[i - lin]) && palavra_Cruzada[i][col] != '0')
            {
                return 0;
            }
        }
    }

    return 1;
}
void coloca_Palavra(char **palavra_Cruzada, int m, int n, int lin, int col, int dir, palavra *lista_Palavra, int k, int **ocupadas)
{
    int i, j;
    int t;

    if (dir == 0)
    {
        for (t = 0, j = col; j < n && t < lista_Palavra[k].tam; t++, j++)
        {
            palavra_Cruzada[lin][j] = lista_Palavra[k].palavra[t];
            ocupadas[lin][j] += 1;
        }
    }
    if (dir == 1)
    {
        for (t = 0, i = lin; i < m && t < lista_Palavra[k].tam; t++, i++)
        {
            palavra_Cruzada[i][col] = lista_Palavra[k].palavra[t];
            ocupadas[i][col] += 1;
        }
    }
}

void apaga_Palavra(char **palavra_Cruzada, int m, int n, int lin, int col, int dir, int **ocupadas)
{
    int i, j;

    /*Horizontal*/
    if (dir == 0)
    {
        /*Percorre a linha até ela acabar ou achar um '*'*/
        for (j = col; j < n && palavra_Cruzada[lin][j] != '*'; j++)
        {
            /*Verifica se tem letra em cima ou em baixo daquela posição --> Se tem é pq aquela letra é usada duas vezes*/
            if (ocupadas[lin][j] < 2)
            {
                /*Apaga a palavra que tava lá*/
                palavra_Cruzada[lin][j] = '0';
                ocupadas[lin][j] -= 1;
            }
        }
    }
    /*Vertical*/
    if (dir == 1)
    {
        /*Percorre a coluna até ela acabar ou achar um '*'*/
        for (i = lin; i < m && palavra_Cruzada[i][col] != '*'; i++)
        {
            /*Verifica se tem letra aos lados daquela posição --> Se tem é pq aquela letra é usada duas vezes*/
            if (ocupadas[i][col] < 2)
            {
                /*Apaga a palavra que tava lá*/
                palavra_Cruzada[i][col] = '0';
                ocupadas[i][col] -= 1;
            }
        }
    }
}
int cruzadas(char **palavra_Cruzada, int **matriz_Tabuleiro, int m, int n, int num_palavras, palavra *lista_palavra)
{
    int i = 0;
    int j;
    int k;
    int num_posicoes;
    int comeco = 0;
    posicoes *guarda;
    int *palavra_disponivel;
    item pos_pal;
    item backtrack;
    pilha *p;
    int **ocupadas = malloc(m * sizeof(int *));
    for (i = 0; i < m; i++)
    {
        ocupadas[i] = malloc(n * sizeof(int));
        for (j = 0; j < n; j++)
        {
            ocupadas[i][j] = 0;
        }
    }

    p = cria();

    guarda = malloc(m * n * sizeof(posicoes));

    palavra_disponivel = malloc((num_palavras) * sizeof(int));
    for (i = 0; i < num_palavras; i++)
    {
        palavra_disponivel[i] = 1;
    }

    num_posicoes = pode_Colocar(matriz_Tabuleiro, m, n, guarda, num_palavras);

    i = 0;
    /*Iterar pelo vetor de posiçoes defino anteriormente*/
    while (i < num_posicoes)
    {
        k = -1;
        /*k é a palavra selecionanda pela função*/
        k = seleciona_Palavra(matriz_Tabuleiro, palavra_Cruzada, lista_palavra, m, n, guarda[i], num_palavras, palavra_disponivel, comeco);
    
        if (k != -1)
        {
            /*Coloca a palavra k no tabuleiro*/
            coloca_Palavra(palavra_Cruzada, m, n, guarda[i].posicao_m, guarda[i].posicao_n, guarda[i].dir, lista_palavra, k, ocupadas);
            /*Marca qual palavra foi usada e qual posições eu coloquei*/
            pos_pal.posicao_palavra = i;
            pos_pal.palavra_disponivel = k;
            /*empilha essas duas informações na pilha p*/
            empilha(p, pos_pal);
            comeco = 0;
            i++;
        }
        /*Não encaixa*/
        else
        {
            /*Se a pilha estiver vazia e k já tiver atingido o numero de palavras máximo, então não ha solução*/
            if (pilhaVazia(p) && k == -1)
            {
                free(guarda);
                free(palavra_disponivel);
                destroi(p);
                for (i = 0; i < m; i++)
                {
                    free(ocupadas[i]);
                }
                free(ocupadas);
                return 0;
            }
            /*Se não encaixa e a pilha não está vazia */
            else if (!pilhaVazia(p) && k == -1)
            {
                /*Desempilha a ultima empilhada e testa outra, pelo menos era pra fazer isso*/
                backtrack = desempilha(p);
                palavra_disponivel[backtrack.palavra_disponivel] = 1;
                i = backtrack.posicao_palavra;
                comeco = backtrack.palavra_disponivel + 1;
                /*Apaga aquele palavra do tabuleiro*/
                apaga_Palavra(palavra_Cruzada, m, n, guarda[i].posicao_m, guarda[i].posicao_n, guarda[i].dir, ocupadas);

            }
        }
    }
    free(guarda);
    free(palavra_disponivel);
    destroi(p);
    for (i = 0; i < m; i++)
    {
        free(ocupadas[i]);
    }
    free(ocupadas);
    return 1;
}