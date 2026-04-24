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

        # ép kiểu để tránh sort sai
        self.df["N_Vul"] = self.df["N_Vul"].astype(int)
        self.df["AvgTime"] = self.df["AvgTime"].astype(float)

        # check nhanh
        print("Loaded data:")
        print(self.df.groupby("Algorithm")["N_Vul"].unique())
    

    #Graph 1: Cost Comparison
    def plot_cost(self):
        plt.figure()

        scaled = self.df["AvgCost"] * 1e6

        plt.bar(self.df["Algorithm"], scaled)
        plt.title("Cost Comparison")
        plt.xlabel("Algorithm")
        plt.ylabel("Average Cost (×1e-6)")
        plt.show()

    #Graph 2: Runtime Comparison
    def plot_runtime(self):
        plt.figure()

        scaled = self.df["AvgTime"] * 1e6  # microseconds

        plt.bar(self.df["Algorithm"], scaled)

        plt.yscale("log")  # 🔥 KEY FIX

        plt.title("Run Time Comparison (log scale)")
        plt.xlabel("Algorithm")
        plt.ylabel("Average Run Time (µs, log scale)")
        plt.show()
        
    #Graph 3: Scability Comparision
    def plot_scalability(self):
        plt.figure()

        # đảm bảo dữ liệu sạch
        df = self.df.copy()
        df["N_Vul"] = df["N_Vul"].astype(int)
        df["AvgTime"] = df["AvgTime"].astype(float)

        for algo in sorted(df["Algorithm"].unique()):
            subset = df[df["Algorithm"] == algo].copy()

            # remove duplicate nếu có
            subset = subset.drop_duplicates(subset=["N_Vul"])

            # sort chuẩn numeric
            subset = subset.sort_values(by="N_Vul")

            # DEBUG cực quan trọng
            print(f"{algo}: {subset['N_Vul'].tolist()}")

            plt.plot(
                subset["N_Vul"],
                subset["AvgTime"],
                marker='o',
                linewidth=2,
                label=algo
            )

        plt.yscale("log")
        plt.title("Scalability (log scale)")
        plt.xlabel("Number of Vulnerabilities")
        plt.ylabel("Runtime (seconds, log scale)")
        plt.legend()
        plt.grid(True, which="both", linestyle="--", alpha=0.5)

        plt.tight_layout()
        plt.show()

    #Graph 4: Quality Optimal Comparison
    def plot_quality_optimal(self):
        optimal = self.df[self.df["Algorithm"] == "ILP"]["AvgCost"].min()
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
            "quality": plotter.plot_quality_optimal
        }

        if mode in actions:
            actions[mode]()
        else:
            print("Invalid mode!")

    # SCALABILITY
    else:
        folder = sys.argv[2]

        all_data = []

        for file in sorted(os.listdir(folder)):  # sort để đảm bảo order
            if file.endswith(".csv"):
                path = os.path.join(folder, file)
                df = pd.read_csv(path)

                print(f"\nFILE: {file}")
                print(df)

                # ép kiểu ngay từ đầu
                df["N_Vul"] = df["N_Vul"].astype(int)
                df["AvgTime"] = df["AvgTime"].astype(float)

                all_data.append(df)

        if not all_data:
            print("No CSV files found!")
            sys.exit(1)

        df = pd.concat(all_data, ignore_index=True)

        # DEBUG tổng thể
        print("\n=== FINAL DATA CHECK ===")
        print(df.groupby("Algorithm")["N_Vul"].unique())

        plotter = Plotter(None)
        plotter.df = df

        plotter.plot_scalability()