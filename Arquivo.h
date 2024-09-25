#define TAM_ID 4
#define TAM_SIGLA 4
#define TAM_NOME 50
#define TAM_DISCIPLINA 50
#define TAM_REGISTRO 120

extern const char* nomeDados;
extern const char* nomeIndicePrimario;
extern const char* nomeIndiceSecundario;
extern const char* nomeListaInvertida;

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
    char idAluno[TAM_ID];
    char siglaDisciplina[TAM_SIGLA];
    int offset;
} IndicePrimario;

typedef struct {
    char nomeAluno[TAM_NOME];
    int offsetListaInvertida;
} IndiceSecundario;

typedef struct {
    char idAluno[TAM_ID];
    char siglaDisciplina[TAM_SIGLA];
    int offset;
} ListaInvertida;

void printRegistro(Registro registro);
int carregarRegistros(Registro** registros);
int carregarBuscaChavePrimaria(BuscaChavePrimaria** chaves);
int carregarBuscaChaveSecundaria(BuscaChaveSecundaria** chaves);
void inicializarArquivo(const char* nomeArquivo);
void inserirRegistro(Registro registro);
void atualizarIndicePrimario(Registro* registro, int offset);
void atualizarIndiceSecundario(Registro registro);
Registro buscaChavePrimaria(BuscaChavePrimaria chaveBusca);
Registro buscaChaveSecundaria(const char* nomeBusca);