import sys
import os
import pandas as pd
import matplotlib.pyplot as plt

class Plotter:
    def __init__(self, file_path):
        self.file_path = file_path
        self.df = None
    
    def load_data(self):
        self.df = pd.read_csv(self.file_path)
    

    #Graph 1: Cost Comparison
    def plot_cost(self):
        plt.figure()
        plt.bar(self.df["Algorithm"], self.df["AvgCost"])
        plt.title("Cost Comparison")
        plt.xlabel("Algorithm")
        plt.ylabel("Average Cost")
        plt.show()

    #Graph 2: Runtime Comparison
    def plot_runtime(self):
        plt.figure()
        plt.bar(self.df["Algorithm"], self.df["AvgTime"])
        plt.title("Run Time Comparision")
        plt.xlabel("Algorithm")
        plt.ylabel("Average Run Time")
        plt.show()

    #Graph 3: Scability Comparision
    def plot_scability(self):
        plt.figure()

        for algo in self.df["Algorithm"].unique():
            subset = self.df[self.df["Algorithm"] == algo]

            # sort để line không bị zigzag
            subset = subset.sort_values(by="N_Vul")

            plt.plot(
                subset["N_Vul"],
                subset["AvgTime"],
                marker='o',
                label=algo
            )

        plt.title("Scalability")
        plt.xlabel("Number of Vulnerabilities")
        plt.ylabel("Runtime")
        plt.legend()
        plt.grid(True)
        plt.show()

    #Graph 4: Quality Optimal Comparison
    def plot_quality_optimal(self):
        optimal = self.df[self.df["Algorithm"] == "ILP"]["AvgCost"].values[0]
        self.df["Ratio"] = self.df["AvgCost"] / optimal

        plt.figure()
        plt.bar(self.df["Algorithm"], self.df["Ratio"])
        plt.title("Quality versus Optimal")
        plt.xlabel("Algorithm")
        plt.ylabel("Cost Ratio")
        plt.show()


if __name__ == "__main__": 
    if len(sys.argv) < 3:
            print("Usage:")
            print("  python plot.py <mode> <file>")
            print("  python plot.py scalability <folder>")
            sys.exit(1)

    mode = sys.argv[1]

    # CASE NORMAL GRAPH
    if mode != "scalability":
        file_path = sys.argv[2]

        plotter = Plotter(file_path)
        plotter.load_data()

        actions = {
            "cost": plotter.plot_cost,
            "runtime": plotter.plot_runtime,
            "quality": plotter.plot_quality
        }

        if mode in actions:
            actions[mode]()
        else:
            print("Invalid mode!")

    # SCALABILITY
    else:
        folder = sys.argv[2]

        all_data = []

        for file in os.listdir(folder):
            if file.endswith(".csv"):
                path = os.path.join(folder, file)
                df = pd.read_csv(path)
                all_data.append(df)

        if not all_data:
            print("No CSV files found!")
            sys.exit(1)

        df = pd.concat(all_data, ignore_index=True)

        plotter = Plotter(None)
        plotter.df = df   # inject data trực tiếp

        plotter.plot_scalability()