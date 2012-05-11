#-------------------------------------------------------------------------------
# Name:        module2
# Purpose:
#
# Author:      faito
#
# Created:     05.05.2010
# Copyright:   (c) faito 2010
# Licence:     <your licence>
#-------------------------------------------------------------------------------
#!/usr/bin/env python

from bt import btTracker
from BaseHTTPServer import HTTPServer

def main():
    try:
        server = HTTPServer(('', 6445), btTracker)
        print 'Starting http-server...'
        server.serve_forever()
    except KeyboardInterrupt:
        print '^C received, shutting down'
        server.socket.close()

if __name__ == '__main__':
    main()