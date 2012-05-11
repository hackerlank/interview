# -*- coding: utf-8 -*-

__author__="faito"
__date__ ="$21.12.2010 4:18:05$"

import urllib2
import socket

DEFAULT_SOCKET_TIMEOUT = 2

def url_get_content(url):

    socket.setdefaulttimeout(DEFAULT_SOCKET_TIMEOUT)

    try:
	response = urllib2.urlopen(url)
	the_page = response.read()
    except:
	raise

    return the_page