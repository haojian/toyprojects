from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor
import serial


class yCarCom(Protocol):
	def connectionMade(self):
		self.factory.clients.append(self)
		print "clients are ", self.factory.clients
	
	def connectionLost(self, reason):
	    self.factory.clients.remove(self)
	
	def dataReceived(self, data):
	    #print "data is ", data
		a = data.split(':')
		if len(a) > 1:
			command = a[0]
			content = a[1]
			
			msg = ""
			if command == "connect":
				self.name = content
				msg = self.name + " has joined"
				self.message(msg)
				
			elif command == "msg":
				msg = self.name + ": " + content
				ser.write(content)
				response = ser.readline()
				self.message(response)
				
			else: 
				msg = data
			
			print msg
				
	def message(self, message):
		self.transport.write(message + '\n')


port = 1234
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.open()

factory = Factory()
factory.protocol = yCarCom
factory.clients = []

reactor.listenTCP(port, factory)
print "yCar server started at port: " + str(port)
reactor.run()