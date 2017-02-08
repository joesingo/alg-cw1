import os.path
import sys
import subprocess

import numpy as np
from collections import OrderedDict

from scipy import stats
import matplotlib.pyplot as plt

PROG_PATH = "./sorting_test"

# Change this to whatever you want!
RESULTS_PATH = "/home/joe/uni/comp/Algorithms and Data structures/cw1/results/"

# The min array size, max array size, and number of data points
ARGS = ["1000", "10000", "50"]

INSERTION = "insertion"
COUNTING = "counting"

BEST_CASE = "best"
AVERAGE_CASE = "average"
WORST_CASE = "worst"

ALGS = [INSERTION, COUNTING]
CASES = [BEST_CASE, AVERAGE_CASE, WORST_CASE]

# All valid algorithm/case combinations
ALG_CASE_COMBS = [
    (INSERTION, BEST_CASE), (INSERTION, AVERAGE_CASE), (INSERTION, WORST_CASE),
    (COUNTING, BEST_CASE), (COUNTING, WORST_CASE)
]

# A list of the sitations to show n^2 along the horizontal axis instead of n
QUADRATIC_SCALES = [
    (INSERTION, AVERAGE_CASE), (INSERTION, WORST_CASE), (COUNTING, WORST_CASE)
]

"""
Call the sorting_test program for each valid algorithm/case combination and save
the results as a CSV in the location specified
"""
def collect_data(filename):
    print("Saving results to {}".format(filename))

    with open(filename, "w") as f:

        for alg, case in ALG_CASE_COMBS:
            args = [PROG_PATH, alg, case] + ARGS

            if (alg, case) in QUADRATIC_SCALES:
                args.append("-q")

            print("Calling {}".format(" ".join(args)))

            f.write("{},{}\n".format(alg, case))
            f.flush()
            subprocess.call(args, stdout=f)

"""
Parse a CSV file and return timing data in a dictionary. The keys are tuples of
the form (alg, case). The values are OrderedDicts where the keys are the array
sizes and values are average time taken in seconds
"""
def parse_csv(filename):
    timings = {}

    with open(filename) as results:

        alg = None
        case = None

        for line in results:

            line = line.strip()

            # Skip blank lines
            if not line:
                continue

            try:
                a, b = line.split(",")

                if (a, b) in ALG_CASE_COMBS:
                    alg = a
                    case = b

                # If an alg and case were not read and are not currently set, print
                # error and exit
                elif alg is None or case is None:
                    print("Malformed CSV - invalid algorithm or case")
                    sys.exit(1)

                # Otherwise it must be data
                else:
                    try:
                        size = int(a)
                        time = float(b)

                    except ValueError:
                        print("Invalid size/time")
                        sys.exit(1)

                    i = (alg, case)

                    if i not in timings:
                        timings[i] = OrderedDict()

                    timings[i][size] = time

            except ValueError:
                print("Malformed CSV")
                sys.exit(1)

    return timings

# If filename has been specified on command line then use that file
if len(sys.argv) > 1:
    filename = sys.argv[1]

# Otherwise run the program to gather results
else:
    filename = os.path.join(RESULTS_PATH, "results.csv")
    collect_data(filename)

timings = parse_csv(filename)

# Create LaTeX tables and graphs
for (alg, case), data in timings.items():

    case_display_name = case
    desc = "case" if alg == INSERTION else "performance"

    if alg == COUNTING:
        case_display_name = ("good" if case == BEST_CASE else "bad")

    base_filename = os.path.join(RESULTS_PATH, "{}-{}".format(alg, case_display_name))

    # Write table to .tex file
    table_filename = base_filename + ".tex"

    with open(table_filename, "w") as f:
        print("Writing table to {}".format(table_filename))

        f.write("\\begin{tabular}{c | c}\n")
        f.write("\\textbf{Array size} & \\textbf{Average run-time (seconds)} "
                "\\\ \\hline\n")

        for n in data:
            f.write("{} & {} \\\ \n".format(n, data[n]))

        f.write("\\end{tabular}\n\n")

    ns = np.array(data.keys())
    ys = np.array(data.values())

    if (alg, case) in QUADRATIC_SCALES:
        label = r"$n^2$"
        ns = np.square(ns)

    else:
        label = r"$n$"

    # Set up plot layout
    fig, ax = plt.subplots(figsize=(16, 9))

    ax.grid(True)
    plt.title("{} sort - {} {}".format(alg.title(), case_display_name.title(),
                                       desc),
              fontsize=20, y=1.04)

    plt.xlabel(label, fontsize=24)
    plt.ylabel("Average time (seconds)", fontsize=15)
    ax.grid(True)

    slope, intercept, r_value, p_value, std_err = stats.linregress(ns, ys)

    scatter = plt.plot(ns, ys, "or", markersize=8)
    plt.plot(ns, slope * ns + intercept)

    plot_filename = base_filename + ".png"
    print("Saving plot to {}".format(plot_filename))
    plt.savefig(plot_filename)
