# T1_ED

## Trabalho 1 - Alocação Dinâmica, Listas

- [x] Criar um programa para controle acadêmico, que atenda aos seguintes requisitos mínimos:

- [x] Os dados devem ser armazenados em listas em memória.

- [x] Deve ser possível cadastrar alunos, professores, disciplinas e turmas (operações: incluir, excluir, buscar e visualizar). 

- [x] Cada aluno deve ter os seguintes dados: matrícula, nome, curso.

- [x] Cada professor deve ter os seguintes dados: código, nome.

- [x] Cada disciplina deve ter os seguintes dados: código, curso, nome, carga horária.

- [x] Deve ser possível matricular alunos em uma turma. Uma turma pode ter apenas um professor associado.

-----------
## Comandos:
### SHOW:
Para visualizar as listas do sistema:  
![image](https://user-images.githubusercontent.com/50972168/201409345-a861b660-a6fe-4be3-96e6-6f6dc6cd8439.png)

### ADD:
Para adicionar novos elementros as listas deve-se se digitar ADD seguido do nome da lista e de seus campos:  

Para adicionar um curso:   
> ADD CURSO *NOME_CURSO*  

Para adicionar um professor:
> ADD PROFESSOR *NOME_PROFESSOR*  

Para adicionara um aluno:  
> ADD ALUNO *NOME_ALUNO* *NUM_COD_CURSO*  

Para adicionar uma disciplina:  
> ADD DISCIPLINA *NOME_DISCIPLINE* *NUM_COD_CURSO*  

Para adicionar uma turma:  
> ADD TURMA *NOME_TURMA" *NUM_COD_PROFESSOR* *COD_NUM_DISCIPLINA*  

para adicionar alunos as turmas usa-se o comando MERGE;

### SEARCH:
O sistema conta com dois "registradores" S1 e S2 que armazenam os resultados das buscas e são posteriormente utilizados para outros comandos;
Para realizar uma busca usá-se a sintaxe:  
> SEARCH S1|S2 *NOME_TABELA* *NOME_COLUNA* *TERMO_DE_BUSCA*

ex.:  
- Buscar turmas de disciplinas contendo "DADOS" no nome e armazenar em S1:  
  >SEARCH S1 TURMA #DISCIPLINA NOME DADOS 
  
- Buscar alunos de cursos que tenham ID = 1 e armazenar em S2:  
  >SEARCH S2 ALUNO #CURSO ID 1
  
  
### VIEW [NOME DA LISTA]:
Para visualizar uma lista, usa-se o comando VIEW seguido do nome da tabela (ex.: VIEW CURSO)
![image](https://user-images.githubusercontent.com/50972168/201408934-757f6f09-6aa3-4883-bd19-18b54460a88c.png)

O mesmo comando pode ser utilizado para visualizar S1 e S2:
exemplo de VIEW dos comandos de busca realizados acima:.
- > VIEW S1  
![image](https://user-images.githubusercontent.com/50972168/201416522-f5ab658e-2b8a-43cf-8485-bbd0884bd51a.png)


- > VIEW S2 
![image](https://user-images.githubusercontent.com/50972168/201416392-c26a1920-d15b-4d9e-828d-3281309a1ade.png)


### MERGE:
Caso queira-se adicionar alunos à uma ou mais turmas, faz-se uma buscar por turmas e outra por alunos e usá-se o comando merge, considerando os comandos realizados acima:  
> MERGE S1 S2  
Irá adicionar todos os alunos de S2 nas turmas que estão em S1;
![image](https://user-images.githubusercontent.com/50972168/201417170-536461ef-7334-4d69-838b-3fbffb22a11b.png)


### DELETE:  
Levando em consideração os comandos anteriores, caso queira-se reverter o ultimo comando, ou seja, excluir o aluno da turma, usamos o comando DELETE:  
> DELETE S1 S2  

O comando delete pode ser usado também para deletar itens de listas:
> SEARCH S2 DISCIPLINA NOME CALCULO
> DELETE DISCIPLINA S2
> VIEW DISCIPLINA
![image](https://user-images.githubusercontent.com/50972168/201421655-73b0e1b1-1e78-462a-8bee-2390ff89e106.png)
![image](https://user-images.githubusercontent.com/50972168/201422433-e1b87e08-4890-4413-a0b9-bfab43922d8e.png)

### EXIT:
O comando EXIT exclui todas as da base recursivamente e depois excluis as listas/registradores S1 e S1. E então, encerra o programa;

O arquivo entrada preenche as informações e executa os códigos explicados acima;

Criação das listas:  
![image](https://user-images.githubusercontent.com/50972168/201425061-091517ed-2074-4f85-9340-ed4bb988c79b.png)


OBS: O programa, por enquanto(e para sempre), não aceita entradas com espaço.
Criação das listas(# indica referencia a outra linha e $ indica uma lista de referencia a outras linhas);

  
