import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
import time

def RandomForestWithParams(data):
    train_data, test_data = train_test_split(data, test_size=0.15, random_state=42)
    train_x = train_data.drop(["Survived"], axis=1)
    train_y = train_data["Survived"]
    test_x = test_data.drop(["Survived"], axis=1)
    test_y = test_data["Survived"]

    # 3.1. Random Forest
    start = time.time()
    model = RandomForestClassifier(n_estimators=50, max_depth=5)
    model.fit(train_x, train_y)
    rf_exec_time = time.time() - start

    predictions = model.predict(test_x)
    accuracy = accuracy_score(predictions, test_y)
    precision = precision_score(predictions, test_y)
    recall = recall_score(predictions, test_y)
    lines, cols = data.shape
    print(f'Random forest with {cols - 1} parameters accuracy is {accuracy}, precision is {precision}, recall is {recall}')

# Step 1: Load and preprocess the Titanic dataset
# titanic_data = (pd.read_csv('./train.csv')).drop(["Name", "Fare", "Cabin", "Ticket", "Parch", "PassengerId"], axis=1)
titanic_data = (pd.read_csv('./train.csv')).drop(["Name", "Cabin", "Ticket", "PassengerId"], axis=1)
# print(titanic_data.head())

# Handle missing values and encode categorical variables
# titanic_data.fillna(titanic_data.median())
titanic_data = titanic_data.dropna()
# Instead of get_dummies
# label_encoder = LabelEncoder()
# titanic_data['Sex'] = label_encoder.fit_transform(titanic_data['Sex'])
# titanic_data['Embarked'] = label_encoder.fit_transform(titanic_data['Embarked'])
# titanic_data['Pclass'] = label_encoder.fit_transform(titanic_data['Pclass'])
df_num = titanic_data.drop(['Sex', 'Embarked', 'Pclass'], axis=1)
df_sex = pd.get_dummies(titanic_data['Sex'])
df_emb = pd.get_dummies(titanic_data['Embarked'], prefix='Emb')
df_pcl = pd.get_dummies(titanic_data['Pclass'], prefix='Pclass')
titanic_data = pd.concat((df_num, df_sex, df_emb, df_pcl), axis=1)
print(titanic_data.head())

# Step 2: Split the data into train, validation, and test sets
train_data, test_data = train_test_split(titanic_data, test_size=0.15, random_state=42)
train_x = train_data.drop(["Survived"], axis=1)
train_y = train_data["Survived"]
test_x = test_data.drop(["Survived"], axis=1)
test_y = test_data["Survived"]

# 3.1. Random Forest
start = time.time()
model = RandomForestClassifier(n_estimators=50, max_depth=5)
model.fit(train_x, train_y)
rf_exec_time = time.time() - start


importances = model.feature_importances_
features = train_x.columns
indices = np.argsort(importances)

plt.title('Важность признаков')
plt.barh(range(len(indices)), importances[indices], color='b', align='center')
plt.yticks(range(len(indices)), features[indices])
plt.xlabel('Относительная важность')
plt.show()

RandomForestWithParams(titanic_data[["male", "female", "Survived"]])
RandomForestWithParams(titanic_data[["male", "female", "Fare", "Pclass_3", "Survived"]])
RandomForestWithParams(titanic_data[["male", "female", "Fare", "Pclass_3", "Age", "Pclass_1", "SibSp", "Parch", "Survived"]])
RandomForestWithParams(titanic_data)