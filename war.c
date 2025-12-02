#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Estruturas de Dados ---

// Estrutura para representar um Território no mapa
typedef struct {
    char nome[30];
    char cor[10]; // Cor (dono) do território (ex: "Azul", "Vermelho")
    int tropas;
} Territorio;

// --- Protótipos das Funções ---

// Funções do jogo e utilitárias
void inicializarJogo(Territorio** mapa, int numTerritorios, char** missoes, int totalMissoes, char** missaoJogador);
void exibirMissao(char* missao); // Passagem por valor (embora leia o conteúdo do ponteiro)
void atribuirMissao(char* destino, char** missoes, int totalMissoes); // Passagem por referência (modifica destino)
int rolarDado();
void atacar(Territorio* atacante, Territorio* defensor);
int verificarMissao(char* missao, Territorio* mapa, int tamanho); // Passagem por referência (leitura do estado)
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missao); // Passagem por referência (libera memória)

// --- Implementação das Funções ---

/**
 * @brief Inicializa o jogo, alocando memória para o mapa e a missão do jogador,
 * e atribuindo a missão.
 * @param mapa Ponteiro para o ponteiro do vetor de Territórios (passagem por referência).
 * @param numTerritorios Número de territórios a serem criados.
 * @param missoes Vetor de strings contendo as descrições das missões.
 * @param totalMissoes Número total de missões disponíveis.
 * @param missaoJogador Ponteiro para o ponteiro da string de missão do jogador (alocação dinâmica).
 */
void inicializarJogo(Territorio** mapa, int numTerritorios, char** missoes, int totalMissoes, char** missaoJogador) {
    // Alocação de memória para o mapa usando calloc
    *mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    if (*mapa == NULL) {
        perror("Erro ao alocar memória para o mapa");
        exit(EXIT_FAILURE);
    }

    // Inicialização simples dos territórios
    strcpy((*mapa)[0].nome, "Asia Central"); strcpy((*mapa)[0].cor, "Azul"); (*mapa)[0].tropas = 3;
    strcpy((*mapa)[1].nome, "Groelandia"); strcpy((*mapa)[1].cor, "Vermelho"); (*mapa)[1].tropas = 2;
    strcpy((*mapa)[2].nome, "Alaska"); strcpy((*mapa)[2].cor, "Azul"); (*mapa)[2].tropas = 1;
    strcpy((*mapa)[3].nome, "Brasil"); strcpy((*mapa)[3].cor, "Amarelo"); (*mapa)[3].tropas = 4;
    strcpy((*mapa)[4].nome, "Nigeria"); strcpy((*mapa)[4].cor, "Vermelho"); (*mapa)[4].tropas = 5;

    // Alocação de memória para a missão do jogador (dinâmica)
    // O ponteiro missaoJogador agora aponta para o bloco de memória alocado.
    *missaoJogador = (char*)malloc(256 * sizeof(char)); 
    if (*missaoJogador == NULL) {
        perror("Erro ao alocar memória para a missão");
        exit(EXIT_FAILURE);
    }

    // Atribuição da missão
    atribuirMissao(*missaoJogador, missoes, totalMissoes);
}

/**
 * @brief Exibe a missão do jogador (passagem por valor).
 * @param missao A string (char*) contendo a descrição da missão.
 */
void exibirMissao(char* missao) {
    printf("\n*** MISSAO ESTRATEGICA ***\n");
    printf("Sua missao e: **%s**\n", missao);
    printf("***************************\n");
}

/**
 * @brief Sorteia uma missão e copia para o destino (passagem por referência).
 * @param destino O ponteiro para onde a missão sorteada será copiada.
 * @param missoes Vetor de strings (missões disponíveis).
 * @param totalMissoes Número total de missões.
 */
void atribuirMissao(char* destino, char** missoes, int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    // Usa strcpy para copiar o conteúdo para a memória alocada dinamicamente (destino)
    strcpy(destino, missoes[indiceSorteado]); 
}

/**
 * @brief Simula a rolagem de um dado.
 * @return Um inteiro entre 1 e 6.
 */
int rolarDado() {
    return (rand() % 6) + 1;
}

/**
 * @brief Simula um ataque entre dois territórios, atualizando as tropas e o dono.
 * @param atacante Ponteiro para o Território atacante.
 * @param defensor Ponteiro para o Território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Validação de ataque: território inimigo
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n❌ Ataque Invalido: Nao e possivel atacar um territorio aliado (%s).\n", defensor->nome);
        return;
    }
    // Validação de ataque: tropas suficientes
    if (atacante->tropas < 2) {
        printf("\n❌ Ataque Invalido: Necessario ao menos 2 tropas para atacar (%s).\n", atacante->nome);
        return;
    }

    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();

    printf("\n--- SIMULACAO DE COMBATE ---\n");
    printf("  %s (%s, %d tropas) ataca %s (%s, %d tropas)\n", 
           atacante->nome, atacante->cor, atacante->tropas, 
           defensor->nome, defensor->cor, defensor->tropas);
    printf("  Atacante rolou %d | Defensor rolou %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        int tropasTransferidas = atacante->tropas / 2;
        
        // Atualização de campos: Defensor perde, muda de cor e recebe tropas
        printf("  ✅ Atacante VENCEU! %s conquista %s.\n", atacante->cor, defensor->nome);
        printf("  %d tropas transferidas. %s agora pertence a %s.\n", tropasTransferidas, defensor->nome, atacante->cor);
        
        strcpy(defensor->cor, atacante->cor); 
        defensor->tropas = tropasTransferidas; 
        atacante->tropas -= tropasTransferidas;
    } else {
        // Defensor vence ou empate
        printf("  🔻 Defensor VENCEU! %s resistiu ao ataque.\n", defensor->nome);
        atacante->tropas--; // Atacante perde uma tropa
        printf("  %s perde uma tropa. Tropas restantes: %d.\n", atacante->nome, atacante->tropas);
    }
    printf("----------------------------\n");
}

/**
 * @brief Avalia se a missão do jogador (cor "Azul") foi cumprida.
 * @param missao A string (char*) contendo a descrição da missão.
 * @param mapa Ponteiro para o vetor de Territórios.
 * @param tamanho Número total de territórios.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    // Missão 1: "Conquistar o Brasil"
    if (strstr(missao, "Conquistar o Brasil") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            // Verifica se o território é o Brasil E se o dono é o jogador (Azul)
            if (strcmp(mapa[i].nome, "Brasil") == 0 && strcmp(mapa[i].cor, "Azul") == 0) {
                return 1; 
            }
        }
    }
    
    // Missão 3: "Eliminar a cor Vermelho"
    if (strstr(missao, "Eliminar a cor Vermelho") != NULL) {
        int corEliminada = 1;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                corEliminada = 0; // Ainda existe a cor Vermelho
                break;
            }
        }
        return corEliminada;
    }

    // Retorna 0 para missões não implementadas ou não cumpridas
    return 0; 
}

/**
 * @brief Exibe o estado atual do mapa.
 * @param mapa Ponteiro para o vetor de Territórios.
 * @param tamanho Número total de territórios.
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- ESTADO ATUAL DO MAPA ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("  [%d] %s | Dono: %s | Tropas: %d\n", 
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("----------------------------\n");
}

/**
 * @brief Libera toda a memória alocada dinamicamente, prevenindo vazamento de memória.
 * @param mapa Ponteiro para o vetor de Territórios alocado via calloc.
 * @param missao Ponteiro para a string de missão do jogador alocada via malloc.
 */
void liberarMemoria(Territorio* mapa, char* missao) {
    printf("\n--- GERENCIAMENTO DE MEMORIA ---\n");
    
    // Libera a memória do mapa
    if (mapa != NULL) {
        free(mapa);
        printf("  Memoria do Mapa liberada. (free(mapa))\n");
    }
    
    // Libera a memória da missão do jogador
    if (missao != NULL) {
        free(missao);
        printf("  Memoria da Missao liberada. (free(missaoJogador))\n");
    }
    printf("--------------------------------\n");
}

// --- Função Principal ---

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Vetor de strings com as missões pré-definidas (char* missoes[])
    char* missoes[] = {
        "Conquistar o Brasil",
        "Conquistar 3 territorios seguidos (A ser implementada)",
        "Eliminar a cor Vermelho",
        "Ter 10 tropas em um unico territorio (A ser implementada)",
        "Conquistar 5 territorios com 1 tropa em cada (A ser implementada)"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);
    int numTerritorios = 5;

    // Ponteiros para alocação dinâmica (o ponteiro inicia apontando para NULL)
    Territorio* mapa = NULL;
    char* missaoJogador = NULL; 

    // 1. Inicialização e Atribuição da Missão
    inicializarJogo(&mapa, numTerritorios, missoes, totalMissoes, &missaoJogador);
    
    // 2. Exibição da Missão (Passagem por valor/leitura)
    exibirMissao(missaoJogador);

    printf("\n--- INICIO DO JOGO: SIMULACAO DE TURNOS ---\n");
    
    int turnos = 3;
    int vencedor = 0;
    
    for (int t = 1; t <= turnos; t++) {
        if (vencedor) break;
        
        printf("\n==================================\n");
        printf("         TURNO %d\n", t);
        printf("==================================\n");
        exibirMapa(mapa, numTerritorios);

        // Exemplo de Ação do Jogador (Azul)
        // Tentativa de ataque 1 (mapa[0] para mapa[1])
        atacar(&mapa[0], &mapa[1]);
        
        // Tentativa de ataque 2 (mapa[3] para mapa[4])
        atacar(&mapa[3], &mapa[4]);
        
        // 3. Verificação da Missão (Passagem por referência para leitura)
        // Verifica se o jogador (Azul) cumpriu sua missão
        if (verificarMissao(missaoJogador, mapa, numTerritorios)) {
            printf("\n\n************************************************\n");
            printf("🏆🏆 MISSAO CUMPRIDA! O JOGADOR (Azul) VENCEU! 🏆🏆\n");
            printf("Missao: %s\n", missaoJogador);
            printf("************************************************\n");
            vencedor = 1;
        }
    }
    
    if (!vencedor) {
        printf("\n--- FIM DOS TURNOS SIMULADOS ---\n");
    }

    // 4. Liberação de Memória
    liberarMemoria(mapa, missaoJogador); // A função já sabe liberar ambos os ponteiros.
    
    return 0; // Retorno padrão para main
}