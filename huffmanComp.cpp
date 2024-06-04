/**************************************************
 *
 * Caio Felipe Azevedo Tome
 * Matheus Vinicius da Silva de Oliveira
 * Trabalho de Estruturas de Dados: Huffman
 * Professor: Diego Padilha Rubert
 *
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>    // permitida
#include <map>       // permitida
#include <cstdio>    // permitida
#include <climits>   // permitida
#include <algorithm> // permitida por estar na função da buffer do professor
#include <cstring>
#include <bitset> // permitida
#include <cstdlib>
#include <cstdint>
#include <vector> // permitida

#define TAM 256

using std::string;
using namespace std;

class Bits // definição do Buffer
{
public:
    FILE *file;
    uint8_t b[8];
    uint8_t n;
    Bits(FILE *file);
    uint8_t ocupados();
    uint8_t livres();

    void adiciona_bit(uint8_t bit);
    void descarrega();
    uint8_t obtem_bit();
};

Bits::Bits(FILE *file) : file(file), n(0) {} // Contrutor Buffer

void Bits::adiciona_bit(uint8_t bit) // Adiciona um bit no Buffer
{
    b[n++] = bit != 0;

    if (n == 8)
        descarrega();
}

void Bits::descarrega() // Joga 8 bits no arquivo
{
    uint8_t byte = 0;

    // Os bits são acumulados de b[7] a b[0], assumindo que b[7] é o bit mais significativo
    for (int i = 0; i < 8; i++)
    {
        byte = (byte << 1) | b[i];
    }

    fwrite(&byte, sizeof(uint8_t), 1, file);

    n = 0;
}

uint8_t Bits::obtem_bit() // Obter um bit para colocar no buffer
{
    if (n == 0)
    {
        uint8_t byte;

        size_t numero_bytes_lidos = fread(&byte, sizeof(uint8_t), 1, file);

        if (numero_bytes_lidos == 0)
        {
            return 2;
        }

        for (int i = 0; i < 8; i++)
        {
            b[7 - i] = (byte & (1 << i)) ? 1 : 0;
        }

        n = 8;
    }

    uint8_t bit = b[8 - n];

    n--;

    return bit;
}

uint8_t Bits::ocupados() // mostra quantos bits estão ocupados
{
    return n;
}

uint8_t Bits::livres() // mostra quantos bits estão livres
{
    return 8 - n;
}
class Processo // Definição da Àrvore
{
    friend class Heap;

public:
    Processo();
    ~Processo(){};
    Processo(int p, int id, Processo *esq = nullptr, Processo *dir = nullptr);
    void escreve(const char *sep = "");
    int p;
    int id;
    Processo *esq;
    Processo *dir;
};

class Heap
{
public:
    Heap();
    Heap(const int n, const Processo dados[]);
    Heap(const Heap &outro);
    ~Heap();
    Heap &operator=(const Heap &outro);
    void escreve_niveis();
    void escreve(const string &prefixo = "", int i = 0);
    void insere(int p, int id, Processo *esq = nullptr, Processo *dir = nullptr);
    Processo consulta_minima();
    Processo extrai_minima();
    void altera_prioridade(int i, int p);
    int n;

private:
    Processo *S;
    int capacidade;
    static const int TAMANHO_INICIAL = 4;

    int pai(int i);
    int esquerdo(int i);
    int direito(int i);
    void troca(int i, int j);
    void desce(int i);
    void sobe(int i);
};

Processo::Processo() : p(0), id(0), esq(nullptr), dir(nullptr) {} // criação de Processo

Processo::Processo(int p, int id, Processo *esq, Processo *dir) : p(p), id(id), esq(esq), dir(dir) {} // criação de Processo

void Processo::escreve(const char *sep) // para teste
{
    printf("(%d, %d)%s", p, id, sep);
}

Heap::Heap() : n(0), capacidade(TAMANHO_INICIAL) // cria um tamanho inicial
{
    S = new Processo[TAMANHO_INICIAL];
}

Heap::Heap(const int n, const Processo dados[]) : n(n), capacidade(n * 2)
{
    S = new Processo[capacidade];
    for (int i = 0; i < n; ++i)
    {
        S[i] = dados[i];
    }
    for (int i = pai(n - 1); i >= 0; --i)
    {
        desce(i);
    }
}

Heap::Heap(const Heap &outro) : n(outro.n), capacidade(outro.capacidade)
{
    S = new Processo[capacidade];
    std::copy(outro.S, outro.S + outro.n, S);
}

Heap::~Heap()
{
    delete[] S;
}

Heap &Heap::operator=(const Heap &outro)
{
    if (this != &outro)
    {
        delete[] S;
        n = outro.n;
        capacidade = outro.capacidade;
        S = new Processo[capacidade];
        std::copy(outro.S, outro.S + outro.n, S);
    }
    return *this;
}

void Heap::escreve_niveis()
{
    int nivel = 0;
    int potencia2 = 1;
    for (int i = 0; i < n; ++i)
    {
        if (i == potencia2)
        {
            printf("\nNivel %d: ", nivel++);
            potencia2 *= 2;
        }
        S[i].escreve(" ");
    }
    printf("\n");
}

void Heap::escreve(const string &prefixo, int i)
{
    if (i < n)
    {
        printf("%s", prefixo.c_str());
        S[i].escreve("\n");
        escreve(prefixo + "│   ", esquerdo(i));
        escreve(prefixo + "└── ", direito(i));
    }
}

void Heap::insere(int p, int id, Processo *esq, Processo *dir)
{
    if (n == capacidade)
    {
        capacidade *= 2;
        Processo *novoS = new Processo[capacidade];
        std::copy(S, S + n, novoS);
        delete[] S;
        S = novoS;
    }
    int i = n++;
    S[i] = Processo(p, id, esq, dir);
    sobe(i);
}

Processo Heap::consulta_minima()
{
    if (n > 0)
    {
        return S[0];
    }
    else
    {
        return Processo(INT_MAX, -1);
    }
}

Processo Heap::extrai_minima()
{
    if (n > 0)
    {
        Processo min = S[0];
        S[0] = S[--n];
        desce(0);
        return min;
    }
    else
    {
        return Processo(INT_MAX, -1);
    }
}

void Heap::altera_prioridade(int i, int p)
{
    if (i >= 0 && i < n)
    {
        S[i].p = p;
        if (pai(i) >= 0 && S[i].p < S[pai(i)].p)
        {
            sobe(i);
        }
        else
        {
            desce(i);
        }
    }
}

int Heap::pai(int i)
{
    return (i - 1) / 2;
}

int Heap::esquerdo(int i)
{
    return 2 * i + 1;
}

int Heap::direito(int i)
{
    return 2 * i + 2;
}

void Heap::troca(int i, int j)
{
    Processo temp = S[i];
    S[i] = S[j];
    S[j] = temp;
}

void Heap::desce(int i)
{
    while (esquerdo(i) < n)
    {
        int j = esquerdo(i);
        if (direito(i) < n && S[direito(i)].p < S[j].p)
        {
            j = direito(i);
        }
        if (S[i].p <= S[j].p)
        {
            break;
        }
        troca(i, j);
        i = j;
    }
}

void Heap::sobe(int i)
{
    while (i > 0 && S[i].p < S[pai(i)].p)
    {
        troca(i, pai(i));
        i = pai(i);
    }
}

class TabelaFrequencia
{
public:
    unsigned int tabela[TAM];
    TabelaFrequencia()
    {
        for (int i = 0; i < TAM; i++)
        {
            tabela[i] = 0;
        }
    }

    void preencher(unsigned char texto[])
    {
        int i = 0;
        while (texto[i] != '\0')
        {
            tabela[texto[i]]++;
            i++;
        }
    }
    void imprimir() const
    {
        for (int i = 0; i < TAM; i++)
        {
            if (tabela[i] > 0)
            {
                printf("\t%d = %d = %c\n", i, tabela[i], i);
            }
        }
    }

    Heap criarMinHeap()
    {
        Heap minHeap;
        for (int i = 0; i < TAM; i++)
        {
            if (tabela[i] > 0)
            {
                minHeap.insere(tabela[i], i);
            }
        }

        std::cout << "Min Heap criada com as frequências:\n";
        return minHeap;
    }
};

class huffman
{
public:
    Processo *raiz;

    Processo *criarArvoreHuffman(Heap &minHeap) // usa a MinHeap para criar a árvore
    {
        while (minHeap.n > 1)
        {
            Processo p1 = minHeap.extrai_minima();
            Processo p2 = minHeap.extrai_minima();

            minHeap.insere(p1.p + p2.p, 0, new Processo(p1), new Processo(p2));
        }
        return new Processo(minHeap.extrai_minima());
    }

    void imprimir(Processo *raiz, int tam) // teste para ver c está bom
    {
        if (raiz == nullptr)
        {
            return;
        }
        if (raiz->esq == nullptr && raiz->dir == nullptr)
        {
            printf("\tFolha : %c\tAltura: %d\n", raiz->id, tam);
        }
        else
        {
            imprimir(raiz->esq, tam + 1);
            imprimir(raiz->dir, tam + 1);
        }
    }

    int altura_arvore(Processo *raiz) // vê a altura da árvore
    {
        if (raiz == nullptr)
            return -1;
        else
        {
            int esq = altura_arvore(raiz->esq) + 1;
            int dir = altura_arvore(raiz->dir) + 1;
            return std::max(esq, dir);
        }
    }

    char **aloca_dicionario(int colunas) // cria o dicionário alocando memória
    {
        char **dicionario = new char *[TAM];

        for (int i = 0; i < TAM; i++)
        {
            dicionario[i] = new char[colunas + 1];
            memset(dicionario[i], '\0', colunas + 1);
        }

        return dicionario;
    }

    void gerar_dicionario(char **dicionario, Processo *raiz, const char *caminho, int colunas) // coloca os códigos e letras no dicionário alocado
    {
        if (raiz->esq == nullptr && raiz->dir == nullptr)
        {
            strcpy(dicionario[raiz->id], caminho);
        }
        else
        {
            char *esquerda = new char[colunas + 2];
            char *direita = new char[colunas + 2];

            strcpy(esquerda, caminho);
            strcpy(direita, caminho);

            strcat(esquerda, "0");
            strcat(direita, "1");

            gerar_dicionario(dicionario, raiz->esq, esquerda, colunas);
            gerar_dicionario(dicionario, raiz->dir, direita, colunas);

            delete[] esquerda;
            delete[] direita;
        }
    }

    void imprime_dicionario(char **dicionario) // teste para ver se está de boa
    {
        printf("\n\tDicionario:\n");
        for (int i = 0; i < TAM; i++)
        {
            if (strlen(dicionario[i]) > 0)
            {
                printf("\t%3d: %s\n", i, dicionario[i]);
            }
        }
    }

    void serializar_arvore(Processo *raiz, string &estrutura_arvore, string &folhas, Bits *buffer) // cria o percurso Pré-Ordem
    {
        if (raiz->esq == nullptr && raiz->dir == nullptr)
        {
            buffer->adiciona_bit((uint8_t)1);
            estrutura_arvore += '1'; // Adicionando '1' para folhas
            folhas += (char)raiz->id;
        }
        else
        {
            buffer->adiciona_bit((uint8_t)0);
            estrutura_arvore += '0'; // Adicionando '0' para nós internos
            if (raiz->esq != nullptr)
                serializar_arvore(raiz->esq, estrutura_arvore, folhas, buffer);
            if (raiz->dir != nullptr)
                serializar_arvore(raiz->dir, estrutura_arvore, folhas, buffer);
        }
    }
};

int calcula_tamanho_string(char **dicionario, unsigned char texto[]) // nome explica
{
    int tamanho = 0;
    for (int i = 0; texto[i] != '\0'; i++)
    {
        tamanho += strlen(dicionario[texto[i]]);
    }
    return tamanho;
}

void reconstruir_arvore_huffman(Processo *&raiz, Bits &bits, const vector<char> &folhas, size_t &indice_folhas, int max_folhas) // criar a árvore a partir da arquivo compactado
{
    uint8_t bit = bits.obtem_bit();

    if (bit == 2)
    {
        std::cerr << "Erro: final inesperado do arquivo.\n";
        exit(1);
    }

    long long unsigned int tampa_buraco = (long long unsigned int)max_folhas;

    if (bit == 1)
    {
        if (indice_folhas >= tampa_buraco)
        {
            std::cerr << "Erro: índice de folhas excede o número máximo esperado de folhas.\n";
            exit(1);
        }
        if (indice_folhas < folhas.size())
        {
            raiz = new Processo(0, static_cast<int>(folhas[indice_folhas++]));
        }
        else
        {
            std::cerr << "Erro: índice de folhas fora do intervalo.\n";
            exit(1);
        }
    }
    else if (bit == 0)
    { // Se o bit é 0, o próximo elemento é um nó interno
        raiz = new Processo();
        reconstruir_arvore_huffman(raiz->esq, bits, folhas, indice_folhas, max_folhas);
        reconstruir_arvore_huffman(raiz->dir, bits, folhas, indice_folhas, max_folhas);
    }
}

void descompactar(const char *entrada, const char *saida) // função responsável por descompactar
{
    FILE *arquivo = fopen(entrada, "rb");
    if (!arquivo)
    {
        std::cerr << "Erro ao abrir o arquivo de entrada.\n";
        return;
    }

    uint16_t K;
    uint32_t T;
    int Kt = fread(&K, sizeof(uint16_t), 1, arquivo);
    int Tt = fread(&T, sizeof(uint32_t), 1, arquivo);
    if (!Kt)
    {
        printf("Não foI!");
    }
    if (!Tt)
    {
        printf("Não foi!");
    }
    int result1 = (int)T;
    int result2 = (int)K;
    printf("%d, %d", result1, result2);

    vector<char> folhas(K);
    int arvo = fread(folhas.data(), sizeof(char), K, arquivo);
    if (!arvo)
    {
        printf("Arquivo de árvore não lido!");
    }

    Bits bits(arquivo);

    Processo *raiz = nullptr;
    size_t indice_folhas = 0;
    reconstruir_arvore_huffman(raiz, bits, folhas, indice_folhas, result2);

    Processo *aux = raiz;
    FILE *descompactado = fopen(saida, "wb");
    if (!descompactado)
    {
        std::cerr << "Erro ao abrir o arquivo de saída.\n";
        return;
    }
    int total_letras = (int)T;

    printf("Total de Letras: %d\n", total_letras);

    while (total_letras > 0)
    {
        uint8_t bit = bits.obtem_bit();
        if (bit == 2)
        {
            break;
        }

        if (bit == 0)
        {
            aux = aux->esq;
        }

        else
        {
            aux = aux->dir;
        }

        if (aux->esq == nullptr && aux->dir == nullptr)
        {
            total_letras--;

            fputc(aux->id, descompactado);
            aux = raiz;
        }
    }

    printf("Total de Letras Que faltaram: %d\n", total_letras);

    fclose(descompactado);
    fclose(arquivo);
}

void codificar(char **dicionario, Bits *buffer, const char *entrada) // função que coloca os códigos de acordo com o dicionário
{
    FILE *original = fopen(entrada, "r");

    if (!original)
    {
        perror("Falha ao abrir o arquivo");
        return;
    }

    char c;
    // Lê caracteres um a um até que fread retorne um valor menor que o número de elementos solicitados
    while (fread(&c, sizeof(char), 1, original) == 1)
    {
        char *codigo = dicionario[(unsigned char)c]; // Acesso correto ao dicionário
        for (int i = 0; codigo[i] != '\0'; i++)
        {
            buffer->adiciona_bit(codigo[i] - '0'); // Adiciona cada bit do código ao buffer
        }
    }
    fclose(original); // Fecha o arquivo aberto
}

void percurso_pre_ordem(Processo *raiz, FILE *arquivo) // Executa o Percurso pre Ordem e escreve no arquivo
{
    if (raiz == nullptr)
    {
        return; // Se o nó é nulo, apenas retorna.
    }
    if (raiz->esq == nullptr && raiz->dir == nullptr)
    {
        // Se o nó é uma folha, escreve o identificador no arquivo
        uint8_t letra = raiz->id;
        fwrite(&letra, sizeof(uint8_t), 1, arquivo);
    }
    // Chamadas recursivas para esquerda e direita
    percurso_pre_ordem(raiz->esq, arquivo);
    percurso_pre_ordem(raiz->dir, arquivo);
}

void compactar(TabelaFrequencia &tabela, huffman &arvore, Processo *raiz, char **dicionario, const char *entrada, const char *saida) // função que compacta o arquivo
{
    FILE *arquivo = fopen(saida, "wb");
    if (arquivo)
    {
        int num_chars = 0;
        for (int i = 0; i < TAM; i++)
        {
            if (tabela.tabela[i] > 0)
            {
                num_chars++;
            }
        }

        uint16_t K = (uint16_t)num_chars;
        fwrite(&K, sizeof(uint16_t), 1, arquivo);

        int unique = 0;

        for (int i = 0; i < TAM; i++)
        {
            unique += tabela.tabela[i];
        }

        uint32_t T = (uint32_t)unique;

        fwrite(&T, sizeof(uint32_t), 1, arquivo);
        Bits *buffer = new Bits(arquivo);

        percurso_pre_ordem(raiz, arquivo);

        string estrutura_arvore = "", folhas = "";
        arvore.serializar_arvore(raiz, estrutura_arvore, folhas, buffer);

        codificar(dicionario, buffer, entrada);

        if (buffer->ocupados() > 0)
        {
            uint8_t byte = 0;
            for (int i = 0; i < buffer->ocupados(); i++)
            {
                byte = (byte << 1) | buffer->b[i];
            }

            byte <<= (8 - buffer->ocupados()); // Preencher com 0 os bits restantes
            fwrite(&byte, sizeof(uint8_t), 1, arquivo);
        }

        fclose(arquivo);
        delete buffer;
    }
    else
    {
        printf("\nErro ao abrir/criar arquivo em compactar\n");
    }
}

char *decodificar(unsigned char texto[], Processo *raiz) // devolve o a sequencia de bits decodificada
{
    int i = 0;
    Processo *aux = raiz;
    char temp[2];
    char *decodificado = new char[strlen((char *)texto) + 1];

    decodificado[0] = '\0';

    while (texto[i] != '\0')
    {
        if (texto[i] == '0')
            aux = aux->esq;
        else
            aux = aux->dir;

        if (aux->esq == nullptr && aux->dir == nullptr)
        {
            temp[0] = aux->id;
            temp[1] = '\0';
            strcat(decodificado, temp);
            aux = raiz;
        }

        i++;
    }
    return decodificado;
}

int main(int argc, char **argv) // função main
{
    if (argc != 4)
    {
        std::cerr << "Uso: " << argv[0] << " c|d <arquivo_entrada> <arquivo_saida>\n";
        return 1;
    }

    char operacao = argv[1][0];
    const char *entrada = argv[2];
    const char *saida = argv[3];

    if (operacao == 'c')
    {
        // Carregar dados do arquivo de entrada
        FILE *arquivo_entrada = fopen(entrada, "rb");
        if (!arquivo_entrada)
        {
            std::cerr << "Erro ao abrir o arquivo de entrada.\n";
            return 1;
        }

        // Leitura do texto do arquivo para um buffer
        fseek(arquivo_entrada, 0, SEEK_END);
        long tamanho = ftell(arquivo_entrada);
        fseek(arquivo_entrada, 0, SEEK_SET);
        unsigned char *texto = new unsigned char[tamanho + 1];
        int arq = fread(texto, 1, tamanho, arquivo_entrada);
        if (!arq)
        {
            printf("Arquivo não lido!");
        }
        texto[tamanho] = '\0';
        fclose(arquivo_entrada);

        // Realiza a compactação
        TabelaFrequencia tabela;
        tabela.preencher(texto);
        tabela.imprimir();
        Heap minHeap = tabela.criarMinHeap();
        huffman arvore;
        Processo *raiz = arvore.criarArvoreHuffman(minHeap);
        int altura = arvore.altura_arvore(raiz);
        char **dicionario = arvore.aloca_dicionario(altura);
        arvore.gerar_dicionario(dicionario, raiz, "", altura);
        arvore.imprime_dicionario(dicionario);
        compactar(tabela, arvore, raiz, dicionario, entrada, saida);

        // Limpeza
        delete[] texto;
        for (int i = 0; i < TAM; i++)
        {
            delete[] dicionario[i];
        }
        delete[] dicionario;
    }
    else if (operacao == 'd')
    {
        descompactar(entrada, saida);
    }
    else
    {
        std::cerr << "Operação desconhecida '" << operacao << "'. Use 'c' para compactar ou 'd' para descompactar.\n";
        return 1;
    }

    return 0;
}
// Fim