# -*- coding: utf-8 -*-

__author__="faito"
__date__ ="$19.12.2010 22:31:25$"

import re
from itertools import dropwhile
import net
import sys

class extractor:

    THRESHOLD = 20
    HREF_ABSORBTION_LEVEL = 5
    OBJECT_ABSORBTION_LEVEL = 5

    def __init__(self):
	pass

    def extract(self, document):

	stream = document

	""" Удаляем javascript """
	stream = re.sub(r"(?i)<script[^>]*>[\s\S]*?</script>", '', stream)
	""" Удаляем css """
	stream = re.sub(r"(?i)<style[^>]*>[\s\S]*?</style>", '', stream)
	""" Удаляем HTML-комментарии """
	stream = re.sub(r"(?i)<!--[\s\S]*?-->", '', stream)
	""" Удаляем <noscript> """
	stream = re.sub(r"(?i)<noscript[^>]*>[\s\S]*?</noscript>", '', stream)
	""" blockquote """
	stream = re.sub(r"(?i)<blockquote[^>]*>|</blockquote>|<span[^>]*>|</span>|<p [^>]*>|</p>|<p>|<strong[^>]*>|</strong>|<b>|</b>", '', stream)
	""" Удаляем iframe """
	stream = re.sub(r"(?i)<iframe[^>]*>[\s\S]*?</iframe>", '', stream)
	""" Удаляем onclick, class, style"""
	stream = re.sub(r'(?i)onclick="([^"]*)"|class="([^"]*)"|style="([^"]*)"', '', stream)
	
	""" Если тег приклеен к тексту, его надо отделить пробелом """
	stream = re.sub(r'(?i)([^<>]+)<([^<>]+)>', r'\1    <\2>', stream)
	stream = re.sub(r'(?i)([^<>]+)<([^<>]+)>', r'\1    <\2>', stream)
	stream = re.sub(r'(?i)<([^<>]+)>([^<>]+)', r'<\1>    \2', stream)
	stream = re.sub(r'(?i)<([^<>]+)>([^<>]+)', r'<\1>    \2', stream)
	stream = re.sub(r'(?i)([^<>]+)><([^<>]+)', r'\1>    <\2', stream)
	stream = re.sub(r'(?i)([^<>]+)><([^<>]+)', r'\1>    <\2', stream)

	p = re.compile(r'(?i)\s+')
	stream = p.split(stream)

	""" Первый этап: сращиваем теги """
	stream_w_tags = []
	tag_flag = False

	for word in stream:
	    if word.startswith('<'):
		tag_flag = True

	    if word.endswith('>'):
		tag_flag = False

	    if (tag_flag and not word.startswith('<')) or (word.endswith('>') and not word.startswith('<')):
		last_word = stream_w_tags.pop() if len(stream_w_tags) > 0 else ""
		last_word += " " + word
		stream_w_tags.append(last_word)
	    else:
		stream_w_tags.append(word)

	""" Второй этап: обработка потока """
	tuples = []
	distance = 0
	br = False
	words_in_a_row = 0
	for word in stream_w_tags:
	    """ Считаем расстояние """
	    if word.startswith("<"):
		if word[:3].lower() == "<br":
		    distance += 0
		    br = True
		elif words_in_a_row >= self.HREF_ABSORBTION_LEVEL and (word[:2].lower() == "<a" or word[:4].lower() == "</a>"):
		    distance += 0
		    """ Ссылка is fine too """
		    tuples.append((word, distance, br))
		    distance = 0
		    br = False
		elif words_in_a_row >= self.OBJECT_ABSORBTION_LEVEL and (word[:7].lower() == "<object" or word[:9].lower() == "</object>"):
		    distance += 0
		    """ Object is fine too """
		    tuples.append((word, distance, br))
		    distance = 0
		    br = False
		elif words_in_a_row >= self.OBJECT_ABSORBTION_LEVEL and (word[:6].lower() == "<param" or word[:8].lower() == "</param>"):
		    distance += 0
		    """ Param is fine too """
		    tuples.append((word, distance, br))
		    distance = 0
		    br = False
		elif words_in_a_row >= self.OBJECT_ABSORBTION_LEVEL and (word[:6].lower() == "<embed" or word[:8].lower() == "</embed>"):
		    distance += 0
		    """ Embed is fine too """
		    tuples.append((word, distance, br))
		    distance = 0
		    br = False
		else:
		    distance += 1
		    br = False
		    words_in_a_row = 0
	    else:
		""" Нашли не-тег (слово), заносим его в граф и обнуляем дистанцию """
		tuples.append((word, distance, br))
		distance = 0
		br = False
		words_in_a_row += 1

	""" Третий этап: находим наибольшую последовательность слов с наименьшим расстоянием """
	sequences = []
	tmp_seq = ""
	lowest_distance = None
	counter = 0
	node_counter = 0

	for tuple in tuples:
	    counter += 1
	    if lowest_distance is None or tuple[1] < lowest_distance:
		lowest_distance = tuple[1]
		tmp_seq = tuples[counter-2][0] if counter >= 2 else ""
		node_counter = 1 if tmp_seq != "" else 0

	    if tuple[1] == lowest_distance:

		if tuple[2] is True:
		    tmp_seq += "\n" + tuple[0]
		    node_counter += 1
		else:
		    tmp_seq += " " + tuple[0]
		    node_counter += 1

	    else:
		if tmp_seq != "":
		    sequences.append((tmp_seq.strip(), node_counter))
		    tmp_seq = tuples[counter-1][0] if counter >= 1 else ""
		    node_counter = 1 if tmp_seq != "" else 0

	actual_data = sequences[:]
	actual_data = dropwhile(lambda x: x[1] < self.THRESHOLD, actual_data)
	actual_data = sorted(actual_data, lambda x,y: x[1] - y[1], None, True)

	try:
	    return actual_data[0][0]
	    #return actual_data.next()[0]
	except:
	    return "Nothing"


if __name__ == "__main__":
    #http://danbooru.donmai.us/post/show/812419/absurdres-barefoot-cherry_blossoms-claws-green_hai
    if len(sys.argv) < 2:
	print "Specify URL"
	sys.exit(1)

    try:

	the_extractor = extractor()

	the_text = net.url_get_content(sys.argv[1])

	print the_extractor.extract(the_text)

    except:
	raise