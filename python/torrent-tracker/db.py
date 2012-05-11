# To change this template, choose Tools | Templates
# and open the template in the editor.

import psycopg2

class db:

    USER = ''
    PASS = ''
    HOST = ''
    PORT = 5432
    DBNAME = ''

    _conn = None
    _cursor = None

    def __call__(self):
        return self

    def connect(self):

        self._conn = psycopg2.connect("host=%s port=%d dbname=%s user=%s password=%s"
        % (self.HOST, self.PORT, self.DBNAME, self.USER, self.PASS))

        self._cursor = self._conn.cursor()

        return 1

    def disconnect(self):

        self._cursor.close()
        self._conn.close()

        return 1

    def simpleQuery(self, query):

        return self._cursor.execute(query)

    def commit(self):
        
        return self._conn.commit()

    def rowsAffected(self):

        return self._cursor.rowcount

    def query(self, query, params):

        result = self._cursor.execute(query, params)

        return result

    def getAll(self, query, params):

        self._cursor.execute(query, params)

        result = self._cursor.fetchall()

        return result

    def getOne(self, query, params):

        self._cursor.execute(query, params)

        result = self._cursor.fetchone()

        return result

    







