#!/bin/sh

./ui_test > ui_test.log 2>&1 &
# ./test > test.log 2>&1 &
disown
