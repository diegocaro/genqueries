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
vector<float> frange;
vector<uint> rangesize;

uint nodes=0;
uint edges=0;
uint contacts=0;
uint lifetime=0;


struct change {
	uint from, to, t, tend;
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
	fprintf(stderr, "%s <outputdir> <numberqueries> <list of percent range>\n", argv[0]);
}

// read temporal graph and populate adjlog
void read_stdin(vector<uint> q, vector<struct change> &cv) {
	uint a[4];
	uint t;
	
	uint i;
	t = 0;
	uint k=0;
	uint contacts_read=0;
	
	struct change c;
	while(EOF != scanf("%u %u %u %u", &a[0], &a[1], &a[2], &a[3])) {
		if (k == q.size()) break;

		if (contacts_read == q[k]) {
			c.from = a[0];
			c.to = a[1];
			c.t = a[2];
			c.tend = a[3];
			cv.push_back(c);
			k++;
		}
		
		
		if(contacts_read%1000000==0) fprintf(stderr,"Progress: %.2f%%\r", (float)contacts_read/contacts*100);
		contacts_read++;
	}
	
}

void printq(vector<struct change> &cv, int kindquery, int rangesize, FILE *f) {
	uint i;
	uint t;
	for (i = 0; i < cv.size(); i++) {
		fprintf(f,"%u ", kindquery);
		
		if (rangesize > cv[i].t) t = 0;
		else t = cv[i].t - rangesize;
		
		switch(kindquery) {
			case 0: // DIRNEI
			fprintf(f,"%u %u\n", cv[i].from, cv[i].t);
			break;
			case 1: // REVNEI
			fprintf(f,"%u %u\n", cv[i].to, cv[i].t);
			break;
			case 2: // DIRNEI RANGE
			case 3:
			fprintf(f,"%u %u %u\n", cv[i].from, t, cv[i].t);
			break;
			case 4: // REVNEI RANGE
			case 5:
			fprintf(f,"%u %u %u\n", cv[i].to, t, cv[i].t);
			break;
			case 6: // EDGE 
			case 10: // EDGE NEXT
			fprintf(f,"%u %u %u\n", cv[i].from, cv[i].to, cv[i].t);
			break;
			case 7: // EDGE RANGE
			case 8:
			fprintf(f,"%u %u %u %u\n", cv[i].from, cv[i].to, t, cv[i].t);
			break;
			
		}
		
		
		fprintf(f,"0\n");
	}
}


int main(int argc, char *argv[]) {
	map<string,int> kind;
	kind["edge"] = 6;
	kind["edge_weak"] = 7;
	kind["edge_strong"] = 8;
	kind["edge_next"] = 10;

	kind["snap_last"] = 9;

	kind["dirnei"] = 0;
	kind["revnei"] = 1;
	kind["dirnei_weak"] = 2;
	kind["dirnei_strong"] = 3;
	kind["revnei_weak"] = 4;
	kind["revnei_strong"] = 5;
	
	
	if (argc < 3) {
		usage(argv);
		return 1;
	}
	char *basedir = argv[1];
	nqueries = atoi(argv[2]);
	int i;	

	if (argc > 3) {
		//run range queries
		for(i = 3; i < argc; i++) {
			frange.push_back(atof(argv[i]));
		}
	}

	scanf("%u %u %u %u", &nodes, &edges, &lifetime, &contacts);
	
	for( i = 0; i < frange.size(); i++) {
		rangesize.push_back(lifetime*frange[i]);
	}

	vector<uint> q;
	vector<struct change> cv;
	
       /* initialize random seed: */
        srand (time(NULL));
	
	//int totalq;
	//totalq = nqueries*(3 + 6*frange.size()); //total of random contacts
	for(i = 0; i < nqueries; i++) {
		//q[i] = rand64()%contacts;
		q.push_back(rand()%contacts);
	}
	
	sort (q.begin(), q.end()); 
	
	read_stdin(q, cv);
	random_shuffle(cv.begin(), cv.end());
	
	FILE *f;
	char filename[1000];
        for (std::map<string,int>::iterator it=kind.begin(); it!=kind.end(); ++it) {
		
		if (it->second == 0 || it->second == 1 || it->second == 6 || it->second == 10) {
			sprintf(filename, "%s/%s", basedir, it->first.c_str());
			printf("Opening %s\n",filename);
			f = fopen(filename, "w");
			printq(cv, it->second, 0,f);
			fclose(f);
		}
		else if (it->second == 9) {
			sprintf(filename, "%s/%s", basedir, it->first.c_str());
			printf("Opening %s\n",filename);
			f = fopen(filename, "w");
			fprintf(f,"%u %u\n", it->second, lifetime-2);
			fclose(f);
		}
		else {
			for(i = 0; i < rangesize.size(); i++) {
				sprintf(filename, "%s/%s-%g", basedir, it->first.c_str(), frange[i]);
				printf("Opening %s\n",filename);
				f = fopen(filename, "w");
				printq(cv, it->second, rangesize[i],f);
				fclose(f);
				
			}
			
			
			
		}
        }
          
	
	//printq(cv);
	
	return 0;
}