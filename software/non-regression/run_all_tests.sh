#!/bin/sh

status="ok"

for test in $(find . -name '*.nreg')
do
    if ./$test
    then
        printf "Passed %s.\n" "$test"
    else
        printf "Failed %s!\n" "$test"
        status="not ok"
    fi
done

if [ "$status" = "ok" ]
then
    printf "\nAll test passed.\n"
else
    printf "\nSome test failed!!!\n"
    exit 1
fi

