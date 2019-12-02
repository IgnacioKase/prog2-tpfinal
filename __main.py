import subprocess
from functools import reduce


def check_boundaries(vertex, n):
    return vertex[0] >= 0 and vertex[1] >= 0 and vertex[0] < n and vertex[1] < n


def getNeighbors(vertex, n):
    posNeighbors = [
        (vertex[0] + 1, vertex[1]),
        (vertex[0] - 1, vertex[1]),
        (vertex[0], vertex[1] + 1),
        (vertex[0], vertex[1] - 1)
    ]
    return list(filter(lambda x: check_boundaries(x, n), posNeighbors))


def bfs(graph, vertex, end):
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


def __main(args, kargs):
    response = subprocess.check_output(["./a.exe", "in/test1.txt"])

    maze = '''I 1 0 0 0 0 0
    0 0 1 0 0 0 0
    0 1 0 1 1 0 0
    0 1 0 1 X 1 0
    0 0 0 0 0 0 0
    0 0 0 0 0 0 0
    1 0 0 0 1 0 0'''.split('\n')

    graph = [row.split() for row in maze]

    iniPos = (0, 0)
    endPos = (0, 0)

    for i, row in enumerate(graph):
        if 'I' in row:
            iniPos = (i, row.index('I'))
        if 'X' in row:
            endPos = (i, row.index('X'))

    path = bfs(graph, iniPos, endPos)

    if len(path) == 0:
        pass  # we have to repeat the maze generate process
    else:
        output = reduce(lambda x, y: x + y,
                        map(lambda v: "(%s, %s), " % (v[1] + 1, v[0] + 1), path))
        output = output.strip()[:-1]
        print(output)


__main(False, False)
