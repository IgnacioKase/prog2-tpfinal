#!/usr/bin/env python
# -*- coding: utf-8 -*-
import subprocess
from functools import reduce


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

def __main(args, kargs):
    response = subprocess.check_output(["./a.exe", "in/test1.txt"])
    maze = response.decode('utf-8').strip().split('\n')

    graph = [row.split() for row in maze]

    iniPos = (0, 0)
    endPos = (0, 0)

    for i, row in enumerate(graph):
        if 'I' in row:
            iniPos = (i, row.index('I'))
        if 'X' in row:
            endPos = (i, row.index('X'))

    path = BFS(graph, iniPos, endPos)

    if len(path) == 0:
        pass  # we have to repeat the maze generate process
    else:
        output = reduce(lambda x, y: x + y,
                        map(lambda v: "(%s, %s), " % (v[1] + 1, v[0] + 1), path))
        output = output.strip()[:-1]
        print(output)

######### FUNCIONES AUXILIARES #########


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
    f = open("test/checkBoundaries.txt", "r")
    line = f.readline()
    while line:
        vertex, n = line.split()
        vertex = tuple(vertex)
        assert checkBoundaries(
            (int(vertex[0]), int(vertex[1])), int(n)) == bool(f.readline())
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
    assert checkBoundaries((1, 2), 3) == True
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
    visited[vertex[0]][vertex[1]] = True
    while queue:
        v = queue.pop(0)
        neighbors = getNeighbors(v, n)
        for u in neighbors:
            if visited[u[0]][u[1]] == False and graph[u[0]][u[1]] != '1':
                queue.append(u)
                parent[u[0]][u[1]] = v
                visited[u[0]][u[1]] = True

    v = parent[end[0]][end[1]]
    output = []
    while v != (-1, -1):
        output.append(v)
        v = parent[v[0]][v[1]]

    return output


def test_BFS():
    assert checkBoundaries((1, 2), 3) == True
#########################################


__main(False, False)
