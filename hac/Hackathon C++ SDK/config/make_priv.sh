#!/bin/sh

chain=prv

hdac-cli $chain stop
sleep 3

cp ~/.hdac/$chain/hdac.conf .
rm -rf ~/.hdac/$chain
hdac-util create $chain
cp params-priv.dat ~/.hdac/$chain/params.dat
cp hdac.conf ~/.hdac/$chain/

hdacd $chain -daemon -gen=1 -rpcthreads=50 -autosubscribe=streams -autoimportaddress
