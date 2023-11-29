# FIFA Players Dataset Model Training
This data is [FIFA 22 Complete Player Dataset](https://www.kaggle.com/datasets/stefanoleone992/fifa-22-complete-player-dataset)  
Only Men Player Data across 2015 - 2022 are used to conduct the training. The **data** folder contains all the data used. We will be predicting the **overall** value of player.

## Dataset Schema Introduction

The dataset presents a detailed schema representing professional soccer players from the FIFA database. It encompasses a range of attributes from basic personal information to in-depth performance metrics:

- **UID**: A unique identifier for each dataset entry.
- **sofifa_id**: FIFA's specific unique identifier for each player.
- **short_name & long_name**: The player's commonly used name and their full legal name, respectively.
- **overall**: A numeric score between 0 to 100 reflecting the player's overall skill level.
- **potential**: The predicted peak performance rating of a player's career.
- **value_eur**: The market value of a player expressed in Euros.
- **wage_eur**: The wage of a player, also in Euros.
- **player_positions**: The positions a player can occupy on the field, such as Center Forward (CF), Left Wing (LW), Right Wing (RW).
- **age & dob**: The player's age and date of birth.
- **nationality_id**: A code representing the player's nationality.
- **Year**: The year the FIFA record pertains to.
- **player_url**: A URL leading to the player's full profile on the FIFA site.
- **Performance Metrics**: Attributes that define a player's capabilities, such as pace, shooting, passing, dribbling, defending, and physic.
- **Detailed Metrics**: More granular attributes like attacking crossing, skill moves, movement balance, and so on.
- **Position Ratings**: Ratings that indicate how well-suited a player is for various positions on the field (e.g., goalkeeper (gk), right-back (rb), center-back (cb)).
- **Preferred Foot**: Specifies whether the player is left or right-foot dominant.
- **player_tags & player_traits**: Tags and traits that describe a player's characteristics and playing style.

## Dataset Constraints

- **UID**: Primary Key
- **Not Null Constraints**: sofifa_id, short_name, long_name, overall, player_positions, age, dob, nationality_id, Year.
- **Check Constraints**: 
  - overall values between 0 and 100.
  - preferred_foot values either 'Left' or 'Right'.
- **Unique Constraints**: player_url.

> **Note**: Some features currently contain bad or null values, preventing the application of certain constraints.

## Notebook Versions

### Base Version
Excludes 'potential', 'wage_eur', and 'value_eur' for a more challenging model training scenario. These three features are directly related to the 'overall' value and therefore we exclude them to look for a more general model.

### Version 2 (With pgAdmin)
Includes all features, providing a comprehensive dataset for analysis.

### Version 3 (Cloud Version)
Tailored for cloud-based analytics with specific preprocessing adaptations.

## Data Pipeline
We include a script automates the preprocessing of the raw dataset, making it ready for model use. This pipeline includes data cleaning, feature selection, scaling, imputation, and more.

## Data Engineering

### Unused Features

1. Identifiers and URLs:
- Columns: sofifa_id, player_url, short_name, long_name, player_face_url, club_logo_url, club_flag_url, nation_logo_url, nation_flag_url, UID
- Reason: These are unique identifiers and URLs, which are not predictive of a player's performance but rather are used for identification and data retrieval purposes.
2. Club and Nation Specific Info:
- Columns: club_team_id, club_name, league_name, nationality_id, nationality_name, nation_team_id
- Reason: These fields are either nominal categorical data or represent specific entities (teams, leagues, nations) that by themselves may not provide predictive value unless further engineered or contextualized.
3. Contract and Personal Details:
- Columns: dob, club_joined, club_contract_valid_until, club_loaned_from
- Reason: Birth dates and contract details, while potentially useful for different analyses (e.g., age calculation), are directly represented by other features (e.g., age) or are not directly relevant to predicting current overall score.
4. Positional Information:
- Columns: club_position, club_jersey_number, nation_position, nation_jersey_number, player_positions
- Reason: Specific positional details are less about player ability and more about current team formation or role. If you're not focusing on position-specific performance, these can be excluded.
5. Aesthetics and Meta Attributes:
- Columns: real_face, body_type, player_tags, player_traits
- Reason: Attributes like real face, body type, tags, and traits are more about the game's graphical representation and player meta-attributes, not directly related to performance skills.
6. Computed or Derived Fields:
- Columns: ls, st, rs, lw, lf, cf, rf, rw, lam, cam, ram, lm, lcm, cm, rcm, rm, lwb, ldm, cdm, rdm, rwb, lb, lcb, cb, rcb, rb, gk
- Reason: These appear to be derived or secondary statistics possibly based on primary attributes like pace, shooting, etc. If they are derived from other features in the dataset, including them can lead to multicollinearity.

### Preprocessing Steps
- Removing null values
- Type casting
- Scaling
- Data imputation
- Correlation analysis
- One-hot encoding
- Feature importance analysis

### Pyspark Model

#### Linear Regression
We start off with a general regression model to observe the model's linear relationship. Turns out it is a great model to use for our dataset. For linear regression, we use RMSE as our metric to measure.

- Raw model training result:
  - (RMSE) on train data = 1.2819346390523099
  - (RMSE) on test data = 1.2765777163509375
  
After tuning the `regParam`, `elasticNetParam`, and `maxIter`, we ended up with a similar result:

- Best Model Train RMSE: 1.2819355821866885
- Best Model Test RMSE: 1.276583966594367

We chose these parameters to tune the weight change in each training step and see if more/less iterations will affect our training. It turns out the data has a strong linear relationship between the feature columns and our output.

**Final Model Parameters:**
- `regParam` = 0.001
- `maxIter` = 25
- `elasticNetParam` = 0 (L2 reg)

#### Random Forest Regressor
From the previous model result, we can expect a worse initial performance with this model as it is better with a non-linear relationship in data. With an initial data training, we obtain the result of:

- Train RMSE: 2.1127236663754667
- Test RMSE: 2.118435319010242

We chose the parameter `numTrees` and `maxDepth` to tune. This is to see with increased complexity of the model if we can train better. However, we might expect overfitting with it.

Tuned Result:
- Train RMSE: 1.2591774801821691
- Test RMSE: 1.3135627968389536

Somewhat overfitting but overall better result. This suggests that with more depth and trees, the random forest regressor can capture the linear relationship between features and the overall value; however, it becomes sensitive to noises in the data.

**Final Model Parameters:**
- `numTrees` = 35
- `maxDepth` = 10
- others are default

### Pytorch and Tensorflow model

#### Pytorch Shallow Neural Network
Since from PySpark models we learn that our data has a linear relationship, therefore we will start with a shallow network as it is more fitted to the data. We will use the ReLU activation as it does not scale our data. It turns out we are right; we start with 100 epochs, 64 batch size, 0.01 learning rate, and Adam optimizer with MSELoss function we obtain an initial result close to the linear regression model. Later on, as we conduct different parameters, here's the best model summary:

**Best epoch: 144**
- Train loss: 0.5290159618450423
- Test loss: 0.47972606195628276
- batch size: 128
- learning rate: 0.001
- number of epochs: 150

```python
myNeuralNet(
  (ln1): Linear(in_features=59, out_features=64, bias=True)
  (ln2): Linear(in_features=64, out_features=32, bias=True)
  (ln3): Linear(in_features=32, out_features=1, bias=True)
)
```


#### TensorFlow Deep Neural Network
For the local version, we experimented with a deep neural network in TensorFlow. Given the linear nature of the data indicated by previous models, we started with a more complex model with additional hidden layers while maintaining ReLU as the activation function, considering our output is a continuous numerical value ranging from 0 to 100. We opted to use Mean Absolute Error (MAE) as our evaluation metric.

Initial results indicated some degree of overfitting, which is attributed to the increased model complexity capturing noise in the data. After tuning the model to achieve better generalization, the results improved:

**Model Performance:**
- Test loss: 0.4753
- Test MAE: 0.4994

**Model Architecture:**
```python
model = tf.keras.Sequential([
    tf.keras.layers.Dense(64, activation='relu', input_shape=[input_shape]),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(8, activation='relu'),
    tf.keras.layers.Dense(4, activation='relu'),
    tf.keras.layers.Dense(1)
])
model.compile(optimizer='adam', loss='mse', metrics=['mae'])
```
Notice that in the cloud version we are unable to conduct a tensorflow model therefore we replicated this neural network structure and use pytorch on the cloud instead. However the performance shouldn't vary too much.


## Usage

Download this repository to your local environment in the same folder and run data_processing.ipynb in a jupyter notebook environment. Read the comment and see what part you need to modify/Uncomment.

Make sure you have all the necessary libraries and dependencies for pyspark, pytorch and tensorflow. #

## Running Process
- Task I and II (Data Ingestion and analysis):
These two tasks are done within data_analysis.ipynb. To start off, make sure you have the data folder in the same directory with your python notebooks. Then run through the code. When you get to the prepare truncated data section, you can uncomment them if you want a truncated version data to upload to cloud later on. For the postgres ingestion part, remeber to switch to your own database properties. Also uncomment the writing section if you haven't written to the database. Lastly, for each query, remember to input a numerical number then the query will output the result.

- Task III (Model training and data engineering) Spark Version:
The spark version is done with the notebook specified as data_engineering_pyspark. Create a spark session, then change to your own database properties to read the data in. When you get to the correlation analysis part, you might need to wait a minute or two due to the large amount of data we are dealing with. If you come to a time out error with your spark session, truncate your data and restart your kernel. When you get to the ingesting Engineered Data into Postgres part, remember to change the properties to your own. This part will be needed for the pytorch version because we are skipping the data cleaning and preprocessing steps in pytorch. Lastly, when training with randomforest regressor, you might need to wait a few minutes. As previously mention, if you come to a timeout or connection loss with pyspark, truncate your dataset and restart the kernel.

- Task III (Model training and data engineering) Pytorch Version:
For this version, when reading from the database, remember to have your own properties ready. After reading in the data, there are some commented code which is for cloud version. You can disregard them if you are not planning to run the code on the cloud. After that you can simply run through the code.

- Task IV (Combine Task I, II and III) Cloud version:
There are some slight changes in the cloud version of the previous three tasks. Instead of reading from local directory or postgres database, we are reading from the cloud storage in GCP. You will need to create a bucket and upload the csv or truncated version data I previously mentioned. In addition when reading from bucket, you will need you own bucket path instead of mine. For the cloud version, I am using the n2d-standard-2 machine with 2 worker nodes. 


## Video Demonstration
This section contains more detailed walkthrough of my code including showing database in pgadmin and etc.
https://cmu.box.com/s/ery6ed2lnls415vj45vw68oxsb5c2ggt

