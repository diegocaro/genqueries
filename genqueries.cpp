//
// Usage:
//		./genqueries <kindquery> <numberqueries> [<frange>]
//		Range should be a fraction



#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

int nqueries=0;
int kindquery=0;
double frange=0.0;
uint rangesize=0;

uint nodes=0;
uint changes=0;
uint maxtime=0;


struct change {
	uint from, to, t;
};

u_long rand64() {
	u_long r30, s30, t4, res;
	r30 = RAND_MAX*rand()+rand();
	s30 = RAND_MAX*rand()+rand();
	t4  = rand() & 0xf;

	res = (r30 << 34) + (s30 << 4) + t4;
	return res;
}

void usage(char *argv[]) {
	fprintf(stderr, "%s <kindquery> <numberqueries> [<frange>]\n", argv[0]);
}

// read temporal graph and populate adjlog
void read_stdin(vector<uint> q, vector<struct change> &cv) {
	uint a[4];
	uint t;
	
	uint i;
	t = 0;
	uint k=0;
	uint changes_read=0;
	
	struct change c;
	while(EOF != scanf("%u %u %u %u", &a[0], &a[1], &a[2], &a[3])) {
		if (k == q.size()) break;

		if (changes_read == q[k]) {
			c.from = a[0];
			c.to = a[1];
			c.t = a[2];
			cv.push_back(c);
			k++;
		}
		
		
		if(changes_read%1000000==0) fprintf(stderr,"Progress: %.2f%%\r", (float)changes_read/changes*100);
		changes_read++;
	}
	
}

void printq(vector<struct change> &cv) {
	uint i;
	uint t;
	for (i = 0; i < cv.size(); i++) {
		printf ("%u ", kindquery);
		
		if (rangesize > cv[i].t) t = 0;
		else t = cv[i].t - rangesize;
		
		switch(kindquery) {
			case 0: // DIRNEI
			printf("%u %u\n", cv[i].from, cv[i].t);
			break;
			case 1: // REVNEI
			printf("%u %u\n", cv[i].to, cv[i].t);
			break;
			case 2: // DIRNEI RANGE
			case 3:
			printf("%u %u %u\n", cv[i].from, t, cv[i].t);
			break;
			case 4: // REVNEI RANGE
			case 5:
			printf("%u %u %u\n", cv[i].to, t, cv[i].t);
			break;
			case 7: // CELL 
			
			break;
			case 8: // CELL RANGE
			case 9:
			
			break;
			
		}
		
		
		printf("0\n");
	}
}


int main(int argc, char *argv[]) {
	map<string,int> kind;
	kind["CELL"] = kind["cell"] = 6;
	kind["CELL_WEAK"] = kind["cell_weak"] = 7;
	kind["CELL_STRONG"] = kind["cell_strong"] = 8;

	kind["DIRNEI"] = kind["dirnei"] = 0;
	kind["REVNEI"] = kind["revnei"] = 1;
	kind["DIRNEI_WEAK"] = kind["dirnei_weak"] = 2;
	kind["DIRNEI_STRONG"] = kind["dirnei_strong"] = 3;
	kind["REVNEI_WEAK"] = kind["revnei_weak"] = 4;
	kind["REVNEI_STRONG"] = kind["revnei_strong"] = 5;
	
	
	if (argc < 3) {
		usage(argv);
		return 1;
	}
	
	if (kind.count(argv[1]) == 0 ) {
		usage(argv);
		return 1;
	}
	
	kindquery = kind[argv[1]];
	nqueries = atoi(argv[2]);
	if (kindquery >= 2 && kindquery <=5) {
		if (argc < 4) {
			usage(argv);
			return 1;
		}
		frange = atof(argv[3]);
	}
	
	

	scanf("%u %u %u", &nodes, &changes, &maxtime);
	rangesize = (uint) (maxtime*frange);
	
	vector<uint> q;
	vector<struct change> cv;
	
       /* initialize random seed: */
        srand (time(NULL));
	int i;	
	for(i = 0; i < nqueries; i++) {
		//q[i] = rand64()%changes;
		q.push_back(rand()%changes);
	}
	
	sort (q.begin(), q.end()); 
	
	read_stdin(q, cv);
	random_shuffle(cv.begin(), cv.end());
	
	printq(cv);
	
	return 0;
}