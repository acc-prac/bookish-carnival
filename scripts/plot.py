#!python

import sys
from pathlib import Path
from itertools import islice, cycle
import pandas as pd
from cycler import cycler
import matplotlib.pyplot as plt
import matplotlib as mpl

def better_mpl_line_style_cycle(num_styles = 10):
    prop_cycle = plt.rcParams['axes.prop_cycle']
    colors = list(islice(cycle(prop_cycle.by_key()['color']), num_styles))
    linestyles = list(islice(cycle(['-', '--', ':', '-.']), num_styles))
    plt.rc('axes', prop_cycle=(cycler('color', colors) +
                               cycler('linestyle', linestyles)))

def main(argv):
    # Some defaults I like
    mpl.style.use(Path(__file__).resolve().parent / 'mystyle.mplstyle')
    # make overlapping lines easier to see
    better_mpl_line_style_cycle()

    # read data, skip context information
    df = pd.read_csv(argv[0], skiprows=9)

    # get additional columns from benchmark "name"
    df[['name', 'n_elements']] = df['name'].str.split(pat='/', expand=True)
    # needed when stats from benchmark repetitions are reported
    df[['n_elements', 'stat']] = df['n_elements'].str.split(pat='_', expand=True)
    # cast numeral strings to ints
    df['n_elements'] = df['n_elements'].astype(int)

    # preserve time_unit for plot labels
    # (assume that it does not differ between rows)
    time_unit = df['time_unit'][0]

    # remove warmup row(s)
    df = df[df['name'] != '_warmup_'] 

    # print(df)

    # determine statistic(s) to plot
    df_mean = df[df['stat'] == 'mean']
    df_stddev = df[df['stat'] == 'stddev']

    # pivot data into a form that is nice to plot
    df_mean = df_mean.pivot(index='n_elements', columns='name', values='real_time')
    df_stddev = df_stddev.pivot(index='n_elements', columns='name', values='real_time')

    print(df_mean)
    print(df_stddev)

    # plot real time vs n_elements (log-log-plot)
    fig, ax = plt.subplots()
    df_mean.plot(yerr=df_stddev, ax=ax)
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_ylabel(f'real time [{time_unit}]')

    if len(argv) >= 2:
        fig.savefig(argv[1])

    # plot real times for highest n_elements as bar plot (linear y axis)
    fig, ax = plt.subplots()
    df_mean.loc[df_mean.index.max()].plot(kind='bar', yerr=df_stddev.loc[df_stddev.index.max()], ax=ax)
    ax.set_ylabel(f'real time [{time_unit}]')
    # Labels (benchmark names) are often too big and therefore overlap and/or overflow the canvas.
    # You can use rotation and size to get better results
    ax.tick_params(axis='x', labelrotation=0., labelsize=10.)

    # Put newline in front of "<...>"-part of benchmark name for more efficient use of space.
    labels = ax.get_xticklabels()
    for label in labels:
        label.set_text('\n<'.join(label.get_text().rsplit('<')))
    ax.set_xticklabels(labels)

    if len(argv) >= 3:
        fig.savefig(argv[2])
    else:
        plt.show()

if __name__ == "__main__":
    main(sys.argv[1:])
