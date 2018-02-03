from pykalman import KalmanFilter
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os, time

import urllib.request
import json
#from pprint import pprint

N4_x = []
N4_y = []
as_array = []

###get x, y values in as_array
for i in range(0,40):
	urllib.request.urlretrieve('http://192.168.1.1/website/src/val.js', 'val.js')
	if (os.stat('val.js').st_size != 0):
		data = json.load(open('val.js'))
		N4_x.append(float(data["N4"][0]))
		N4_y.append(float(data["N4"][1]))
		as_array.append(tuple((N4_x[i], N4_y[i])))
		#print(N4_x[i],' ',N4_y[i])
		time.sleep(0.5)
	else:
		i-=1

print (as_array)

###remove outliers from as_array using concepts of standard deviation
df = pd.DataFrame(as_array)
#print( df )
from scipy import stats
#print ( df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values )

dataaaa_array = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values

measurements = np.asarray(dataaaa_array)

###kalman filter
initial_state_mean = [measurements[0, 0],
                      0,
                      measurements[0, 1],
                      0]

transition_matrix = [[1, 1, 0, 0],
                     [0, 1, 0, 0],
                     [0, 0, 1, 1],
                     [0, 0, 0, 1]]

observation_matrix = [[1, 0, 0, 0],
                      [0, 0, 1, 0]]

kf1 = KalmanFilter(transition_matrices = transition_matrix,
                  observation_matrices = observation_matrix,
                  initial_state_mean = initial_state_mean)

kf1 = kf1.em(measurements, n_iter=5)
(smoothed_state_means, smoothed_state_covariances) = kf1.smooth(measurements)

# plt.figure(1)
# times = range(measurements.shape[0])
# plt.plot(times, measurements[:, 0], 'bo',
         # times, measurements[:, 1], 'ro',
         # times, smoothed_state_means[:, 0], 'b--',
         # times, smoothed_state_means[:, 2], 'r--',)
# plt.show()

kf2 = KalmanFilter(transition_matrices = transition_matrix,
                  observation_matrices = observation_matrix,
                  initial_state_mean = initial_state_mean,
                  observation_covariance = 10*kf1.observation_covariance,
                  em_vars=['transition_covariance', 'initial_state_covariance'])

kf2 = kf2.em(measurements, n_iter=5)
(smoothed_state_means, smoothed_state_covariances)  = kf2.smooth(measurements)

# plt.figure(2)
# times = range(measurements.shape[0])
# plt.plot(times, measurements[:, 0], 'bo',
         # times, measurements[:, 1], 'ro',
         # times, smoothed_state_means[:, 0], 'b--',
         # times, smoothed_state_means[:, 2], 'r--',)
# plt.show()

kf3 = KalmanFilter(transition_matrices = transition_matrix,
                  observation_matrices = observation_matrix,
                  initial_state_mean = initial_state_mean,
                  observation_covariance = 10*kf2.observation_covariance,
                  em_vars=['transition_covariance', 'initial_state_covariance'])

kf3 = kf3.em(measurements, n_iter=5)
(smoothed_state_means, smoothed_state_covariances)  = kf3.smooth(measurements)

# plt.figure(3)
# times = range(measurements.shape[0])
# plt.plot(times, measurements[:, 0], 'bo',
         # times, measurements[:, 1], 'ro',
         # times, smoothed_state_means[:, 0], 'b--',
         # times, smoothed_state_means[:, 2], 'r--',)
# plt.show()

kf4 = KalmanFilter(transition_matrices = transition_matrix,
                  observation_matrices = observation_matrix,
                  initial_state_mean = initial_state_mean,
                  observation_covariance = 10*kf3.observation_covariance,
                  em_vars=['transition_covariance', 'initial_state_covariance'])

kf4 = kf4.em(measurements, n_iter=5)
(smoothed_state_means, smoothed_state_covariances)  = kf4.smooth(measurements)

# plt.figure(4)
# times = range(measurements.shape[0])
# plt.plot(times, measurements[:, 0], 'bo',
         # times, measurements[:, 1], 'ro',
         # times, smoothed_state_means[:, 0], 'b--',
         # times, smoothed_state_means[:, 2], 'r--',)
# plt.show()

kf5 = KalmanFilter(transition_matrices = transition_matrix,
                  observation_matrices = observation_matrix,
                  initial_state_mean = initial_state_mean,
                  observation_covariance = 10*kf4.observation_covariance,
                  em_vars=['transition_covariance', 'initial_state_covariance'])

kf5 = kf5.em(measurements, n_iter=5)
(smoothed_state_means, smoothed_state_covariances)  = kf5.smooth(measurements)

plt.figure(5)
times = range(measurements.shape[0])
plt.plot(times, measurements[:, 0], 'bo',
         times, measurements[:, 1], 'ro',
         times, smoothed_state_means[:, 0], 'b--',
         times, smoothed_state_means[:, 2], 'r--',)
plt.show()


print (repr(measurements))



tot_col0 =0
tot_col1 =0

for arrayi in measurements:
	tot_col0 += arrayi[0]
	tot_col1 += arrayi[1]

print (tot_col0/measurements.shape[0])
print (tot_col1/measurements.shape[0])

