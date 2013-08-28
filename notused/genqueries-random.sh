genqueries="~/Research/tools/genqueries.py"
pointq="dirnei revnei"

rangefraq="0.001 0.01 0.1 0.5"
rangeq="dirnei_weak dirnei_strong revnei_weak revnei_strong"

numq=2000

for dataset in `ls` ; do
	gzfile=`ls ../new_temporal/$dataset*.gz 2>/dev/null`;
	if [[ ! -f $gzfile ]]; then
		continue
	fi
	
	nodes=`gzcat $gzfile |head -n 1|cut -d " " -f 1`;
	maxtime=`gzcat $gzfile |head -n 1|cut -d " " -f 3`; 

	#for numq in $numqueries; do
		for p in $pointq; do
			echo "python $genqueries $nodes $maxtime $p $numq > $dataset/$p"; 
		done
		
		for r in $rangeq; do
			for fraq in $rangefraq; do
				echo "python $genqueries $nodes $maxtime $r $numq $fraq > $dataset/${r}-${fraq}"; 
			done
		done
	#done
done


## SNAPSHOT QUERIES
for dataset in `ls` ; do
	gzfile=`ls ../new_temporal/$dataset*.gz 2>/dev/null`;
	if [[ ! -f $gzfile ]]; then
		continue
	fi
	
	maxtime=`gzcat $gzfile |head -n 1|cut -d " " -f 3`; 

	echo "echo 9 0 > $dataset/snap-first"
	echo "echo 9 $maxtime > $dataset/snap-last"
done