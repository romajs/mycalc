# MyCalc - Compiladores UNESP @ RC /2013

@@@.....@@@..@@@..@@@....@@@@@.......@@@.....@@@..........@@@@@..  
.@@@@.@@@@....@@..@@...@@@....@@....@@@@@.....@@........@@@....@@  
.@@.@@@.@@.....@@@@...@@...........@@...@@....@@.......@@........  
.@@.....@@......@@....@@...........@@@@@@@....@@.......@@........  
.@@.....@@......@@.....@@@....@@...@@...@@....@@...@@...@@@....@@  
@@@@...@@@@....@@@@......@@@@@....@@@...@@@..@@@@@@@@.....@@@@@..  

## Grupo 03

André Cantelli  
Daniel Felipe Lucas  
José Romualdo dos Santos  
Rodrigo Akio  
Tulio Pereira  

## How-To:

Para compilar utilize o comando `make` e depois execute com `./mycalc` em Unix ou `mycalc.exe` no Windows.  

Use a diretiva `-d` para acionar o modo de 'debug' ex: `./mycalc -d`.

ex:  
> "> 5*5  
E:  
T:  
F:  
(push) operand[0] = 5.00  
_F:  
(push) oper[0] = *  
F:  
(push) operand[1] = 5.00  
_F:  
(pop) oper[0] = *  
(pop)  operand[0] = 25.00  
_T:  
_E:  
 = 25  
status: 0  
OK."
