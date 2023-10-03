import numpy as np
import pandas as pd
import math
import datetime


RESET = '\033[0m'
RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
BOLD = '\033[1m'
# #1ex
print(GREEN + "---------------1st TASK---------------" + RESET)
data = np.random.rand(10, 5)
print(data)
table = pd.DataFrame(data)
# print(table)
mean_gt_03 = table.apply(lambda row: row[row > 0.3].mean(), axis=1)
print(mean_gt_03)
# print(table.mean(axis=1))


#2ex
print(GREEN + "---------------2nd TASK---------------" + RESET)
oil_table = pd.read_csv("./wells_info.csv")
pd.set_option('display.max_rows', None)
pd.set_option('display.max_columns', None)
pd.set_option('display.width', 1000)
pd.set_option('display.colheader_justify', 'center')
pd.set_option('display.precision', 2)
# print(oil_table, "\n\n\n")
pd.to_datetime(oil_table['CompletionDate'])
# print(oil_table)
produce_time = pd.DataFrame({"API": [], "produce_time": []})
# print(produce_time)

oil_table["SpudDate"] = pd.to_datetime(oil_table["SpudDate"])
oil_table["CompletionDate"] = pd.to_datetime(oil_table["CompletionDate"])
produce_time = produce_time.assign(produce_time = ((oil_table["CompletionDate"].dt.year - oil_table["SpudDate"].dt.year) * 12 + (oil_table["CompletionDate"].dt.month - oil_table["SpudDate"].dt.month)))
produce_time = produce_time.assign(API = oil_table["API"])
print(produce_time)

def foo(a):
    print(a)
#3ex
print(GREEN + "---------------3rd TASK---------------" + RESET)
oil_table_gaps = pd.read_csv("./wells_info_na.csv")
most_freq = oil_table_gaps.stack().value_counts().idxmax(skipna=True)
LatWGS84_mean = oil_table_gaps["LatWGS84"].median()
LonWGS84_mean = oil_table_gaps["LonWGS84"].median()
print(oil_table_gaps)
oil_table_gaps["LatWGS84"] = oil_table_gaps["LatWGS84"].apply(lambda x: LatWGS84_mean if math.isnan(x) else x)
oil_table_gaps["LonWGS84"] = oil_table_gaps["LonWGS84"].apply(lambda x: LonWGS84_mean if math.isnan(x) else x)
print(YELLOW + "table with replaced values:" + RESET)
oil_table_gaps = oil_table_gaps.fillna(most_freq)
print(oil_table_gaps)

