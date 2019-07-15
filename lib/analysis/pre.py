"""Preliminary analyses
"""

from numpy import loadtxt
from lib.analysis import sim_paths

def mean_volumes(config=None):
    mvs = {}
    for c, sims in sim_paths().items():
        if config and c != config:
            continue

        for path in sims:
            from os import chdir
            from os.path import basename
            import json
            from numpy import loadtxt
            from lib.utils import dir_point

            chdir(path)

            Point = dir_point(basename(path))
            _, volumes = loadtxt('history/volumes.txt', unpack=True)

            with open('state.json', 'r') as state_file:
                state = json.load(state_file)

            try:
                cut = state['cut']
            except KeyError:
                cut = 0

            mvs = {**mvs, Point: volumes[cut:].mean()}

    print(mvs)

def divergent_points(config=None):
    from matplotlib.pyplot import plot, show

    convergent = []
    divergent = []
    for c, sims in sim_paths().items():
        if config and c != config:
            continue

        for path in sims:
            from os import chdir
            from os.path import basename, isfile
            from lib.utils import dir_point

            chdir(path)

            Point = dir_point(basename(path))

            if isfile('max_volume_reached'):
                divergent += [Point]
            else:
                convergent += [Point]

    if divergent:
        plot(*zip(*divergent), 'r+')
    if convergent:
        plot(*zip(*convergent), 'b+')

    show()