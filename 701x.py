import pandas as pd
import matplotlib.pyplot as plt

# Step 1: Load and preprocess the data
data = pd.read_csv('acceleration.csv')
data['time_utc'] = pd.to_datetime(data['time_utc'])

# Step 2: Feature extraction
data['acceleration_magnitude'] = (data['acceleration_mgs_x']**2 + data['acceleration_mgs_y']**2 + data['acceleration_mgs_z']**2)**0.5

# Step 3: Identify resting periods
threshold = 30  # Adjust this threshold based on the data characteristics
resting_periods = data[data['acceleration_magnitude'] < threshold]

# Step 4: Plot the results
plt.figure(figsize=(10, 6))
plt.plot(data['time_utc'], data['acceleration_magnitude'], label='Acceleration Magnitude')
plt.scatter(resting_periods['time_utc'], resting_periods['acceleration_magnitude'], color='red', marker='o', label='Resting Periods')
plt.xlabel('Time UTC')
plt.ylabel('Acceleration Magnitude')
plt.title('Animal Behavior Analysis')
plt.legend()
plt.show()

# Step 5: Estimate sleep/rest duration
if not resting_periods.empty:
    resting_periods['time_utc'] = pd.to_datetime(resting_periods['time_utc'])
    rest_duration = (resting_periods['time_utc'].max() - resting_periods['time_utc'].min()).total_seconds()
    print(f"Estimated sleep/rest duration: {rest_duration} seconds")
else:
    print("No resting periods found.")
