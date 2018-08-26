import os
#S1
os.system('ovs-vsctl set port s1-eth1 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s1-eth2 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s1-eth3 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
#s2
os.system('ovs-vsctl set port s2-eth1 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s2-eth2 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s2-eth3 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s2-eth4 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
#s3
os.system('ovs-vsctl set port s3-eth1 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s3-eth2 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s3-eth3 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s3-eth4 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
#s4
os.system('ovs-vsctl set port s4-eth1 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s4-eth2 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
os.system('ovs-vsctl set port s4-eth3 qos=@newqos -- --id=@newqos create qos type=linux-htb queues=0=@q0,\
1=@q1 other-config:max-rate=25000000 -- --id=@q0 create queue other-config:priority=900 \
other-config:max-rate=25000000 other-config:min-rate=25000000 -- --id=@q1 create queue other-config:priority=700 other-config:max-rate=15000000 other-config:min-rate=15000000')
