import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.linear_model import LogisticRegression
import xgboost as xgb
from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score

titanic_data = (pd.read_csv('./titanic_prepared.csv')).drop(["id"], axis=1)
# print(titanic_data.head())

# Handle missing values and encode categorical variables
# titanic_data.fillna(titanic_data.median())
titanic_data = titanic_data.dropna()

# Split the data into train, validation, and test sets
train_data, test_data = train_test_split(titanic_data, test_size=0.1)
train_x = train_data.drop(["label"], axis=1)
train_y = train_data["label"]
test_x = test_data.drop(["label"], axis=1)
test_y = test_data["label"]

# Tune hyperparameters and train models
# Decision Tree
decision_tree = DecisionTreeClassifier(max_depth=10)
decision_tree.fit(train_x, train_y)
decision_tree_predictions = decision_tree.predict(test_x)
accuracy = accuracy_score(decision_tree_predictions, test_y)
precision = precision_score(decision_tree_predictions, test_y)
recall = recall_score(decision_tree_predictions, test_y)
print(f"Decision tree's accuracy is {accuracy}, precision is {precision}, recall is {recall}")

# XGBoost
xgboost = xgb.XGBClassifier(max_depth=3, n_estimators=100)
xgboost.fit(train_x, train_y)
xgboost_predictions = xgboost.predict(test_x)
accuracy = accuracy_score(xgboost_predictions, test_y)
precision = precision_score(xgboost_predictions, test_y)
recall = recall_score(xgboost_predictions, test_y)
print(f"XGBoost's accuracy is {accuracy}, precision is {precision}, recall is {recall}")

# Logistic regression
logistic_regression = LogisticRegression()
logistic_regression.fit(train_x, train_y)
logistic_regression_predictions = decision_tree.predict(test_x)
accuracy = accuracy_score(logistic_regression_predictions, test_y)
precision = precision_score(logistic_regression_predictions, test_y)
recall = recall_score(logistic_regression_predictions, test_y)
print(f"Logistic regression's accuracy is {accuracy}, precision is {precision}, recall is {recall}")

importances = decision_tree.feature_importances_
features = train_x.columns
indices = np.argsort(importances)

plt.title('Важность признаков')
plt.barh(range(len(indices)), importances[indices], color='b', align='center')
plt.yticks(range(len(indices)), features[indices])
plt.xlabel('Относительная важность')
plt.show()

train_x = train_x[[features[indices[-1]], features[indices[-2]]]]
test_x = test_x[[features[indices[-1]], features[indices[-2]]]]

decision_tree = DecisionTreeClassifier(max_depth=10)
decision_tree.fit(train_x, train_y)
decision_tree_predictions = decision_tree.predict(test_x)
accuracy = accuracy_score(decision_tree_predictions, test_y)
precision = precision_score(decision_tree_predictions, test_y)
recall = recall_score(decision_tree_predictions, test_y)
tab = '\t'
print(f"Decision tree's accuracy with two the most important params is {accuracy}, precision is {precision}, recall is {recall}")