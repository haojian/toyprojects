import serial
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.open()
response = ser.readline()
print response
try:
    while 1:
	var = raw_input("Enter something: ")
	ser.write(var)
        response = ser.readline()
        print response
except KeyboardInterrupt:
    ser.close()
