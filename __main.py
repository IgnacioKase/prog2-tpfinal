#!/usr/bin/env python
# -*- coding: utf-8 -*-
import subprocess
from functools import reduce
import sys
from ast import literal_eval as make_tuple


#
# TRABAJO PRÁCTICO FINAL
# GENERACION/SOLUCION Laberinto C/Python
#
# ALUMNO:
#       - KASEVICH, IGNACIO
#
# GIT: https://github.com/IgnacioKase/prog2-tpfinal
#
# 28/11/2019
#

######### FUNCION PRINCIPAL #########

#
#  __main__
# @Parametros: args[String], kargs[list]
# @Salida: String (por consola no retorna la función)
#
# Recibe un archivo, y llama a __main.c para generar
# un laberinto a partir de este, buscará el camino más
# corto entre el punto 'I' y el punto 'X'. En caso de
# no existir dicho camino, generará otro laberinto.
# Retorna el camino mas corto de 'X' a 'I'
#

def main():
    graph, iniPos, endPos = generateMaze()
    path = []
    PATH_OUT = sys.argv[4]
    foutput = open(PATH_OUT, "w+", encoding="utf-8")
    solved = False
    while not solved and len(graph) > 0:
        path = BFS(graph, iniPos, endPos)
        solved = len(path) > 0
        if not solved:
            graph, iniPos, endPos = generateMaze()

    output = reduce(lambda x, y: x + y,
                    map(lambda v: "(%s, %s), " % (v[1] + 1, v[0] + 1), path)).strip()[:-1]
    foutput.write(output)


######### FUNCIONES AUXILIARES #########

#
# generateMaze:
# @Parametros: -
# @Salida: tuple(graph[List[string]], pair/tuple, pair/tuple)
#
# Ejecuta el programa en C que genera el laberinto
# y luego lee el archivo de salida del mismo. Y
# arma el grafo correspondiente (lista de strings)
# e identifica la posicion de inicio y de llegada
# en el mismo.
#

def generateMaze():
    PATH_C = sys.argv[1]
    PATH_IN = sys.argv[2]
    PATH_MAZE = sys.argv[3]

    response = subprocess.run([PATH_C, PATH_IN, PATH_MAZE])

    if response.returncode != 0:
        print("Error al generar el laberinto, mas información en:\tlog.txt")
        return []

    iniPos = endPos = (0, 0)
    iniFound = False
    endFound = False
    graph = []
    i = 0

    fMaze = open(PATH_MAZE, "r", encoding="utf-8")

    line = fMaze.readline()

    while line:
        graph.append(line.strip())
        if not iniFound:
            if 'I' in graph[-1]:
                iniPos = (i, int(graph[-1].index('I') / 2))
                iniFound = True
        if not endFound:
            if 'X' in graph[-1]:
                endPos = (i, int(graph[-1].index('X')/2))
                endFound = True
        i += 1
        line = fMaze.readline()
    fMaze.close()
    return (graph, iniPos, endPos)
#########################################


#
# getTest:
# @Parametros: filePath[string]
# @Salida: List[pair/tuple]
#
# Toma la ruta de un archivo y lee los casos de
# test en ellos siendo el formato:
# -Primera linea input
# -Segunda linea output esperado
# Es decir que el archivo deberá tener una cantidad
# de lineas pares.
#

def getTest(filePath):
    file = open(filePath, "r", encoding="utf-8")
    line1 = file.readline()
    line2 = file.readline()
    tests = [(line1.strip(), line2.strip())]
    while line1 and line2:
        line1 = file.readline()
        line2 = file.readline()
        if line1 and line2:
            tests.append((line1.strip(), line2.strip()))
    file.close()
    return tests
#########################################


#
# checkBoundaries:
# @Parametros: vertex[pair/tuple], n[int]
# @Salida: Bool
#
# Toma las coordenadas (x,y) de un punto
# y checkea que esten contenidas entre 0
# y n, [0,n).
#

def checkBoundaries(vertex, n):
    return vertex[0] >= 0 and vertex[1] >= 0 and vertex[0] < n and vertex[1] < n


def test_checkBoundaries():
    tests = getTest("test/checkBoundaries.txt")
    for test in tests:
        in_test = test[0].split()
        vertex = make_tuple(in_test[0])
        n = int(in_test[1])
        out = test[1] == 'True'
        assert checkBoundaries(vertex, n) == out
#########################################


#
# posNeighbors:
# @Parametros: vertex[pair/tuple], n[int]
# @Salida: list[pair/tuple]
#
# Toma las coordenadas (x,y) de un punto
# en una matriz, que representa un grafo
# de estilo 'tablero' y devuelve las
# coordenadas de los vecinos, checkeando
# unicamente que estos existan en los
# limites de la matriz.
#

def getNeighbors(vertex, n):
    posNeighbors = [
        (vertex[0] + 1, vertex[1]),
        (vertex[0] - 1, vertex[1]),
        (vertex[0], vertex[1] + 1),
        (vertex[0], vertex[1] - 1)
    ]
    return list(filter(lambda x: checkBoundaries(x, n), posNeighbors))


def test_getNeighbors():
    tests = getTest("test/getNeighbors.txt")
    for test in tests:
        in_test = test[0].split()
        vertex = make_tuple(in_test[0])
        n = int(in_test[1])
        out = test[1][1:-1].split(';')
        neighbors = set([make_tuple(v) for v in out])
        response = set(getNeighbors(vertex, n))
        assert neighbors == response
#########################################


#
# BFS:
# @Parametros: graph[list[list[pair/tuple]],
#              vertex[pair/tuple],
#              end[pair/tuple]
# @Salida: list[pair/tuple]
#
# Toma un grafo, representando por una lista
# de listas, de estilo 'tablero', las coordenadas
# de un vértice, y las coordenadas de un objetivo
# realiza la busqueda en anchura del grafo con
# @vertex de inicio/raíz, y guarda el camino
# mas corto para los demás nodos. Devuelve
# el camino mas corto de  @vertex a @end.
#

def BFS(graph, vertex, end):
    n = (len(graph))  # length of the maze (n x n maze)
    visited = [[False] * n for i in range(n)]
    parent = [[(-1, -1)] * n for i in range(n)]
    queue = []
    queue.append(vertex)
    found = False
    visited[vertex[0]][vertex[1]] = True
    while queue and not found:
        v = queue.pop(0)
        neighbors = getNeighbors(v, n)
        for u in neighbors:
            if visited[u[0]][u[1]] == False and graph[u[0]][u[1]] != '1':
                queue.append(u)
                found = graph[u[0]][u[1]] == 'X' or found
                parent[u[0]][u[1]] = v
                visited[u[0]][u[1]] = True

    v = parent[end[0]][end[1]]
    output = []
    while v != (-1, -1):
        output.append(v)
        v = parent[v[0]][v[1]]
    output.reverse()
    if len(output) > 0:
        output.append(end)
    return output


def test_BFS():
    tests = getTest("test/BFS.txt")
    for test in tests:
        in_test = test[0].split()
        graph = in_test[0][1:-1].split(';')
        iniPos = make_tuple(in_test[1])
        endPos = make_tuple(in_test[2])
        out = []
        if test[1] != "[]":
            out = test[1][1:-1].split(';')
        path = []
        path = [make_tuple(v) for v in out]

        assert BFS(graph, iniPos, endPos) == path
#########################################


if __name__ == "__main__":
    main()
