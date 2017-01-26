#!/usr/bin/fish

set scenarios "best" "average" "worst"
set algs "insertion" "counting"
set args "500" "250" "30"

set csv "results.csv"

echo "" > $csv

for alg in $algs
    for i in $scenarios

        # Skip if average and counting sort
        if test $alg != "counting" -o $i != "average"
            set msg "$alg sort $i case"
            echo $msg
            echo $msg >> $csv

            ./sorting_test $alg $i $args >> $csv
        end
    end
end
