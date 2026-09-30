from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


# Paths
ROOT_DIR = Path(__file__).resolve().parent
RESULTS_DIR = ROOT_DIR / "SortCritic" / "results"
PLOTS_DIR = RESULTS_DIR / "plots"


# Benchmark configuration
DISTRIBUTIONS = [
    "Random",
    "Sorted",
    "Reversed",
]

ALGORITHMS = [
    "Bubble Sort",
    "Insertion Sort",
    "Selection Sort",
    "Merge Sort",
    "Quick Sort",
]


def load_results() -> pd.DataFrame:
    csv_files = sorted(RESULTS_DIR.glob("benchmark_*.csv"))

    if not csv_files:
        raise FileNotFoundError(
            f"No benchmark CSV files found in: {RESULTS_DIR}"
        )

    frames = []

    for csv_file in csv_files:
        frame = pd.read_csv(csv_file)
        frames.append(frame)

    results = pd.concat(frames, ignore_index=True)
    return results


def validate_results(results: pd.DataFrame) -> None:
    required_columns = {
        "algorithm",
        "distribution",
        "seed",
        "inputSize",
        "run",
        "durationMs",
        "success",
    }

    missing_columns = required_columns - set(results.columns)

    if missing_columns:
        raise ValueError(
            f"Missing CSV columns: {sorted(missing_columns)}"
        )

    failed_results = results[results["success"] != True]

    if not failed_results.empty:
        raise ValueError(
            f"Found {len(failed_results)} failed benchmark runs."
        )


def calculate_summary(results: pd.DataFrame) -> pd.DataFrame:
    summary = (
        results
        .groupby(
            ["distribution", "algorithm", "inputSize"],
            as_index=False
        )["durationMs"]
        .mean()
        .rename(columns={"durationMs": "meanDurationMs"})
    )

    return summary


def plot_distribution(
    summary: pd.DataFrame,
    distribution: str,
    use_log_scale: bool
) -> None:
    distribution_data = summary[
        summary["distribution"] == distribution
    ]

    if distribution_data.empty:
        print(f"Skipped: {distribution}")
        return

    plt.figure(figsize=(10, 6))

    for algorithm in ALGORITHMS:
        algorithm_data = distribution_data[
            distribution_data["algorithm"] == algorithm
        ].sort_values("inputSize")

        if algorithm_data.empty:
            print(
                f"Warning: no data for "
                f"{algorithm} / {distribution}"
            )
            continue

        plt.plot(
            algorithm_data["inputSize"],
            algorithm_data["meanDurationMs"],
            marker="o",
            label=algorithm
        )

    scale_name = "Log Scale" if use_log_scale else "Linear Scale"

    plt.title(
        f"SortCritic Benchmark - {distribution} ({scale_name})"
    )
    plt.xlabel("Input Size")
    plt.ylabel("Mean Duration (ms)")

    if use_log_scale:
        plt.yscale("log")

    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    suffix = "log" if use_log_scale else "linear"

    output_path = (
        PLOTS_DIR
        / f"benchmark_{distribution.lower()}_{suffix}.png"
    )

    plt.savefig(output_path, dpi=150)
    plt.close()

    print(f"Saved: {output_path}")


def main() -> None:
    PLOTS_DIR.mkdir(parents=True, exist_ok=True)

    results = load_results()
    validate_results(results)
    summary = calculate_summary(results)

    for distribution in DISTRIBUTIONS:
        plot_distribution(summary, distribution, use_log_scale=False)
        plot_distribution(summary, distribution, use_log_scale=True)


if __name__ == "__main__":
    main()