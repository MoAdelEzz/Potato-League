import os


with open('fennec.obj', 'r') as f:
    lines = f.readlines()

x = [2, -2]
y = [2, -2]
z = [2, -2]

for line in lines:
    if not line.startswith("v "): continue


    vertices = line[2 : ]
    vertices = vertices.split(' ')

    if float(vertices[0]) < x[0] : x[0] = float(vertices[0]) # min x
    if float(vertices[0]) > x[1] : x[1] = float(vertices[0]) # max x

    if float(vertices[1]) < y[0] : y[0] = float(vertices[1]) # min x
    if float(vertices[1]) > y[1] : y[1] = float(vertices[1]) # max x

    if float(vertices[2]) < z[0] : z[0] = float(vertices[2]) # min x
    if float(vertices[2]) > z[1] : z[1] = float(vertices[2]) # max x


print(x)
print(y)
print(z)