import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
# import matplotlib.pyplot as plt

df = pd.read_csv("./wells_info_with_prod.csv")
df = df.drop(["Name"])
X = df[["CompletionDate", "BottomHoleLatitude", "BottomHoleLongitude"]].copy()
X['CompletionDate'] = pd.to_datetime(X['CompletionDate'])
X['CompletionDate'] = (X['CompletionDate'] - X['CompletionDate'].min()).dt.days
Y = df[["Prod1Year"]].copy()
X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, random_state=42)

scaler = MinMaxScaler()
X_train_scaled = scaler.fit_transform(X_train)
Y_train_scaled = scaler.fit_transform(Y_train.values.reshape(-1, 1))


X_test_scaled = scaler.fit_transform(X_test)
Y_test_scaled = scaler.fit_transform(Y_test.values.reshape(-1, 1))
print(X_test_scaled)
print(Y_test_scaled)