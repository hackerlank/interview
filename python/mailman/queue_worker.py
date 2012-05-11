#!/usr/bin/python
# -*- coding: utf-8

import urllib
from xml.etree.ElementTree import ElementTree as ET
from xml.dom import minidom
import subprocess
import os
from pipes import quote
import re

import MySQLdb
import tempfile

GET_URL = "";
FINISH_URL = ""
MAILMAN_SYS = [
    'admin',
    'bounces',
    'confirm',
    'join',
    'leave',
    'owner',
    'request',
    'subscribe',
    'unsubscribe'
]

WHO = "localhost"
ADMIN = "admin@localhost"

db = MySQLdb.connect(host="", user="", passwd="", db="", charset='utf8')
cursor = db.cursor()
queue = minidom.parse(urllib.urlopen(GET_URL))
entries = []

for entry in queue.getElementsByTagName('entry'):
    system_name = filter(lambda x: x.localName == 'system_name', entry.childNodes)[0].childNodes[0].data
    args = map(lambda x: x.childNodes[0].data, filter(lambda x: x.localName == 'args', entry.childNodes))

    id = filter(lambda x: x.localName == 'id', entry.childNodes)[0].childNodes[0].data
    try:
        parent_id = filter(lambda x: x.localName == 'parent_id', entry.childNodes)[0].childNodes[0].data
    except:
        parent_id = None

    e = {
        'system_name': system_name,
        'args': args,
        'id': id,
        'parent_id': parent_id
    }

    entries.append(e)

sorted(entries, key=lambda x: x['id'])

def args_to_dict(args):

    d = {}
    for arg in args:
        t = tuple(arg[1:-1].split(','))
        d[t[0]] = t[1]

    return d

for entry in entries:

    params = args_to_dict(entry['args'])
    err_code = 0
    err_msg = ''

    sh_command = ''
    sh_args = ''
    use_system = False

    mailbox, domain = params['title'].split('@')

    if entry['system_name'] == 'create_maillist':
        if not 'title' in params:
            raise ValueError('Empty title')

        use_system = True
        sh_command = '/usr/lib/mailman/bin/newlist -l ru -u %s -q %s %s %s' % (WHO, quote(params['title']), ADMIN, '123456')
        errors = ''

        for suf in MAILMAN_SYS:
            string = "%s-%s:\t \"|/var/lib/mailman/mail/mailman %s %s\"" % (mailbox, suf, suf, mailbox)
            os.system("echo %s >> %s" % (quote(string), "/etc/aliases"))

            address = mailbox + '-' + suf + '@' + domain
            goto = mailbox + '-' + suf + '@' + WHO

            sql = "INSERT IGNORE INTO alias(address, goto, domain, active) VALUES ('%(address)s', '%(goto)s', '%(domain)s', 1)" % { 'address': address, 'goto': goto, 'domain': domain}
            cursor.execute(sql)

        string = "%s:\t \"|/var/lib/mailman/mail/mailman %s %s\"" % (params['title'].split('@')[0], 'post', params['title'].split('@')[0])
        os.system("echo %s >> %s" % (quote(string), "/etc/aliases"))

        address = params['title']
        goto = mailbox + '@' + WHO
        sql = "INSERT IGNORE INTO alias(address, goto, domain, active) VALUES ('%(address)s', '%(goto)s', '%(domain)s', 1)" % { 'address': address, 'goto': goto, 'domain': domain}
        cursor.execute(sql)

        db.commit()
        os.system("newaliases")

    elif entry['system_name'] == 'drop_maillist':
        if not 'title' in params:
            raise ValueError('Empty title')

        use_system = True
        sh_command = '/usr/lib/mailman/bin/rmlist %s' % (quote(mailbox))
        #sh_args = "%s" % (quote(mailbox))
        errors = ''

        string = "cat /etc/aliases | grep -v -P '%s(|-admin|-bounces|-confirm|-join|-leave|-owner|-request|-subscribe|-unsubscribe):' | grep -v -P '$^' > /etc/aliases.backup_t" % (params['title'].split('@')[0])
        os.system(string)
        os.system("mv /etc/aliases.backup_t /etc/aliases")
        os.system("newaliases")

        for suf in MAILMAN_SYS:
            address = mailbox + '-' + suf + '@' + domain
            sql = "DELETE FROM alias WHERE address = '%(address)s'" % { 'address': address }
            cursor.execute(sql)

        address = params['title']
        sql = "DELETE FROM alias WHERE address = '%(address)s'" % { 'address': address }

        db.commit()

    elif entry['system_name'] == 'add_email_to_maillist':
        if not 'title' in params:
            raise ValueError('Empty title')

        if not 'email' in params:
            raise ValueError('Empty email')

        use_system = True
        sh_command = "echo %s | /usr/lib/mailman/bin/add_members -r - %s" % (quote(params['email']), quote(params['title'].split('@')[0]))
        errors = ''
    elif entry['system_name'] == 'delete_email_from_maillist':
        if not 'title' in params:
            raise ValueError('Empty title')

        if not 'email' in params:
            raise ValueError('Empty email')

        use_system = True
        sh_command = "echo %s | /usr/lib/mailman/bin/remove_members -f - %s" % (quote(params['email']), quote(params['title'].split('@')[0]))
        errors = ''

    print sh_command, sh_args
    if not use_system:
        p = subprocess.Popen(executable=sh_command, args=sh_args, stdout=None, stderr=subprocess.PIPE, shell=True)
        output, errors = p.communicate()
        err_code = p.returncode
    else:
        err_code = os.system(sh_command)


    if err_code != 1:
        err_msg = errors

    if err_code == 1:
        err_code = 0

    print FINISH_URL % (entry['id'], err_code, err_msg)
    result = minidom.parse(urllib.urlopen(FINISH_URL % (entry['id'], err_code, err_msg)))