/*BIBLIOTECAS*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para strcspn
#include <sqlite3.h>
/*******************************************************************************/
/*ESTRUTURAS DE DADOS*/
typedef struct MaterialPermanente{
    unsigned int nrp;
    char material[30];
    char marca[30];
    char modelo[30];
    char numeroSerie[30];
} MaterialPermanente;
/********************************************************************************/
/*PROTÓTIPOS*/
void limparBuffer();
int telaInicial();
MaterialPermanente* cadastrarMaterialPermanente();
void printMP(MaterialPermanente *mp);
void salvarNoBD(MaterialPermanente *p_mp);
int callback(void *data, int argc, char **argv, char **colName);
void consultarNRP(int nrp);
/*********************************************************************************/
int main(int argc, char *argv[]) 
{
    int escolha = telaInicial();
    switch (escolha)
    {
    case 1: // Cadastrar material permanente
        MaterialPermanente *mp =  cadastrarMaterialPermanente();
        break;
    
    case 2: // Consultar material permanente
        consultarNRP(415823);
        break;

    case 9: // Sair
        return 0;
        break;

    default:
        break;
    }
    return 0;
}

/*********************************************************************************/

/*FUNÇÕES*/
void limparBuffer() 
{
    int c;
    // Lê caractere por caractere até achar o Enter (\n) ou o fim do arquivo (EOF)
    while ((c = getchar()) != '\n' && c != EOF);
}

int telaInicial() 
{
    char a_opcao[10]; 
    int opcao = 0;
    
    // Ponteiro para validação do strtol
    char *endptr; 

    do {
        // system("clear") é para Linux/Mac. No Windows seria system("cls").
        // Para ser portável, costumamos usar diretivas de pré-processador, mas deixaremos assim por ora.
        system("clear"); 

        printf("##### SISTEMA PATRIMONIAL #####\n");
        printf("1 - Cadastrar material permanente\n");
        printf("2 - Consultar material permanente\n");
        printf("9 - Sair\n");
        printf("Selecione uma opcao: ");

        // 1. Leitura segura
        if (fgets(a_opcao, sizeof(a_opcao), stdin) != NULL) {
            
            // 2. Verifica se o \n foi lido (se coube tudo no buffer)
            // A função strcspn retorna o tamanho do segmento antes do \n
            if (a_opcao[strcspn(a_opcao, "\n")] != '\n') 
            {
                // Se não tinha \n na string lida, significa que o usuário digitou
                // mais caracteres do que cabia. O resto está no buffer (stdin).
                // Precisamos limpar!
                limparBuffer();
            }
            
            // 3. Conversão com validação
            opcao = (int)strtol(a_opcao, &endptr, 10);

            // Validação extra: Se endptr aponta para a_opcao, nada foi convertido (ex: usuário digitou "abc")
            if (endptr == a_opcao) {
                opcao = 0; // Força uma opção inválida para repetir o loop
            }
        }
    } while (opcao != 1 && opcao != 2 && opcao != 9);

    return opcao;
}

MaterialPermanente* cadastrarMaterialPermanente()
{
    char a_buffer[30];
    memset(a_buffer, 0, sizeof(a_buffer));
    
    char *endptr;

    // Alocação de memória na HEAP
    MaterialPermanente *p_mp = (MaterialPermanente *)malloc(sizeof(MaterialPermanente));
    if(p_mp == NULL){
        perror("[cadastrarMaterialPermanente] Erro ao alocar memória");
        return NULL;
    }
    memset(p_mp, 0, sizeof(*p_mp));

    system("clear");

    // Cadastro
    char *campos[5] = {"NRP", "MATERIAL", "MARCA", "MODELO", "NUMERO DE SERIE"};
    printf("##### CADASTRO DE MATERIAL PERMANENTE ######\n");
    for(int i = 0; i < 5; i++)
    {
        memset(a_buffer, 0, sizeof(a_buffer));
        printf("%s: ", campos[i]);
        if(fgets(a_buffer, sizeof(a_buffer), stdin) == NULL)
        {
            perror("[cadastrarMaterialPermanente] Erro na leitura do campo");    
        }

        size_t pos = strcspn(a_buffer, "\n");
        
        if (a_buffer[pos] != '\n')
        {
            limparBuffer();
        }
        else
        {
            a_buffer[pos] = '\0';
        }

        switch(i) {
            case 0: 
                p_mp->nrp = (int)strtol(a_buffer, &endptr, 10); 
                break;
            case 1: 
                strcpy(p_mp->material, a_buffer); 
                break;
            case 2: 
                strcpy(p_mp->marca, a_buffer); 
                break;
            case 3: 
                strcpy(p_mp->modelo, a_buffer); 
                break;
            case 4: 
                strcpy(p_mp->numeroSerie, a_buffer); 
                break;
            default:
                break;
        }
    }

    salvarNoBD(p_mp);
    return p_mp;
}

void printMP(MaterialPermanente *mp)
{
    printf("NRP %d | Material: %s | Marca: %s | Modelo: %s | NS: %s\n", mp->nrp, mp->material, mp->marca , mp->modelo , mp->numeroSerie);
}

void salvarNoBD(MaterialPermanente *p_mp)
{
    sqlite3 *db;
    char *errMsg = NULL;
    
    // Abrir banco
    if(sqlite3_open("database.db", &db) != SQLITE_OK)
    {
        printf("Erro ao abrir banco: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Criar tabela
    const char *sql_create =
        "CREATE TABLE IF NOT EXISTS material_permanente ("
        "nrp INTEGER PRIMARY KEY, "
        "material TEXT, "
        "marca TEXT, "
        "modelo TEXT, "
        "numero_serie TEXT"
        ");";

    if (sqlite3_exec(db, sql_create, NULL, NULL, &errMsg) != SQLITE_OK) {
        printf("Erro ao criar tabela: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }

    // Inserção com prepared statement
    const char *sql_insert =
        "INSERT INTO material_permanente "
        "(nrp, material, marca, modelo, numero_serie) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Erro ao preparar SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Bind dos valores
    sqlite3_bind_int(stmt, 1, p_mp->nrp);
    sqlite3_bind_text(stmt, 2, p_mp->material, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, p_mp->marca, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, p_mp->modelo, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, p_mp->numeroSerie, -1, SQLITE_TRANSIENT);

    // Executa
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Erro ao inserir: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Registro inserido com sucesso no banco de dados!\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int callback(void *data, int argc, char **argv, char **colName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void consultarNRP(int nrp){
    sqlite3 *db;
    char *errMsg = NULL;

    // Abrir banco
    if(sqlite3_open("database.db", &db) != SQLITE_OK)
    {
        printf("Erro ao abrir banco: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Consultar dados
    const char *sql = 
        "SELECT nrp, material, marca, modelo, numero_serie "
        "FROM material_permanente WHERE nrp = ?;";

    sqlite3_stmt *stmt;

    // Prepara comando SQL
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
        printf("Erro ao preparar SELECT: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Faz o bind do parâmetro
    sqlite3_bind_int(stmt, 1, nrp);

    // Executa linha por linha
    while(sqlite3_step(stmt) == SQLITE_ROW){
        printf("NRP: %d\n", sqlite3_column_int(stmt, 0));
        printf("Material: %s\n", sqlite3_column_text(stmt, 1));
        printf("Marca: %s\n", sqlite3_column_text(stmt, 2));
        printf("Modelo: %s\n", sqlite3_column_text(stmt, 3));
        printf("NS: %s\n\n", sqlite3_column_text(stmt, 4));
    }

    sqlite3_finalize(stmt);

    // 5. Fechar
    sqlite3_close(db);

}