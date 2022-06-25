# Locadora LocaMais

Trabalho interdisciplinar - Fundamentos de Engenharia de Software e Algoritmos e Estruturas de Dados I | PUC Minas - Engenharia de Software

```console
user@pc:~$ ./locamais 

============== LocaMais ==================
====== A melhor solução em locação! ======

## MENU ##
1 - Cadastrar cliente
2 - Cadastrar veiculo
3 - Cadastrar locação
4 - Listar clientes
5 - Listar veiculos
6 - Listar locações
7 - Dar baixa em uma locação
998 - Executar testes
999 - SAIR

>>> 
```

### Quadro do projeto

https://github.com/guilhermerodrigues680/trabalho-interdisciplinar-fes-aeds-i/projects/1

### Demo online

https://replit.com/@GuilhermeRodri8/trabalho-interdisciplinar-fes-aeds-i

### Compilando projeto com GCC

```sh
gcc -g src/*.c -o locamais
```

### Compilando com auxílio do Makefile

```sh
make -s
# ou
make main
# ou
make main-debug

# use a regra abaixo para excluir os arquivos gerados durante a compilação.
make clean
```

> O Makefile do projeto foi gerado pelo [replit.com](https://replit.com), assim o compilador padrão é o **`clang`**. Altere a váriavel `CC = clang` para `CC = gcc` para usar o compilador GCC.
