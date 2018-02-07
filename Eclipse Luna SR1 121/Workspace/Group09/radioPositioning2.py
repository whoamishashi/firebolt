import threading
from pykalman import KalmanFilter
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os, time, traceback, sys
from scipy import stats
import urllib.request
import json
#from pprint import pprint

DATA_URL='http://192.168.1.1/website/src/val.js'
DATA_FILE = 'val.js'
DATASET= 60
DATAFREQ = 0.45		#(30ms*15)
LATESTINDEX = 0
OFFSET_X = 0
OFFSET_Y = 0 #0.05
XLIM = 2
YLIM = 2

noisy_data = []
noiseless_data= []

def create_global_func():
	global getData
	def getData():
		"""This global method gets the raw data"""
		while True:
			try:
				urllib.request.urlretrieve(DATA_URL, DATA_FILE)
				if (os.stat(DATA_FILE).st_size != 0):
					data = json.load(open(DATA_FILE))
					return tuple((abs(float(data["N4"][0])), abs(float(data["N4"][1]))))
			except:
				print (traceback.format_exc())

class GetNewData(threading.Thread):
	"""
	This class gets noisy positioning data.
	"""

	def __init__(self):
		super(GetNewData , self).__init__(name="GetNewData thread")
		print ("Thread GetNewData initialized")

	def run(self):
		try:
			self.update_noisy_data()
		except:
			print (traceback.format_exc())
			sys.exit(1)

	def update_noisy_data(self):
		"""get the raw data as x, y values in data_array from the R-Pi server."""
		global noisy_data
		global getData
		global DATASET
		global DATAFREQ
		global LATESTINDEX
		for i in range(0,DATASET):
			try:
				xy = getData()
				LATESTINDEX = i%DATASET
				noisy_data[LATESTINDEX] = xy
				time.sleep(DATAFREQ)
			
			except:
				print ("Exception in update_noisy_data")
				print (traceback.format_exc())

class FilterNoise(threading.Thread):
	"""
	This class filters positioning data noise.
	"""

	def __init__(self):
		super(FilterNoise , self).__init__(name="FilterNoise thread")
		print ("Thread FilterNoise initialized")
		self.data_array = []
		self.measurements = []
		self.initial_state_mean = []
		self.transition_matrix = []
		self.observation_matrix = []
		self.smoothed_state_means = []
		self.smoothed_state_covariances = []

	def run(self):
		try:
			global getData
			global noisy_data
			# print ("noisy_data")
			# print (noisy_data)
			self.data_array = noisy_data
			self.removeOutliersUsingSD()
			self.filterDataUsingKalman()
			self.plotGraph(1)
			
			global noiseless_data
			noiseless_data = self.smoothed_state_means
		except:
			print (traceback.format_exc())
			sys.exit(1)
			
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
			
	def filterDataUsingKalman(self):
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
			plt.xlim([0, self.XLIM])
			plt.ylim([0, self.YLIM])
			plt.savefig('radio.png', bbox_inches='tight')
			#plt.show()
			
		except:
			print (traceback.format_exc())
			return 1

def initialize_noisy_data():
	"""get the raw data as x, y values in data_array from the R-Pi server."""
	global getData
	for i in range(0,DATASET):
		try:
			xy = getData()
			noisy_data.append(xy)
			time.sleep(DATAFREQ)
				
		except:
			print ("Exception in initialize_noisy_data")
			print (traceback.format_exc())
	
	print("noisy_data initialized")
	print(noisy_data)

def getPosition():
	"""Get the average filtered value"""
	try:
		global noiseless_data
		global OFFSET_X
		global OFFSET_Y

		x_inst =0
		y_inst =0
		
		if(len(noiseless_data)!=0):
			# print ("noiseless_data")
			# print (noiseless_data)
			for arrayi in noiseless_data:
				latest_x  = arrayi[0]
				latest_y  = arrayi[1]
			
			x_latest = latest_x + self.OFFSET_X								#latest
			y_latest = latest_y + self.OFFSET_Y								#latest
			
			print ('x_inst y_inst: ', x_inst, y_inst)
		return (x_inst, y_inst)
		
	except:
		print (traceback.format_exc())
		return tuple((1,1))

def initialize():
	create_global_func()
	initialize_noisy_data()
	
	thread1 = GetNewData()
	thread2 = FilterNoise()

	thread1.start()
	thread2.start()

	# while thread1.is_alive() or thread2.is_alive():
		# pass

