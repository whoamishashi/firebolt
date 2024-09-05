#!/usr/bin/python

# UWB module
# Created by: Shashikant Gupta
# Last Modified by: Shashikant Gupta

from pykalman import KalmanFilter			#Kalman Filter algorithm 
import pandas as pd							#dataframe processing
from scipy import stats						#statistial data aprocessing
import matplotlib.pyplot as plt				#plotting graphs
from sklearn.cluster import KMeans			#k means clustering using Llyods algorithm
from sklearn import metrics
import os, time, traceback, sys
import numpy as np
import urllib.request
import json
#from pprint import pprint


class SensorUWB:
	"""
	This class filters UWB positioning data.
	"""
	
	#final
	DATA_URL='http://192.168.1.1/website/src/val.js'
	DATA_FILE = 'val.js'
	
	#final?
	ITERATIONS = 10
	OFFSET_X = 0
	OFFSET_Y = 0.05
	SLEEP = 0.1
	
	def __init__(self):
		self.data_array = []
		self.measurements = []
		self.initial_state_mean = []
		self.transition_matrix = []
		self.observation_matrix = []
		self.smoothed_state_means = []
		self.smoothed_state_covariances = []
		self.centers = []
		
	def getData(self):
		"""get unique raw data as x, y values in data_array from the R-Pi server."""
		old_x = 0
		old_y = 0
		n = self.ITERATIONS
		ne= 0
		while n > 0:
			try:
				urllib.request.urlretrieve(self.DATA_URL, self.DATA_FILE)
				# if (os.stat(self.DATA_FILE).st_size != 0):
				data = json.load(open(self.DATA_FILE))
				new_x = abs(float(data["N4"][0]))
				new_y = abs(float(data["N4"][1]))
				if(old_x != new_x or old_y != new_y):
					self.data_array.append(tuple((new_x, new_y)))
					old_x = new_x
					old_y = new_y
					n -= 1
					# time.sleep(self.SLEEP)

			except:
				ne+=1
				if(ne == self.ITERATIONS*50): return
				else: 
					# print (traceback.format_exc())
					pass
		
		print (self.data_array)

	def removeOutliersUsingSD(self):
		"""remove outliers from data_array using concepts of statistics: z-score, standard deviation, mean"""
		try:
			df = pd.DataFrame(self.data_array)
			#print ( df )
			#print ( df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values )
			self.data_array = df[(np.abs(stats.zscore(df)) < 2).all(axis=1)].values
			# print (self.data_array)

		except:
			print (traceback.format_exc())
			return 1
			
	def kmeansCentroid(self):
		"""calculate K centroids of scattered points data"""
		try:
			# clustering dataset
			 
			# create new plot and data
			x1 = np.asarray(self.data_array[:,0])
			y1 = np.asarray(self.data_array[:,1])
			# print(x1)
			# print(y1)
			X = np.asarray(self.data_array).reshape(len(self.data_array), 2)
			colors = ['b']
			markers = ['o']
			
			# KMeans algorithm 
			K = 2
			kmeans_model = KMeans(n_clusters=K).fit(X)

			# print(kmeans_model.cluster_centers_)
			
			self.centers = np.array(kmeans_model.cluster_centers_)
			print (self.centers)
			x = self.centers[1][0]
			y = self.centers[1][1]

			return (x, y)
			
		except:
			print (traceback.format_exc())
			return 1
			
	def filterData(self):
		"""initialize Kalman Filter parameters"""
		try:

			self.measurements = np.asarray(self.data_array)

			# print (self.measurements)
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
						
			for arrayi in self.smoothed_state_means:
				# tot_col0 += arrayi[0]
				# tot_col1 += arrayi[2]
				x_means  = arrayi[0]
				y_means  = arrayi[2]
		
			# print (self.smoothed_state_means)
			# print (x_means, y_means)
			return (x_means, y_means)

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
			
	def plotGraph(self, op):
		"""Plot filtered measurements as graph"""
		try:
			plt.figure()

			if (op == "centroid"):
				plt.plot(np.asarray(self.data_array[:,0]), np.asarray(self.data_array[:,1]), 'bo')
				plt.plot(np.asarray(self.centers[:,0]), np.asarray(self.centers[:,1]), 'ro')
				#plt.plot(self.centers[0][0], self.centers[0][1], 'ro')

			else:
				# times = range(self.measurements.shape[0])
				# plt.plot(times, self.measurements[:, 0], 'bo',
						 # times, self.measurements[:, 1], 'ro',
						 # times, self.smoothed_state_means[:, 0], 'b--',
						 # times, self.smoothed_state_means[:, 2], 'r--',)
				# plt.show()
				plt.plot(np.asarray(self.data_array[:,0]), np.asarray(self.data_array[:,1]), 'bo')

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
			# plt.savefig('radio.png', bbox_inches='tight')
			plt.show()
			
		except:
			print (traceback.format_exc())
			return 1
		
	def getValue(self):
		"""Get the average filtered value"""
		try:		
			# tot_col0 =0
			# tot_col1 =0
			# x_measurements = 0						#latest
			# y_measurements = 0						#latest			
			# x_means = 0				#latest
			# y_means = 0				#latest
			
			# print (self.measurements)
			# n=len(self.measurements)/2
			# del self.measurements[:int(n)]
			# print (self.measurements)
			# print (self.smoothed_state_means)
			# print (self.smoothed_state_covariances)
			
			# for arrayi in self.measurements:
				# tot_col0 += arrayi[0]
				# tot_col1 += arrayi[1]
				# x_measurements  = arrayi[0]
				# y_measurements  = arrayi[1]
				
			for arrayi in self.smoothed_state_means:
				# tot_col0 += arrayi[0]
				# tot_col1 += arrayi[2]
				x_means  = arrayi[0]
				y_means  = arrayi[2]
				
			# for arrayi in self.measurements:
				# tot_col0 += arrayi[0]
				# tot_col1 += arrayi[1]
				# x_measurements  = self.measurements[-1][0]
				# y_measurements  = self.measurements[-1][1]
				
			# x_avg = tot_col0/self.smoothed_state_means.shape[0] + self.OFFSET_X		#avg
			# y_avg = tot_col1/self.smoothed_state_means.shape[0] + self.OFFSET_Y		#avg
			
			# x_measurements = latest_x + self.OFFSET_X								#latest
			# y_measurements = latest_y + self.OFFSET_Y								#latest
			
			# x_means = latest_x + self.OFFSET_X								#latest
			# y_means = latest_y + self.OFFSET_Y								#latest
			
			# x_means  = self.smoothed_state_means[-1]
			# y_means  = self.smoothed_state_means[-1]
		
			print (x_means, y_means)
			return (x_means, y_means)
			
		except:
			print (traceback.format_exc())
			return 1

def getRadioPosition(op, plotting):
		sensorUWB = SensorUWB()
		sensorUWB.getData()
		sensorUWB.removeOutliersUsingSD()
		
		if (op == "centroid"):
			# do K-Means for data centroids
			result = sensorUWB.kmeansCentroid()
		if (op == "locus"):
			# do Kalman Filter for trajectory
			result = sensorUWB.filterData()
		
		if (plotting == 1): sensorUWB.plotGraph(op)

		return result
