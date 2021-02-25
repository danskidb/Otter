import pandas as pd
import math
import json

data = pd.read_excel('Skills.xlsx')

jsonObject = {}
for rowIndex, row in data.iterrows():
    rowname = row['name']
    jsonObject[rowname] = {}
    for col in iter(data.columns.values.tolist()):
        if (col == 'effectModifiers' or col == 'effects' or col =='ailmentToInflict') and isinstance(row[col], str):
            jsonObject[rowname][col] = row[col].split(",")
            for i in range(len(jsonObject[rowname][col])):
                jsonObject[rowname][col][i] = jsonObject[rowname][col][i].strip()
        elif row[col] and isinstance(row[col], str):
            jsonObject[rowname][col] = row[col]
        elif row[col] == 0 or (row[col] and not isinstance(row[col], str) and not math.isnan(row[col])):
            jsonObject[rowname][col] = row[col]

with open('Skills.json','w') as file:
    json.dump(jsonObject, file, sort_keys= True, indent=4)