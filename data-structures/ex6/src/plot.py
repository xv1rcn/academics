import csv
import matplotlib.pyplot as plt
import sys


def read_csv(path):
    ns = []
    times = []
    mems = []
    with open(path) as f:
        r = csv.DictReader(f)
        for row in r:
            try:
                n = int(row["N"])
            except:
                continue
            ns.append(n)
            try:
                times.append(float(row["avg_time_s"]))
            except:
                times.append(float("nan"))
            try:
                mems.append(float(row["peak_rss_MB"]))
            except:
                mems.append(float("nan"))
    return ns, times, mems


def plot_time(ns, times, out="time_vs_n.png"):
    plt.figure(figsize=(8, 5))
    plt.plot(ns, times, marker="o")
    plt.xlabel("Matrix size N")
    plt.ylabel("Average time (s)")
    plt.title("Matrix size vs Time")

    plt.grid(True)
    plt.tight_layout()
    plt.savefig(out, dpi=300)


def plot_mem(ns, mems, out="mem_vs_n.png"):
    plt.figure(figsize=(8, 5))
    plt.plot(ns, mems, marker="o", color="orange")
    plt.xlabel("Matrix size N")
    plt.ylabel("Peak RSS (MB)")
    plt.title("Matrix size vs Memory")

    plt.grid(True)
    plt.tight_layout()
    plt.savefig(out, dpi=300)


def plot_comparison(ns, times, mems, out="comparison.png"):
    fig, ax1 = plt.subplots(figsize=(9, 5))

    ax1.set_xlabel("Matrix size N")
    ax1.set_ylabel("Time (s)", color="tab:blue")
    ax1.plot(ns, times, marker="o", color="tab:blue", label="Time")
    ax1.tick_params(axis="y", labelcolor="tab:blue")

    ax2 = ax1.twinx()
    ax2.set_ylabel("Memory (MB)", color="tab:orange")
    ax2.plot(ns, mems, marker="s", color="tab:orange", label="Memory")
    ax2.tick_params(axis="y", labelcolor="tab:orange")

    ax1.grid(True)
    fig.suptitle("Time and Memory vs Matrix Size")
    fig.tight_layout()
    plt.savefig(out, dpi=300)


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 plot.py results.csv")
        return
    path = sys.argv[1]
    ns, times, mems = read_csv(path)
    if not ns:
        print("No data found in", path)
        return
    plot_time(ns, times)
    plot_mem(ns, mems)
    plot_comparison(ns, times, mems)
    print("Saved: time_vs_n.png, mem_vs_n.png, comparison.png")


if __name__ == "__main__":
    main()
