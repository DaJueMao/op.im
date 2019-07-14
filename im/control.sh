#!/bin/bash

TARGET=$(pwd)
SUBJARVIS1=$TARGET/bin/server
SUBJARVIS2=$TARGET/bin/client
CONF=$TARGET/conf/im.conf

function usage() {
    printf "Usage: %s [-s(start) | -q(stop) | -r(restart) -c(connect)]\n" "$0"
}
	
function startServer() {
	pid=$(pidof $SUBJARVIS1)
	if [ $? -eq 0 ];then
        echo  "`date +%Y-%m-%d,%H:%m:%s`:server is already runing ···"
	else
        ip=$(awk -F: '/ip/{print $2}' $CONF)
        port=$(awk -F: '/port/{print $2}' $CONF)
		$SUBJARVIS1 $ip $port
        echo "`date +%Y-%m-%d,%H:%m:%s`:server start success ···"
	fi
}

function stopServer() {
	pid=$(pidof $SUBJARVIS1)
	if [ $? -eq 0 ];then
		kill -9 $pid
        echo "`date +%Y-%m-%d,%H:%m:%s`:server stop success ···"
	else
        echo "`date +%Y-%m-%d,%H:%m:%s`:server is already stop ···"
	fi
}

function restartServer() {
	stopServer
    echo "`date +%Y-%m-%d,%H:%m:%s`:server is already stop ···"
	startServer
    echo "`date +%Y-%m-%d,%H:%m:%s`:server is already restart ···"
}

function connect() {
    pid=$(pidof $SUBJARVIS2)
    if [$? -eq 0];then
        echo  "`date +%Y-%m-%d,%H:%m:%s`:connect success ···"
	else
        ip=$(awk -F: '/ip/{print $2}' $CONF)
        port=$(awk -F: '/port/{print $2}' $CONF)
        user_name=$(awk -F: '/user_name/{print $2}' $CONF)
        school=$(awk -F: '/school/{print $2}' $CONF)
        $SUBJARVIS2 $ip $port $user_name $school
        echo "`date +%Y-%m-%d,%H:%m:%s`:connect success ···"
	fi
}

if [ $# -ne 1 ] ; then
	usage
fi 

case $1 in 
	-s | 'start' )
	startServer
	;;
	-q | 'stop' )
	stopServer
	;;
	-r | 'restart')
	restartServer
	;;
	-c | 'connect')
	connect
	;;
esac
