#!/usr/bin/python
import traceback
import radioPositioning

def main():
	try:
		result = radioPositioning.getRadioPosition()		

		print ('x_msmts y_msmts: ', result[0], result[1])
		print ('x_means y_means: ', result[2], result[3])
		
	except:
		print (traceback.format_exc())

if __name__ == "__main__":
  main()