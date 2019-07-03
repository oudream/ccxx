#!/usr/bin/env bash

# kill -SIGINT -<pid>

iPid=14145; for (( i = 0 ; i < 11 ; i++ ));do kill -s 2 ${iPid};done;
