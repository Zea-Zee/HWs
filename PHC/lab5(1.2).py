import numpy as np
import pandas as pd
import math


pd.set_option('display.max_rows', None)
pd.set_option('display.max_columns', None)
pd.set_option('display.width', 1000)
pd.set_option('display.colheader_justify', 'left')
pd.set_option('display.precision', 2)


#5.1
titanic_with_labels = pd.read_csv("./titanic_with_labels.csv", sep=' ', index_col=0)
# print(titanic_with_labels)
res = titanic_with_labels.copy()
# print(res)
mask = (res == '-').any(axis=1)
res = res[~mask]
mask = (res == "Не указан").any(axis=1)
res = res[~mask]
res = res.replace(to_replace=['M','М', 'м', 'Ж', 'ж'], value=[1,1,1,0,0])
max_row = res["row_number"].max()
# print(res)
res["row_number"] = res["row_number"].apply(lambda x: max_row if math.isnan(x) else x)
# mask = res["liters_drunk"] >= 0
# res = res[mask]
# mask = res["liters_drunk"] < 10
# res = res[mask]
mean_liters_drunk = int(res["liters_drunk"].median())
res["liters_drunk"] = res["liters_drunk"].apply(lambda x: x if 0 <= x < 10 else mean_liters_drunk)
# print(res)
# res = res[~]
# print(res)


#5.2
cond = res["age"] < 18
res["age_young"] = np.where(cond, True, False)
# cond = (18 <= res["age"] < 65)    #we live in Russia
cond = res["age"] >= 18
cond2 = res["age"] < 65    #we live in Russia
res["age_adult"] = np.where(cond & cond2, True, False)
cond = res["age"] >= 50
res["age_old"] = np.where(cond, True, False)
res.drop("age", axis=1)


res["drink"] = res["drink"].replace(to_replace=["Water", "Cola", "Fanta", "Beer", "Beerbeer", "Strong beer", "Bugbeer"], value=[0,0,0,1,1,1,1])
# print(res)

cinema_sessions = pd.read_csv("./cinema_sessions.csv", sep=' ', index_col=0)
# print(res["check_number"].compare(cinema_sessions["check_number"]))
res = res.merge(cinema_sessions, how="left", on="check_number")
res["session_start"] = pd.to_datetime(res["session_start"])
cond = res["session_start"].dt.hour < 12
res["morning"] = np.where((res["session_start"].dt.hour < 12), True, False)
res["day"] = np.where(((12 <= res["session_start"].dt.hour) & (res["session_start"].dt.hour < 18)), True, False)
res["evening"] = np.where((res["session_start"].dt.hour > 18), True, False)
# print(res["session_start"].dt.hour)
print(res)