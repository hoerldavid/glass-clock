import serial
import time
ser = serial.Serial('/dev/cu.usbserial-1420', 9600)

while True:
	print(ser.readline())
	T = "T"+str(round(time.time()+3600))
	T = bytes(T, encoding='utf-8')
	ser.write(T)
	print('submitted time update')
	time.sleep(60)
