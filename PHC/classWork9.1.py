import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from sklearn.ensemble import RandomForestClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.neighbors import KNeighborsClassifier
import xgboost as xgb
from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_score
from sklearn.model_selection import GridSearchCV
import time

titanic_data = (pd.read_csv('./train.csv')).drop(["Name", "Cabin", "Ticket", "PassengerId"], axis=1)
# print(titanic_data.head())

# Handle missing values and encode categorical variables
# titanic_data.fillna(titanic_data.median())
titanic_data = titanic_data.dropna()
# Instead of get_dummies
label_encoder = LabelEncoder()
titanic_data['Sex'] = label_encoder.fit_transform(titanic_data['Sex'])
titanic_data['Embarked'] = label_encoder.fit_transform(titanic_data['Embarked'])
titanic_data['Pclass'] = label_encoder.fit_transform(titanic_data['Pclass'])

# Split the data into train, validation, and test sets
train_data, test_data = train_test_split(titanic_data, test_size=0.3, random_state=42)
validation_data, test_data = train_test_split(test_data, test_size=0.5, random_state=42)

# Tune hyperparameters and train models
# Random Forest
start = time.time()
# rf_params = {'n_estimators': [50, 100, 200, 300, 500, 700], 'max_depth': [5, 10, 20, 30]} #Random Forest Accuracy: 0.8037383177570093 Precision: 0.8717948717948718
rf_params = {'n_estimators': [50], 'max_depth': [5]}
rf_classifier = RandomForestClassifier(random_state=42)
rf_grid_search = GridSearchCV(rf_classifier, rf_params, cv=5)
rf_grid_search.fit(train_data.drop(['Survived'], axis=1), train_data['Survived'])
rf_exec_time = time.time() - start

# Logistic Regression
start = time.time()
lr_params = {'C': [0.001, 0.01, 0.1, 1, 10, 100]}
lr_classifier = LogisticRegression(max_iter=1000)
lr_grid_search = GridSearchCV(lr_classifier, lr_params, cv=5)
lr_grid_search.fit(train_data.drop(['Survived'], axis=1), train_data['Survived'])
lr_exec_time = time.time() - start
#
# K-Nearest Neighbors
start = time.time()
knn_params = {'n_neighbors': [3, 5, 7, 9, 11]}
knn_classifier = KNeighborsClassifier()
knn_grid_search = GridSearchCV(knn_classifier, knn_params, cv=5)
knn_grid_search.fit(train_data.drop(['Survived'], axis=1), train_data['Survived'])
knn_exec_time = time.time() - start
#
# XGBoost
start = time.time()
xgb_params = {'n_estimators': [50, 100, 200, 300], 'max_depth': [3, 4, 5, 6]}
xgb_classifier = xgb.XGBClassifier(verbosity=0, use_label_encoder=False, objective='binary:logistic')
xgb_grid_search = GridSearchCV(xgb_classifier, xgb_params, cv=5)
xgb_grid_search.fit(train_data.drop(['Survived'], axis=1), train_data['Survived'])
xgb_exec_time = time.time() - start

# Evaluate the models on the test set
rf_best_model = rf_grid_search.best_estimator_
rf_best_hyperparams = rf_grid_search.best_params_

lr_best_model = lr_grid_search.best_estimator_
lr_best_hyperparams = lr_grid_search.best_params_

knn_best_model = knn_grid_search.best_estimator_
knn_best_hyperparams = knn_grid_search.best_params_

xgb_best_model = xgb_grid_search.best_estimator_
xgb_best_hyperparams = xgb_grid_search.best_params_

rf_predictions = rf_best_model.predict(test_data.drop(['Survived'], axis=1))
lr_predictions = lr_best_model.predict(test_data.drop(['Survived'], axis=1))
knn_predictions = knn_best_model.predict(test_data.drop(['Survived'], axis=1))
xgb_predictions = xgb_best_model.predict(test_data.drop(['Survived'], axis=1))

# Calculate and print the accuracy of the models
rf_accuracy = accuracy_score(test_data['Survived'], rf_predictions)
rf_precision = precision_score(test_data["Survived"], rf_predictions)

lr_accuracy = accuracy_score(test_data['Survived'], lr_predictions)
lr_precision = precision_score(test_data["Survived"], lr_predictions)

knn_accuracy = accuracy_score(test_data['Survived'], knn_predictions)
knn_precision = precision_score(test_data["Survived"], knn_predictions)

xgb_accuracy = accuracy_score(test_data['Survived'], xgb_predictions)
xgb_precision = precision_score(test_data["Survived"], xgb_predictions)

print(f'Random Forest Accuracy: {rf_accuracy}', f'Precision: {rf_precision}', f'With params: {rf_best_hyperparams}', f'And time: {rf_exec_time}')
print(f'Logistic Regression Accuracy: {lr_accuracy}', f'Precision: {lr_precision}', f'With params: {lr_best_hyperparams}', f'And time: {lr_exec_time}')
print(f'K-Nearest Neighbors Accuracy: {knn_accuracy}', f'Precision: {knn_precision}', f'With params: {knn_best_hyperparams}', f'And time: {knn_exec_time}')
print(f'XGBoost Accuracy Accuracy: {xgb_accuracy}', f'Precision: {xgb_precision}', f'With params: {xgb_best_hyperparams}', f'And time: {xgb_exec_time}')