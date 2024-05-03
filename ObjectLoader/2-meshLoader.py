import json
import os

files = os.listdir("./output/")
meshes = [os.path.splitext(file)[0] for file in files]



data_line = open("app.jsonc","r")
data_line = data_line.readlines()
lines = 1
with open('app_new.jsonc', 'w+') as new_file:
    new_file.write("")
    cnt1 = 1
    cnt2 = 1
    for i in range(0,len(data_line)):
        line = data_line[i]
        words = line.split()
        if line.startswith("//"):
            new_file.write("")
        else:
            split_result  = line.split('//', 1)
            new_file.write(split_result[0])
    new_file.write("\n")

with open('app_new.jsonc', 'r') as f:
    data = json.load(f)

mesh = data["scene"]["assets"]["meshes"]

for meshName in meshes:
    mesh[meshName]="assets/models/"+meshName+".obj"

data["scene"]["assets"]["meshes"] = mesh         
            

with open('app.jsonc', 'w') as f:
    json.dump(data, f, indent=4)
