from math import pi, sin, exp, sqrt
import numpy as np

u = list(np.loadtxt("out.txt", skiprows=1)[:, 0]) # U
x = list(np.loadtxt("out.txt", skiprows=1)[:, 1]) # X
y = list(np.loadtxt("out.txt", skiprows=1)[:, 2]) # Y
z = list(np.loadtxt("out.txt", skiprows=1)[:, 3]) # Z

with open("out.txt") as f:
    cells = f.readline().split()

Nx = int(cells[0]) - 1
Ny = int(cells[1]) - 1
Nz = int(cells[2]) - 1

hx = 1 / Nx
hy = 1 / Ny
hz = 1 / Nz

u_an = []
diff = []
diff_sum = 0

# Give a list of analytical solutions
for i in range(0, len(u)):
    n = sin(pi * x[i] * hx) * sin(pi * y[i] * hy) * sin(pi * z[i] * hz) * (1 - exp(-0.5 * pi*pi))
    u_an.append(n)

# Give a list of difference between analytical solutions and list of U
for i in range(0, len(u)):
    diff.append(abs(u_an[i] - u[i]))

#Sum of all differences
for i in range(0, len(diff)):
    diff_sum += diff[i]

#standard deviation
dev = sqrt(diff_sum * diff_sum / len(diff))

print(max(diff))
print(diff_sum)
print(round(dev,2))
