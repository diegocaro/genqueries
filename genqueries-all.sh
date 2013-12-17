genqueries="~/Research/temporalgraph/genqueries/genqueries-all"
rangefraq="0.001 0.01 0.1 0.5"

numq=2000

for dataset in `ls` ; do
	gzfile=`ls ../tgs-format/$dataset.txt.gz 2>/dev/null`;
	if [[ ! -f $gzfile ]]; then
		continue
	fi
	
	echo "gzcat $gzfile | $genqueries $dataset $numq $rangefraq"; 
done

