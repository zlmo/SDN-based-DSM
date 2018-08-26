import socket
from time import sleep#,time,ctime
import datetime
from pyroute2 import IPRoute
import os

UDP_PORT='8888'
UDP_IP='224.0.0.88'

ip = IPRoute()
ethes=([x.get_attr('IFLA_IFNAME') for x in ip.get_links()])
print ethes[1]
os.system('route add -net '+UDP_IP+' netmask 255.255.255.255 '+ethes[1])
