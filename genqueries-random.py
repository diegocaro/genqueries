#
# Usage:
#		./genqueries.py <nodes> <maxtime> <kindquery> <numberqueries> [<frange>]
#		Range should be a fraction
#
#
from sys import argv,stderr
from random import randint

def genNeiPoint(nodes, maxtime, numqueries):
	q = []
	for i in range(numqueries):
		qnode = randint(0, nodes-1)
		qtime = randint(0, maxtime-1)
		
		q.append( (qnode, qtime))
	
	return q

def genNeiRange(nodes, maxtime, numqueries, rangefrac):
	rangesize = int(maxtime*rangefrac)
	
	q = []
	for i in range(numqueries):
		qnode = randint(0, nodes-1)
		qtime = randint(0, maxtime-1-rangesize)
		
		q.append( (qnode, qtime, qtime+rangesize))
	
	return q

kind = dict()
kind['CELL'] = 6
kind['CELL_WEAK'] = 7
kind['CELL_STRONG'] = 8

kind['DIRNEI'] = 0
kind['REVNEI'] = 1
kind['DIRNEI_WEAK'] = 2
kind['DIRNEI_STRONG'] = 3
kind['REVNEI_WEAK'] = 4
kind['REVNEI_STRONG'] = 5


nodes = int(argv[1])
maxtime = int(argv[2])
kindquery = kind[argv[3].upper()]
numqueries = int(argv[4])
if kindquery >= 2 and kindquery <= 5:
	rangefrac = float(argv[5])



if kindquery in (kind['DIRNEI'],kind['REVNEI']):
	print >> stderr, 'Text' "point query"
	res = genNeiPoint(nodes, maxtime, numqueries)
	
if kindquery in (kind['DIRNEI_WEAK'], kind['DIRNEI_STRONG'], kind['REVNEI_WEAK'], kind['REVNEI_STRONG']):
	print >> stderr, "range query"
	res = genNeiRange(nodes, maxtime, numqueries, rangefrac)

#newres = map(lambda p: (kindquery,)+p, res)

for q in res:
	print kindquery, 
	for a in q:
		print a,
	print
	print "0"