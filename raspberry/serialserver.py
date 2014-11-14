from twisted.internet.protocol import Factory, Protocol
from twisted.internet import reactor

 
class IphoneChat(Protocol):
    def connectionMade(self):
        print "a client connected"
	self.factory.clients.append(self)
	print "clients are ", self.factory.clients
    def connectionLost(self, reason):
        self.factory.clients.remove(self)
 
port = 3333
factory = Factory()
factory.protocol = IphoneChat
factory.clients = []
reactor.listenTCP(3333, factory)
print "Iphone Chat server started. listenning on port " + str(port) 
reactor.run()
