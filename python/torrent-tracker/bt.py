#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      faito
#
# Created:     06.05.2010
# Copyright:   (c) faito 2010
# Licence:     <your licence>
#-------------------------------------------------------------------------------
#!/usr/bin/env python


# TODO:
# 1. tracker id
# 2. compact
# 3. no_user_id

from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
from bencode import bencode
from db import db

from urlparse import urlparse

import sys
import traceback
def formatExceptionInfo(maxTBlevel=5):
    cla, exc, trbk = sys.exc_info()
    excName = cla.__name__
    try:
        excArgs = exc.__dict__["args"]
    except KeyError:
        excArgs = "<no args>"
    excTb = traceback.format_tb(trbk, maxTBlevel)
    return (excName, excArgs, excTb)

class btTracker(BaseHTTPRequestHandler):

    BT_ANNOUNCE_INTERVAL = 360
    BT_ANNOUNCE_DEFAULT_NUMWANT = 50

    BT_ANNOUNCE_PARAMS_REQUIRED = ['info_hash', 'peer_id', 'port', 'uploaded',
                                'downloaded', 'left']

    BT_ANNOUNCE_PARAMS_OPTIONAL = ['ip', 'event', 'numwant',
                                    'no_peer_id', 'compact']

    BT_ANNOUNCE_ERRORS = {
        'BT_ERR_INVALID_REQUEST' : {'failure code': 100, 'failure reason': 'Invalid request'},
        'BT_ERR_MISSING_INFO_HASH' : {'failure code': 101, 'failure reason': 'Missing info hash'},
        'BT_ERR_MISSING_PEER_ID' : {'failure code': 102, 'failure reason': 'Missing peer id'},
        'BT_ERR_MISSING_PORT' : {'failure code': 103, 'failure reason': 'Missing port'},
        'BT_ERR_INVALID_INFO_HASH' : {'failure code': 150, 'failure reason': 'Invalid info hash'},
        'BT_ERR_INVALID_PEER_ID' : {'failure code': 151, 'failure reason': 'Invalid peer id'},
        'BT_ERR_INVALID_NUMWANT' : {'failure code': 152, 'failure reason': 'Invalid numwant'},
        'BT_ERR_INFO_HASH_NOT_FOUND' : {'failure code': 200, 'failure reason': 'Info hash not found'},
        'BT_ERR_CLIENT_IMPATIENT' : {'failure code': 500, 'failure reason': 'Client sent an eventless request before the specified time'},
        'BT_ERR_GENERIC' : {'failure code': 900, 'failure reason': 'Generic error'} }

    BT_SERVED_PATHS = {
        '/announce' : 'bt_ParseAnnounceRequest',
        '/info' : 'bt_Info'
    }


    def do_GET(self):
        try:

            request_data = urlparse(self.path)

            if request_data.path not in self.BT_SERVED_PATHS:
                raise Exception, 'BT_ERR_INVALID_REQUEST'

            # locals()[self.BT_SERVED_PATHS[request_data.path]](request_data)
            # o = getattr(self, self.BT_SERVED_PATHS[request_data.path])
            # p = o(request_data)
            getattr(self, self.BT_SERVED_PATHS[request_data.path])(request_data)
            # self.BT_SERVED_PATHS[request_data.path](request_data)

        except Exception, detail:
            # print detail
            self.bt_Failure(detail)

    def bt_Info(self, request_data):
        print 'here'
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write('Hello')

    def bt_ParseAnnounceRequest(self, request_data):
    # http://some.tracker.com:999/announce?info_hash=12345678901234567890&
    # peer_id=ABCDEFGHIJKLMNOPQRST&ip=255.255.255.255&port=6881&
    # downloaded=1234&left=98765&event=stopped
        try:
            params = {}
            tokens = request_data.query.split('&')
            for token in tokens:
                token = token.split('=')
                params[token[0]] = token[1]

            for req_param in params:
                self.bt_CheckAnnounceParam(req_param)

            peer_data = {}

            for req_param in self.BT_ANNOUNCE_PARAMS_REQUIRED:
                if not req_param in params:
                    raise Exception, 'Missing required announce param %s' % req_param
                peer_data[req_param] = params[req_param]

            for req_param in self.BT_ANNOUNCE_PARAMS_OPTIONAL:
                if req_param in params:
                    peer_data[req_param] = params[req_param]

            for req_param in peer_data:
                if req_param not in self.BT_ANNOUNCE_PARAMS_REQUIRED and req_param not in self.BT_ANNOUNCE_PARAMS_OPTIONAL:
                    del peer_data[req_param]

            peer_data['ip'] = self.client_address[0]

            self.bt_RegisterPeer(peer_data)
            
            torrent_data = self.bt_GetTorrentData(peer_data['info_hash'])

            print torrent_data

            response = {}
            # response['failure reason'] = ''
            # response['warning message'] = ''
            response['interval'] = self.BT_ANNOUNCE_INTERVAL
            # response['min_interval'] = ''
            # response['tracker id'] = ''
            response['complete'] = torrent_data['total_seeders']
            response['incomplete'] = torrent_data['total_leechers']
            response['peers'] = torrent_data['peers']

            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(bencode(response))

        except Exception, detail:
            print formatExceptionInfo()
            raise Exception, detail

    def bt_CheckAnnounceParam(self, request_param):
        try:
            if request_param == 'info_hash':
                pass

            elif request_param == 'peer_id':
                pass

            elif request_param == 'ip':
                pass

            elif request_param == 'port':
                pass

            elif request_param == 'downloaded':
                pass

            elif request_param == 'uploaded':
                pass

            elif request_param == 'left':
                pass

            elif request_param == 'event':
                pass

            elif request_param == 'no_peer_id':
                pass

            elif request_param == 'compact':
                pass

        except:
            pass

        finally:
            return 1



    def bt_Failure(self, reason):
        try:
            reason = str(reason)
            bt_response = ''
            if reason in self.BT_ANNOUNCE_ERRORS:
                bt_response = self.BT_ANNOUNCE_ERRORS[reason]
            else:
                bt_response = {'failure reason': reason}

            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(bencode(bt_response))

            print reason

        except Exception, detail:
            print detail
            self.send_error(500, bencode(self.BT_ANNOUNCE_ERRORS['BT_ERR_GENERIC']))


    def bt_RegisterPeer(self, peer_data):
        try:

            dbo = db()
            dbo.connect()

            sql = 'INSERT INTO tracker.peers (' + ','.join(["\"%s\"" % k for k in peer_data]) + ')'

            sql += ' VALUES (' + ','.join(["'%s'" % v for (k,v) in peer_data.items()]) + ')'

            dbo.simpleQuery(sql)
            dbo.commit()

            if dbo.rowsAffected == 0:
                raise Exception, 'Failed to insert peer to DB'

        except Exception:
            raise

        finally:
            dbo.disconnect()

    def bt_GetTorrentData(self, info_hash, no_peer_id = False, compact_peers = False, numwant = 50):
        try:
            dbo = db()
            dbo.connect()

            result = {}

            sql = 'SELECT COUNT(*) FROM tracker.peers WHERE info_hash = %s AND "left" <= 0'

            result['total_seeders'] = dbo.getOne(sql, (info_hash,))

            print result

            sql = 'SELECT COUNT(*) FROM tracker.peers WHERE info_hash = %s AND "left" > 0'

            result['total_leechers'] = dbo.getOne(sql, (info_hash,))

            if compact_peers:

                tmp_peers = dbo.fetchAll('SELECT ip, port FROM tracker.peers WHERE info_hash = %s ORDER BY "left" ASC LIMIT %s', (info_hash, numwant))

                result['peers'] = ''
                for tuple in tmp_peers:

                    bg_ip = htonl(tuple[0]) # to big endian
                    bg_port = htons(tuple[1]) # to big endian
                    result['peers'] += `bg_ip`
                    result['peers'] += `bg_port`

            else:

                if no_peer_id:
                    sql = 'SELECT ip, port FROM tracker.peers WHERE info_hash = %s ORDER BY RANDOM() LIMIT %s'
                else:
                    sql = 'SELECT peer_id, ip, port FROM tracker.peers WHERE info_hash = %s ORDER BY RANDOM() LIMIT %s'

                tmp_peers = dbo.getAll(sql, (info_hash, numwant))

                result['peers'] = []
                for tuple in tmp_peers:

                    if no_peer_id:
                        dict = {'ip': tuple[0], 'port': tuple[1] }
                    else:
                        dict = {'peer id': tuple[0], 'ip': tuple[1], 'port': tuple[2]}

                    result['peers'].append(dict)

            return result

        except Exception, msg:
            print msg
            raise Exception, 'BT_GENERIC_ERROR'

        finally:
            dbo.disconnect()










