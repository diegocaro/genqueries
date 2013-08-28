genqueries="~/Research/temporalgraph/genqueries/genqueries"
pointq="dirnei revnei"

rangefraq="0.001 0.01 0.1 0.5"
rangeq="dirnei_weak dirnei_strong revnei_weak revnei_strong"

numq=2000

for dataset in `ls` ; do
	gzfile=`ls ../tgl/$dataset*.gz 2>/dev/null`;
	if [[ ! -f $gzfile ]]; then
		continue
	fi
	#for numq in $numqueries; do
		for p in $pointq; do
			echo "gzcat $gzfile | $genqueries $p $numq > $dataset/$p"; 
		done
		
		for r in $rangeq; do
			for fraq in $rangefraq; do
				echo "gzcat $gzfile | $genqueries $r $numq $fraq > $dataset/${r}-${fraq}"; 
			done
		done
	#done
done

