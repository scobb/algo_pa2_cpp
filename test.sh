INPUT='in([0-9]+)\.txt'
OUTPUT='out([0-9]+)\.txt'
IN_DIRS=(small med)
pass='true'
for dir in ${IN_DIRS[@]}; do
    echo DIR: $dir
    for file in $(ls $dir); do
        if [[ $file =~ $INPUT ]]; then
            num="${BASH_REMATCH[1]}"
            ./Algo_PA2 $dir/$file > test.out
            if [[ $(diff test.out $dir/out${num}.txt) != "" ]]; then
                echo "FAIL: $file: $(diff test.out $dir/out${num}.txt)"
                pass='false'
            fi
        fi
    done
done
if [[ $pass = 'true' ]]; then
    echo PASS
fi
