import urllib.request
import json
import os, time
#from pprint import pprint


iMedian = 60
iMean = 40
N4_x = []
N4_y = []
N4_z = []

for i in range(0,iMedian):
	urllib.request.urlretrieve('http://192.168.1.1/website/src/val.js', 'val.js')
	if (os.stat('val.js').st_size != 0):
		data = json.load(open('val.js'))
	# print(data["N4"][0], ", ", data["N4"][1], ", ", data["N4"][2])
	# print(float(data["N4"][0]), ", ", float(data["N4"][1]), ", ", float(data["N4"][2]))
	# totN4_x = totN4_x + float(data["N4"][0])
	# totN4_y = totN4_y + float(data["N4"][1])
	# totN4_z = totN4_z + float(data["N4"][2])
	N4_x.append(float(data["N4"][0]))
	N4_y.append(float(data["N4"][1]))
	N4_z.append(float(data["N4"][2]))
	print(N4_x[i], ' ', N4_y[i], ' ', N4_z[i])
	time.sleep(0.5)

N4_x.sort()
N4_y.sort()
N4_z.sort()

totN4_x = totN4_y = totN4_z = 0

range_start = int(iMedian/2-iMean/2)
range_end = int(iMedian/2+iMean/2)
# print(range_start, ' ', range_end)

for i in range(range_start,range_end):
    totN4_x = totN4_x + N4_x[i]
    totN4_y = totN4_y + N4_y[i]
    totN4_z = totN4_z + N4_z[i]

avgN4_x = totN4_x/iMean
avgN4_y = totN4_y/iMean
avgN4_z = totN4_z/iMean

print(avgN4_x, ' ', avgN4_y, ' ', avgN4_z)

# print (totN4_x, ", ", totN4_y, ", ", totN4_z)
# avgN4_x = totN4_x/iterations
# avgN4_1 = totN4_y/iterations
# avgN4_2 = totN4_z/iterations
# print (avgN4_x, ", ", avgN4_1, ", ", avgN4_2)
