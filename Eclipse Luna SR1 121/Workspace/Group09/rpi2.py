#!/usr/bin/python
import traceback
import radioPositioning

def main():
	try:
		radioPositioning.initialize()
		# while True:
		radioPositioning.getPosition()		

		#print ('x_inst y_inst: ', result[0], result[1])
		
	except:
		print (traceback.format_exc())

if __name__ == "__main__":
  main()