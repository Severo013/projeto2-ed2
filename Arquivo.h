#define TAM_ID 4
#define TAM_SIGLA 4
#define TAM_NOME 50
#define TAM_DISCIPLINA 50
#define TAM_REGISTRO 120

// Structs que vão ser carregados por arquivo e manipulados em memória

typedef struct {
    char idAluno[TAM_ID];
    char siglaDisciplina[TAM_SIGLA];
    char nomeAluno[TAM_NOME];
    char nomeDisciplina[TAM_DISCIPLINA];
    float media;
    float frequencia;
} Registro;

typedef struct {
    char idAluno[TAM_ID];
    char siglaDisciplina[TAM_SIGLA];
} BuscaChavePrimaria;

typedef struct {
    char nomeAluno[TAM_NOME];
} BuscaChaveSecundaria;

typedef struct {
    char id[TAM_ID];
    char sigla[TAM_SIGLA];
    int offset;
} IndicePrimario;

// Structs que vão ser gravados e manipulados em arquivo

typedef struct {
    char nome[TAM_NOME];
    int offset_lista_invertida;
} IndiceSecundario;

typedef struct {
    char id[TAM_ID];
    char sigla[TAM_SIGLA];
    int prox_offset;
} ListaInvertida;

int carregarRegistros(Registro** registros);
int carregarBuscaChavePrimaria(BuscaChavePrimaria** chaves);
int carregarBuscaChaveSecundaria(BuscaChaveSecundaria** chaves);


