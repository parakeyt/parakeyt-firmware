#!/usr/bin/python3
from dataclasses import dataclass, field
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import (
    LinearNDInterpolator,
    CloughTocher2DInterpolator,
    RBFInterpolator,
)
from mpl_toolkits.mplot3d import Axes3D
from scipy.spatial import cKDTree

# parameters
ROWS = 16
COLS = 16


# data
@dataclass
class PollData:
    rows: int
    cols: int
    polltime: float


real_data = [
    PollData(1, 1, 0.103501 / 1000),
    PollData(1, 4, 0.301773 / 1000),
    PollData(1, 8, 0.569391 / 1000),
    PollData(2, 8, 1.140830 / 1000),
    PollData(3, 8, 1.705199 / 1000),
    PollData(4, 4, 1.206637 / 1000),
    PollData(4, 1, 0.412839 / 1000),
    PollData(5, 2, 0.851517 / 1000),
    PollData(5, 8, 2.825956 / 1000),
    PollData(7, 4, 2.121931 / 1000),
    PollData(8, 1, 0.829250 / 1000),
    PollData(8, 5, 2.957508 / 1000),
    PollData(8, 8, 4.519664 / 1000),
]

points = np.array([[d.rows, d.cols] for d in real_data])
values = np.array([d.polltime for d in real_data])

row_range = np.arange(1, ROWS + 1)
col_range = np.arange(1, COLS + 1)
X, Y = np.meshgrid(row_range, col_range)

interp = RBFInterpolator(points, values)
temp = []
for i in range(1, ROWS + 1):
    for j in range(1, COLS + 1):
        temp.append([i, j])

t2rate = np.vectorize(lambda x: 1 / x)
Z = t2rate(interp(temp)).reshape(ROWS, COLS).clip(0, 1000)

tree = cKDTree(points)

nan_mask = np.isnan(Z)
if np.any(nan_mask):
    query_points = np.column_stack((X[nan_mask], Y[nan_mask]))
    _, idx = tree.query(query_points)
    Z[nan_mask] = values[idx]


fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection="3d")

xpos = X.ravel()
ypos = Y.ravel()
zpos = np.zeros_like(xpos)

dx = dy = 0.8
dz = Z.ravel()

norm = plt.Normalize(dz.min(), dz.max())
colors = plt.cm.viridis(norm(dz))

ax.bar3d(xpos, ypos, zpos, dx, dy, dz, color=colors, shade=True)

ax.set_xlabel("Rows")
ax.set_ylabel("Colums")
ax.set_zlabel("Polling Rate (Hz)")
ax.set_zlim(0, 1000)

ax.set_title(
    "Projected Parakeyt Polling Rate Scaling on Rectangular Layout Without Row or i2c Optimization (1000Hz cap)"
)

plt.tight_layout()
plt.show()
