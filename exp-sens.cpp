// <outdegree> <node>
// <contacts> <edge>

#include "btree_map.h"
#include "btree_set.h"
#include <algorithm>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cassert>
using namespace std;
using namespace btree;

int uniform(int m) { return rand()%m;}

class Edge {
public:
	int u,v;
	
	inline bool operator<(const Edge& e) const {
	    long c1=((long)u << 32) + v;
		long c2=((long)e.u << 32) + e.v;
		return (c1 < c2);
	}
};

int main( int argc, char *argv[]) {
	srand ( unsigned ( time(0) ) );
	
        int nodes, edges, lifetime, contacts;
		int u,v,a,b;
	
		int samples = 30;
		
		if(argc < 2) {
			fprintf(stderr,"Usage: %s <outputdir>\n",argv[0]);
			return 1;
		}
		
		char *basedir = argv[1];
		char filename[10000];
		FILE *f;
		
        scanf("%d %d %d %d", &nodes, &edges, &lifetime, &contacts);       
		
		btree_map<int, btree_set<int> > succ;	btree_map<int, vector<int> > neivrt;
		btree_map<int, btree_set<int> > pred;	btree_map<int, vector<int> > revvrt;
		btree_map<Edge, int> edgc;				btree_map<int, vector<Edge> > ctsedg;
		btree_map<int, int> vrtc;				btree_map<int, vector<int> > ctsvrt;
		
		Edge e;
		
		int c = 0;
        while ( c < contacts ) {

			c++;
			if (c%100000==0) fprintf(stderr, "Processing: %.2f%%\r", (float)c/contacts*100);

			if (EOF == scanf("%d %d %d %d", &u, &v, &a, &b)) {
				fprintf(stderr,"No input read...\n");
				return -1;
			}
			
			succ[u].insert(v);
			pred[v].insert(u);
			vrtc[u] += 1;
			
			e.u = u; e.v = v;
			edgc[e] += 1;
        }
		
		for(btree_map<int,btree_set<int> >::iterator its = succ.begin(); its != succ.end(); ++its) {
			neivrt[its->second.size()].push_back(its->first);
		}
		succ.clear();
		
		sprintf(filename,"%s/nodes_by_outdegree",basedir);
		f = fopen(filename,"w");
		for(btree_map<int, vector<int> >::iterator itv=neivrt.begin(); itv!=neivrt.end(); ++itv) {
			random_shuffle ( itv->second.begin(), itv->second.end() );
			for(int i = 0; i < min(samples, (int)itv->second.size()); i++) {
				fprintf(f, "%d %d\n", itv->first, itv->second[i]);
			}
			
		}
		fclose(f);
		neivrt.clear();
		
		
		for(btree_map<int,btree_set<int> >::iterator its = pred.begin(); its != pred.end(); ++its) {
			revvrt[its->second.size()].push_back(its->first);
		}
		pred.clear();
		
		sprintf(filename,"%s/nodes_by_indegree",basedir);
		f = fopen(filename,"w");
		for(btree_map<int, vector<int> >::iterator itv=revvrt.begin(); itv!=revvrt.end(); ++itv) {
			random_shuffle ( itv->second.begin(), itv->second.end() );
			for(int i = 0; i < min(samples, (int)itv->second.size()); i++) {
				fprintf(f, "%d %d\n", itv->first, itv->second[i]);
			}
			
		}
		fclose(f);
		revvrt.clear();
		
		
		for(btree_map<int,int>::iterator it = vrtc.begin(); it != vrtc.end(); ++it) {
			ctsvrt[it->second].push_back(it->first);
		}
		vrtc.clear();
		
		sprintf(filename,"%s/nodes_by_contacts",basedir);
		f = fopen(filename,"w");
		for(btree_map<int, vector<int> >::iterator itv=ctsvrt.begin(); itv!=ctsvrt.end(); ++itv) {
			random_shuffle ( itv->second.begin(), itv->second.end() );
			for(int i = 0; i < min(samples, (int)itv->second.size()); i++) {
				fprintf(f, "%d %d\n", itv->first, itv->second[i]);
			}
			
		}
		fclose(f);
		ctsvrt.clear();
		
		
		for(btree_map<Edge,int>::iterator ite = edgc.begin(); ite != edgc.end(); ++ite) {
			ctsedg[ite->second].push_back(ite->first);
		}
		edgc.clear();
		
		sprintf(filename,"%s/edges_by_contacts",basedir);
		f = fopen(filename,"w");
		for(btree_map<int, vector<Edge> >::iterator itv=ctsedg.begin(); itv!=ctsedg.end(); ++itv) {
			random_shuffle ( itv->second.begin(), itv->second.end() );
			for(int i = 0; i < min(samples, (int)itv->second.size()); i++) {
				fprintf(f, "%d %d %d\n", itv->first, itv->second[i].u, itv->second[i].v );
			}
			
		}
		fclose(f);
		ctsvrt.clear();
       
        return 0;
}