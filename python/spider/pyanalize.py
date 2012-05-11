# -*- coding: utf-8 -*-

__author__="faito"
__date__ ="$19.12.2010 22:22:18$"

from extractor import extractor
from spider import spider
import net

import sys

#TEST_URL = "http://www.sing365.com/music/lyric.nsf/94-hours-lyrics-as-i-lay-dying/ec08b347f9982e7b48256d4f00086029"
#TEST_URL = "http://iichan.ru/b/"
#TEST_URL = "http://musdb.net"
#TEST_URL = "http://www.elyrics.net/read/a/as-i-lay-dying-lyrics/94-hours-lyrics.html"
#TEST_URL = "http://www.lyricsmode.com/lyrics/a/as_i_lay_dying/94_hours.html"
#TEST_URL = "http://www.sankakucomplex.com/2010/12/20/wada-were-so-bad-well-have-to-rebuild-our-brands/"
#TEST_URL = "http://www.sankakucomplex.com/2010/12/19/iaijutsu-vs-fast-draw/"
#TEST_URL = "http://habrahabr.ru/blogs/google/110253/"
#TEST_URL = "http://iichan.ru/b/"

def main():

    if len(sys.argv) < 1:
	print "Specify URL"
	sys.exit(1)

    try:

	the_spider = spider(0, 10, True)
	the_spider.work(sys.argv[1])
    
    except:
	raise
    

if __name__ == "__main__":
    main()






