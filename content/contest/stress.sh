for ((i = 1; ; ++i)); do  # if they are same then will loop forever
    echo $i
    ./gen $i > int
    ./a.out < int > out1 || break
    ./brute < int > out2 || break
    diff -w out1 out2 || break
done
