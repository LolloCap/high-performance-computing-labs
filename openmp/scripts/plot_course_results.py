from __future__ import annotations

import html
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
INPUT_DIR = ROOT / "results" / "course-runs"
OUTPUT = ROOT / "results" / "figures" / "openmp-thread-sweep.svg"
PATTERN = re.compile(r"Threads:\s*(\d+)\s*\|\s*Tempo medio:\s*([0-9.]+)")


def read_series(path: Path) -> dict[int, float]:
    values: dict[int, float] = {}
    for match in PATTERN.finditer(path.read_text(encoding="utf-8")):
        thread_count = int(match.group(1))
        if thread_count <= 20:
            values[thread_count] = float(match.group(2))
    if set(values) != set(range(1, 21)):
        raise ValueError(f"{path.name} does not contain one value for threads 1-20")
    return values


def polyline(points: list[tuple[float, float]], color: str, width: float, opacity: float) -> str:
    encoded = " ".join(f"{x:.2f},{y:.2f}" for x, y in points)
    return (
        f'<polyline points="{encoded}" fill="none" stroke="{color}" '
        f'stroke-width="{width}" opacity="{opacity}" stroke-linejoin="round"/>'
    )


def main() -> None:
    paths = sorted(INPUT_DIR.glob("thread-sweep-*.txt"))
    if len(paths) != 3:
        raise ValueError("expected exactly three OpenMP thread sweeps")

    series = [read_series(path) for path in paths]
    mean = {
        thread_count: sum(item[thread_count] for item in series) / len(series)
        for thread_count in range(1, 21)
    }

    width, height = 980, 580
    left, right, top, bottom = 92, 42, 72, 82
    plot_width = width - left - right
    plot_height = height - top - bottom
    y_max = 0.08

    def x_pos(thread_count: int) -> float:
        return left + (thread_count - 1) * plot_width / 19

    def y_pos(seconds: float) -> float:
        return top + plot_height * (1 - seconds / y_max)

    svg: list[str] = [
        '<?xml version="1.0" encoding="UTF-8"?>',
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<rect width="100%" height="100%" fill="#ffffff"/>',
        '<style>text{font-family:Segoe UI,Arial,sans-serif;fill:#172033}.small{font-size:13px}.label{font-size:15px}.title{font-size:24px;font-weight:700}.subtitle{font-size:14px;fill:#526078}</style>',
        f'<text x="{left}" y="34" class="title">OpenMP thread sweep</text>',
        f'<text x="{left}" y="56" class="subtitle">Three course runs; lower elapsed time is better</text>',
    ]

    for tick in range(0, 9):
        value = tick * 0.01
        y = y_pos(value)
        svg.append(
            f'<line x1="{left}" y1="{y:.2f}" x2="{width-right}" y2="{y:.2f}" stroke="#dce2ea" stroke-width="1"/>'
        )
        svg.append(
            f'<text x="{left-14}" y="{y+5:.2f}" text-anchor="end" class="small">{value:.2f}</text>'
        )

    for thread_count in range(1, 21):
        x = x_pos(thread_count)
        svg.append(
            f'<line x1="{x:.2f}" y1="{top+plot_height}" x2="{x:.2f}" y2="{top+plot_height+6}" stroke="#172033"/>'
        )
        if thread_count == 1 or thread_count % 2 == 0:
            svg.append(
                f'<text x="{x:.2f}" y="{top+plot_height+25}" text-anchor="middle" class="small">{thread_count}</text>'
            )

    boundaries = [(8, "8 P-cores"), (12, "12 physical cores"), (20, "20 logical threads")]
    for index, (thread_count, label) in enumerate(boundaries):
        x = x_pos(thread_count)
        anchor = "end" if thread_count == 20 else "middle"
        label_x = x - 4 if thread_count == 20 else x
        label_y = top + 18 + (index % 2) * 18
        svg.append(
            f'<line x1="{x:.2f}" y1="{top}" x2="{x:.2f}" y2="{top+plot_height}" stroke="#8793a6" stroke-width="1.2" stroke-dasharray="5 5"/>'
        )
        svg.append(
            f'<text x="{label_x:.2f}" y="{label_y}" text-anchor="{anchor}" class="small" fill="#526078">{html.escape(label)}</text>'
        )

    colors = ["#5b8ff9", "#61b88a", "#e8a33e"]
    for index, values in enumerate(series):
        points = [(x_pos(t), y_pos(values[t])) for t in range(1, 21)]
        svg.append(polyline(points, colors[index], 1.8, 0.72))

    mean_points = [(x_pos(t), y_pos(mean[t])) for t in range(1, 21)]
    svg.append(polyline(mean_points, "#172b4d", 3.5, 1.0))
    for thread_count, seconds in mean.items():
        x, y = x_pos(thread_count), y_pos(seconds)
        svg.append(
            f'<circle cx="{x:.2f}" cy="{y:.2f}" r="3.6" fill="#172b4d"><title>{thread_count} threads: {seconds:.6f} s mean</title></circle>'
        )

    axis_bottom = top + plot_height
    svg.extend(
        [
            f'<line x1="{left}" y1="{axis_bottom}" x2="{width-right}" y2="{axis_bottom}" stroke="#172033" stroke-width="1.5"/>',
            f'<line x1="{left}" y1="{top}" x2="{left}" y2="{axis_bottom}" stroke="#172033" stroke-width="1.5"/>',
            f'<text x="{left + plot_width/2:.2f}" y="{height-28}" text-anchor="middle" class="label">Requested OpenMP threads</text>',
            f'<text x="24" y="{top + plot_height/2:.2f}" text-anchor="middle" class="label" transform="rotate(-90 24 {top + plot_height/2:.2f})">Mean elapsed time per configuration (s)</text>',
        ]
    )

    legend_y = height - 54
    legend_items = [
        (colors[0], "Series 1"),
        (colors[1], "Series 2"),
        (colors[2], "Series 3"),
        ("#172b4d", "Mean"),
    ]
    for index, (color, label) in enumerate(legend_items):
        x = left + 420 + index * 100
        svg.append(f'<line x1="{x}" y1="{legend_y}" x2="{x+24}" y2="{legend_y}" stroke="{color}" stroke-width="3"/>')
        svg.append(f'<text x="{x+31}" y="{legend_y+5}" class="small">{label}</text>')

    svg.append('</svg>')
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_text("\n".join(svg) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
