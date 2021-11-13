#!/bin/sh

set -e

sleep 99 &
pid=$!
./kill $pid

ps -p $! --no-headers && exit 1

wait
