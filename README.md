@@@.....@@@..@@@..@@@....@@@@@.......@@@.....@@@..........@@@@@..  
.@@@@.@@@@....@@..@@...@@@....@@....@@@@@.....@@........@@@....@@  
.@@.@@@.@@.....@@@@...@@...........@@...@@....@@.......@@........  
.@@.....@@......@@....@@...........@@@@@@@....@@.......@@........  
.@@.....@@......@@.....@@@....@@...@@...@@....@@...@@...@@@....@@  
@@@@...@@@@....@@@@......@@@@@....@@@...@@@..@@@@@@@@.....@@@@@..  

### MyCalc - Compiladores UNESP @ RC /2013
Professor: Eraldo Pereira Marinho

Repositório disponível em: https://github.com/romajs/mycalc

#### Grupo 03
- André Cantelli  
- Daniel Felipe Lucas  
- José Romualdo dos Santos  
- Rodrigo Akio  
- Tulio Pereira  

#### Como utilizar:

Para compilar utilize o comando `make` e depois execute com `./mycalc` em Unix ou `mycalc` no Windows.  
Para sair do programa utilize as teclas `ctrl+c`.  

O programa aceita duas diretivas (argumentos):  
- `-d` modo debug, todo os passos serão exibidos na saída padrão;
- `-t` modo de testes, executa diversos testes pré-definidos no início do programa;

#### Descrição dos Arquivos:  
- args: manipulador de diretivas/argumentos;
- debug: funções úteis para exibição de modo debug;
- lexer: analisador léxico de mycalc;
- mycalc: arquivo principal contendo função `main` e loop do interpretador;
- mytype: arquivo definições de tipos próprios utilzados no programa;
- parser: anasilar sintático e manipulações/operações de pilha;
- tests: testes a serem executados no modo de testes;
- tokens: definições de constantes e símbolos da gramática
