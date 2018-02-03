# import numpy

# arr = [1.85, 1.91, 1.85, 1.91, 1.88, 1.87, 1.86, 1.87, 1.86, 1.83, 1.90, 1.89, 1.89, 1.88, 1.88, 1.86, 1.89, 1.88, 1.86, 1.89, 1.93, 1.93, 1.89, 1.89, 1.90, 1.90, 1.86, 1.87, 1.88, 1.89, 1.81, 1.85, 1.88, 1.93, 1.79, 1.88, 1.83, 1.82, 1.85, 1.88, 1.83, 1.88, 1.89, 1.87]

# elements = numpy.array(arr)

# mean = numpy.mean(elements, axis=0)
# sd = numpy.std(elements, axis=0)

# final_list = [x for x in arr if (x > mean - 1 * sd)]
# final_list = [x for x in final_list if (x < mean + 1 * sd)]

# arr.sort()
# print(arr)
# print(' ')
# final_list.sort()
# print(final_list)

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

import warnings
x = np.ones((1000, 1000)) * np.nan
# I expect to see RuntimeWarnings in this block
with warnings.catch_warnings():
    warnings.simplefilter("ignore", category=RuntimeWarning)
    foo = np.nanmean(x, axis=1)
	
np.seterr(divide='ignore', invalid = 'ignore')
DATA_URL='http://192.168.1.1/website/src/val.js'
DATA_FILE = 'temp.js'
outer_iter =10
inner_iter =10
	# n0 = []
	# n1 = []
	# n2 = []
	# n3 = []
n4 = []
n5 = []

for i in range(0, outer_iter):
	print (i)
																		# try:
	# N0 = []
		# N1 = []
		# N2 = []
		# N3 = []
	N4 = []
	N5 = []

	for i in range(0,inner_iter):
		try:
			urllib.request.urlretrieve(DATA_URL, DATA_FILE)
			if (os.stat(DATA_FILE).st_size != 0):
				data = json.load(open(DATA_FILE))
				
				# N0.append(tuple((float(data["N0"][0]), float(data["N0"][1]))))
					# N1.append(tuple((abs(float(data["N1"][0])), abs(float(data["N1"][1])))))
					# N2.append(tuple((abs(float(data["N2"][0])), abs(float(data["N2"][1])))))
					# N3.append(tuple((abs(float(data["N3"][0])), abs(float(data["N3"][1])))))
				N4.append(tuple((abs(float(data["N4"][0])), abs(float(data["N4"][1])))))
				N5.append(tuple((abs(float(data["N5"][0])), abs(float(data["N5"][1])))))
				
				time.sleep(0.5)
			else:
				i-=1
				
		except:
			i-=1
			print (traceback.format_exc())

	# N1[0] = tuple((N1[0][0], 0.001))
	
	# print (N1)
	# print (N2)
	# print (N3)
	# print (N4)
	# print (N5)

	# df = pd.DataFrame(N0)
	# N0 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
	# N0 = np.asarray(N0)

		# df = pd.DataFrame(N1)
		# N1 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
		# N1 = np.asarray(N1)

		# df = pd.DataFrame(N2)
		# N2 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
		# N2 = np.asarray(N2)

		# df = pd.DataFrame(N3)
		# N3 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
		# N3 = np.asarray(N3)

	df = pd.DataFrame(N4)
	N4 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
	N4 = np.asarray(N4)

	df = pd.DataFrame(N5)
	N5 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
	N5 = np.asarray(N5)

	# print (N1)
	# print (N2)
	# print (N3)
	# print (N4)
	# print (N5)

	# n0 = np.mean(N0, axis=0, dtype=np.float64)
		# n0.append([0.05, 0.05])
		# n1.append(np.mean(N1, axis=0, dtype=np.float64))
		# n2.append(np.mean(N2, axis=0, dtype=np.float64))
		# n3.append(np.mean(N3, axis=0, dtype=np.float64))
	n4.append(np.mean(N4, axis=0, dtype=np.float64))
	n5.append(np.mean(N5, axis=0, dtype=np.float64))
																			# except:
																				# i-=1
																				# print (traceback.format_exc())
					
	# n0 = np.asarray(n0)
	# n1 = np.asarray(n1)
	# n2 = np.asarray(n2)
	# n3 = np.asarray(n3)
n4 = np.asarray(n4)
n5 = np.asarray(n5)

	# print (n0)
	# print (n1)
	# print (n2)
	# print (n3)
	# print (n4)
	# print (n5)

	# plt.plot(*zip(*n0), marker='o', color='r', ls='')
	# plt.plot(*zip(*n1), marker='o', color='g', ls='')
	# plt.plot(*zip(*n2), marker='o', color='b', ls='')
	# plt.plot(*zip(*n3), marker='o', color='y', ls='')
# plt.plot(*zip(*n4), marker='o', color='c', ls='')
# plt.plot(*zip(*n5), marker='o', color='m', ls='')

df = pd.DataFrame(n4)
n4 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
n4 = np.asarray(n4)

df = pd.DataFrame(n5)
n5 = df[(np.abs(stats.zscore(df)) < 3).all(axis=1)].values
n5 = np.asarray(n5)

# print (n4)
# print (n5)
	
n4mean = np.mean(n4, axis=0, dtype=np.float64)
n5mean = np.mean(n5, axis=0, dtype=np.float64)

print (n4mean)
print (n5mean)
