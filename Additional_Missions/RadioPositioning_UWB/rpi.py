#!/usr/bin/python

# UWB Script
# Created by: Shashikant Gupta
# Last Modified by: Shashikant Gupta

import traceback
import radioPositioning
import uart3

def main():
	while True:
		try:
			data = radioPositioning.getRadioPosition("centroid", 0)
			print ("\nx y: ", data[0], data[1])
			if len(data) == 2:
                                return data
			   
		except:
			print (traceback.format_exc())
			 #return 1

if __name__ == "__main__":
  main()
