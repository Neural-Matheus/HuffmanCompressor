# Compactador e Descompactador de Huffman

Este repositório contém um projeto que implementa um compactador e descompactador de arquivos usando o algoritmo de Huffman. O algoritmo é uma técnica de compressão de dados sem perda que utiliza as frequências de ocorrência de caracteres para codificar e decodificar texto.

## Pré-requisitos

Para compilar e executar este código, você precisará de:

- Um compilador C++ compatível com o padrão C++11 ou superior.

## Instalação

Clone este repositório usando:

```bash
https://github.com/Neural-Matheus/HuffmanCompressor
```

## Compilação

Navegue até o diretório do projeto e compile o código usando o seguinte comando:

```bash
g++ -o huffman main.cpp -std=c++11
```

## Uso

Após compilar o projeto, você pode usar o executável `huffman` para compactar ou descompactar arquivos. Aqui estão os comandos para cada operação:

### Compactar um arquivo

Para compactar um arquivo, use:

```bash
./huffman c <arquivo_entrada> <arquivo_saida>
```

Substitua `<arquivo_entrada>` pelo nome do arquivo que você deseja compactar e `<arquivo_saida>` pelo nome do arquivo que será criado após a compactação.

### Descompactar um arquivo

Para descompactar um arquivo, use:

```bash
./huffman d <arquivo_entrada> <arquivo_saida>
```

Substitua `<arquivo_entrada>` pelo nome do arquivo compactado e `<arquivo_saida>` pelo nome do arquivo que será restaurado após a descompactação.

Este projeto foi desenvolvido na matéria de Estrutura de Dados na UFMS.
