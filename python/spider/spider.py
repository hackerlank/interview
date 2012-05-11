# -*- coding: utf-8 -*-

__author__="faito"
__date__ ="$21.12.2010 4:10:29$"

import net
import re
import psycopg2
from urlparse import urlparse
from extractor import extractor
import time

class spider:

    domain = ""
    url = ""
    max_iterations = 0
    max_level = 0
    only_in_domain = True

    db_host = "62.152.59.93"
    db_port = "5432"
    db_name = "lyrics"
    db_user = "lyrics_user"
    db_pass = "lyrics250770"

    conn = None

    current_level = 0 # root level
    current_iter = 0
    current_encoding = 'utf-8'

    skip_iter = 0

    visited_urls = {}

    def __init__(self, max_iterations, max_level, only_in_domain = True):

	self.max_iterations = max_iterations
	self.max_level = max_level
	self.only_in_domain = only_in_domain

	""" Подключаемся к БД """
	self.conn = psycopg2.connect("host=%s port=%s dbname=%s user=%s password=%s" % (self.db_host, self.db_port, self.db_name, self.db_user, self.db_pass))


    def extract_title(self, document):
	
	stream = document
	
	title = re.findall(r"(?i)<title[^>]*>[\s\S]*?</title>", stream)
	
	if len(title) == 0:
	    return ""
	
	title = title[0]
	title = re.sub(r"(?i)<title[^>]*>|</title>", '', title)
	return title


    def extract_hrefs(self, document):

	stream = document
	hrefs = re.findall(r"(?i)<a [^>]*>[\s\S]*?</a>", stream)

	result = []

	for href in hrefs:

	    location = re.findall(r"(?i)href=([^>\ ]*)", href)
	    caption = re.findall(r"(?i)>([^<]*)</a>", href)

	    if len(location):
		if not location[0].strip('"').startswith("/"):
		    continue
	    else:
		continue

	    result.append((location[0] if len(location) else "", caption[0] if len(caption) else ""))

	return result

    def extract_body(self, document):

	the_extractor = extractor()
	return the_extractor.extract(document)

    def grab_url(self, the_url, caption, is_root):

	print "Grabbing url: %s" % the_url
	
	self.current_level += 1
	self.current_iter += 1

	if self.skip_iter != 0 and self.current_iter < self.skip_iter and not is_root:
	    self.current_level -= 1
	    return

	if the_url in self.visited_urls:
	    print "Url %s already visited" % the_url
	    self.current_level -= 1
	    return
	
	cur = self.conn.cursor()

	try:

	    time.sleep(1)
	    the_page = net.url_get_content(the_url)

	    """ Попытка определить кодировку урла """
	    enc = re.findall(r'(?i)charset=([a-z0-9\-]+)', the_page)

	    if len(enc) > 0:
		if enc[0].lower() != self.current_encoding:
		    print "Encoding is set to : %s" % enc[0].lower()
		    cur.execute("SET CLIENT_ENCODING TO '%s'" % enc[0].lower())
		    self.current_encoding = enc[0].lower()

	    """ Проверяем, есть ли этот url в БД """
	    cur.execute("SELECT id FROM public.documents WHERE url = %s LIMIT 1;", (the_url,))

	    if cur.rowcount > 0:
		print "Url exists in DB: %s" % the_url
		self.visited_urls[the_url] = 1

	    else:
		title = self.extract_title(the_page)
		body = self.extract_body(the_page)

		if is_root:
		    caption = title

		print "Inserting into documents"

		cur.execute("INSERT INTO public.documents (url, title, href_title, body, domain) VALUES (%s, %s, %s, %s, %s);", (the_url, title, caption, body, self.domain))
		self.conn.commit()

		self.visited_urls[the_url] = 1

	    if self.current_level < self.max_level and (self.current_iter < self.max_iterations or self.max_iterations == 0):

		href_data = self.extract_hrefs(the_page)

		for location, caption in href_data:
		    location = location.strip('"')
		    if location.startswith("/"):
			location = "http://" + self.domain + location
		    self.grab_url(location, caption, False)

	except Exception as detail:
	    print "Exception for url %s: %s" % (the_url, detail)
	    self.conn.rollback()

	self.current_level -= 1


    def work(self, the_url):

	print "Working on root url %s" % the_url

	""" Определяем домен """
	self.domain = urlparse(the_url).hostname

	print "Domain is %s" % self.domain

	self.current_iter = 0
	
	self.grab_url(the_url, '', True)
	












