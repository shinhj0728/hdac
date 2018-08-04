#!/bin/sh

hdac-cli hdac stop
sleep 3

cp ~/.hdac/hdac/hdac.conf .
rm -rf ~/.hdac/hdac
hdac-util create hdac
cp params-hdac.dat ~/.hdac/hdac/params.dat
cp hdac.conf ~/.hdac/hdac/

hdacd hdac -daemon -gen=1 -miningrequirespeers=0 -autosubscribe=streams -autoimportaddress
