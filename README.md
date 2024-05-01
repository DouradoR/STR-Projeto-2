# STR-Projeto-2
Executivo cíclico

## **Objetivo**
O executivo cíclico é uma técnica comumente utilizada para coordenar a execução de tarefas periódicas, garantindo a sincronização entre elas e otimizando o uso dos recursos do sistema.
O objetivo principal deste projeto é fornecer uma implementação prática de um executivo cíclico em linguagem C, abordando os seguintes aspectos:
Cálculo dos Tempos de Ciclo:
- Determinação do Tempo de Ciclo Primário, representando o ciclo maior que sincroniza todas as tarefas em sua periodicidade.
- Cálculo do Tempo de Ciclo Secundário, representando o menor ciclo possível que ainda respeita as periodicidades das tarefas.
Aplicação de Heurísticas de Escalonamento:
- Utilização de heurísticas como Menor Tempo de Execução Primeiro ou Maior Taxa de Periodicidade Primeiro para sugerir um escalonamento que otimize a execução das tarefas dentro dos ciclos determinados.

## **Implementação**
O sistema compreende as seguintes etapas:
Leitura de dados de entrada: Os detalhes das tarefas a serem escalonadas são lidos a partir de um arquivo JSON.
Cálculo dos tempos de ciclo: Os tempos de ciclo primário e secundário são calculados com base nos períodos das tarefas.
Ordenação das tarefas: As tarefas são ordenadas com base no menor tempo de execução, conforme a heurística SETF.
Escalonamento das tarefas: O escalonamento das tarefas é realizado dentro dos ciclos primários, seguindo a ordem definida pela heurística SETF.

## **Código**
Estruturamos o código
