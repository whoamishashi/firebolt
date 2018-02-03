#!/usr/bin/python

from pykalman import KalmanFilter
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os, time, traceback, sys
from scipy import stats
import urllib.request
import json
#from pprint import pprint

class SensorUWB:
	"""
	This class filters UWB positioning data.
	"""

	DATA_URL='http://192.168.1.1/website/src/val.js'
	DATA_FILE = 'temp.js'
	ITERATIONS= 60
	DATA_FREQ = 0.5
	OFFSET_X = 0
	OFFSET_Y = 0 #0.05
	
	def __init__(self):
		self.data_array = []
		self.measurements = []
		self.initial_state_mean = []
		self.transition_matrix = []
		self.observation_matrix = []
		self.smoothed_state_means = []
		self.smoothed_state_covariances = []
		
	def getData(self):
		"""get the raw data as x, y values in data_array from the R-Pi server."""
		for i in range(0,self.ITERATIONS):
			try:
				urllib.request.urlretrieve(self.DATA_URL, self.DATA_FILE)
				if (os.stat(self.DATA_FILE).st_size != 0):
					data = json.load(open(self.DATA_FILE))
					self.data_array.append(tuple((abs(float(data["N4"][0])), abs(float(data["N4"][1])))))
					time.sleep(self.DATA_FREQ)
				else:
					i-=1
					
			except:
				i-=1
				print (traceback.format_exc())
				# return 1
		
		#print (self.data_array)


	def removeOutliersUsingSD(self):
		"""remove outliers from data_array using concepts of standard deviation"""
		try:

			df = pd.DataFrame(self.data_array)
			#print ( df )
			#print ( df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values )
			self.data_array = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values

		except:
			print (traceback.format_exc())
			return 1
			
	def filterData(self):
		"""do Kalman filtering on position (x, y) data"""
		try:

			self.measurements = np.asarray(self.data_array)

			self.initial_state_mean = [self.measurements[0, 0],
								  0,
								  self.measurements[0, 1],
								  0]

			self.transition_matrix = [[1, 1, 0, 0],
								 [0, 1, 0, 0],
								 [0, 0, 1, 1],
								 [0, 0, 0, 1]]

			self.observation_matrix = [[1, 0, 0, 0],
								  [0, 0, 1, 0]]
						
			kf = self.doKalman()
			kf = self.redoKalman(kf)

		except:
			print (traceback.format_exc())
			return 1

	def doKalman(self):
		"""do Kalman filtering on position (x, y) data"""
		try:
			
			kf = KalmanFilter(transition_matrices = self.transition_matrix,
							  observation_matrices = self.observation_matrix,
							  initial_state_mean = self.initial_state_mean)

			kf = kf.em(self.measurements, n_iter=5)
			(self.smoothed_state_means, self.smoothed_state_covariances) = kf.smooth(self.measurements)
			
			return kf
			
		except:
			print (traceback.format_exc())
			return 1
		
	def redoKalman(self, kf0):
		"""do Kalman filtering on position (x, y) data"""
		try:
			
			kf = KalmanFilter(transition_matrices = self.transition_matrix,
							  observation_matrices = self.observation_matrix,
							  initial_state_mean = self.initial_state_mean,
							  observation_covariance = 10*kf0.observation_covariance,
							  em_vars=['transition_covariance', 'initial_state_covariance'])

			kf = kf.em(self.measurements, n_iter=5)
			(self.smoothed_state_means, self.smoothed_state_covariances) = kf.smooth(self.measurements)
			
			#self.plotGraph(1)

			return kf
			
		except:
			print (traceback.format_exc())
			return 1
			
	def plotGraph(self, n):
		"""Plot filtered measurements as graph"""
		try:

			# plt.figure(n)
			# times = range(self.measurements.shape[0])
			# plt.plot(times, self.measurements[:, 0], 'bo',
					 # times, self.measurements[:, 1], 'ro',
					 # times, self.smoothed_state_means[:, 0], 'b--',
					 # times, self.smoothed_state_means[:, 2], 'r--',)
			# plt.show()
			
			df1 = pd.DataFrame(self.measurements)
			x1 = df1.take([0], axis =1).values[:, 0]
			y1 = df1.take([1], axis =1).values[:, 0]
			
			#df2 = pd.DataFrame(self.smoothed_state_means)
			x2 = self.smoothed_state_means[:, 0] #df2.take([0], axis =1).values[:, 0]
			y2 = self.smoothed_state_means[:, 2] #df2.take([1], axis =1).values[:, 0]

			# plt.scatter(x1, y1)
			# plt.plot(x1,y1)
			# plt.show()
			
			plt.scatter(x2, y2)
			plt.plot(x2,y2)
			plt.xlim([0, 2])
			plt.ylim([0, 2])
			plt.show()
			
		except:
			print (traceback.format_exc())
			return 1
		
	def getValue(self):
		"""Get the average filtered value"""
		try:		
			tot_col0 =0
			tot_col1 =0
			latest_x =0
			latest_y =0
			
			# print (self.measurements)
			# n=len(self.measurements)/2
			# del self.measurements[:int(n)]
			# print (self.measurements)
			
			for arrayi in self.measurements:
				tot_col0 += arrayi[0]
				tot_col1 += arrayi[1]
				latest_x  = arrayi[0]
				latest_y  = arrayi[1]
			
			x = tot_col0/self.measurements.shape[0] + self.OFFSET_X		#avg
			y = tot_col1/self.measurements.shape[0] + self.OFFSET_Y		#avg
			
			# x = latest_x + self.OFFSET_X								#latest
			# y = latest_y + self.OFFSET_Y								#latest

			return (x, y)
			
		except:
			print (traceback.format_exc())
			return 1

def main():
	try:
		sensorUWB = SensorUWB()
		sensorUWB.getData()
		sensorUWB.removeOutliersUsingSD()
		sensorUWB.filterData()
		sensorUWB.plotGraph(1)
		result = sensorUWB.getValue()
		
		# fh = open('output.txt', 'w')
		# for item in sensorUWB.measurements:
			# fh.write("\n%s", % item)
				
		print ('x, y: ', result[0], result[1])
		sys.exit(0)
	except:
		# print (traceback.format_exc())
		sys.exit(1)

if __name__ == "__main__":
  main()
  
