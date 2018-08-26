import os

os.system('ovs-ofctl add-flow s1 eth_type=0x800,in_port=3,ip_dst=224.0.0.80,action=enqueue:2:1')
os.system('ovs-ofctl add-flow s1 eth_type=0x800,in_port=2,ip_dst=224.0.0.80,action=enqueue:3:1')
os.system('ovs-ofctl add-flow s2 eth_type=0x800,in_port=4,ip_dst=224.0.0.80,action=enqueue:2:1,3:1')
os.system('ovs-ofctl add-flow s2 eth_type=0x800,in_port=2,ip_dst=224.0.0.80,action=enqueue:4:1,3:1')
os.system('ovs-ofctl add-flow s2 eth_type=0x800,in_port=3,ip_dst=224.0.0.80,action=enqueue:4:1,2:1')
os.system('ovs-ofctl add-flow s3 eth_type=0x800,in_port=4,ip_dst=224.0.0.80,action=enqueue:2:1,3:1')
os.system('ovs-ofctl add-flow s3 eth_type=0x800,in_port=2,ip_dst=224.0.0.80,action=enqueue:4:1,3:1')
os.system('ovs-ofctl add-flow s3 eth_type=0x800,in_port=3,ip_dst=224.0.0.80,action=enqueue:4:1,2:1')
os.system('ovs-ofctl add-flow s4 eth_type=0x800,in_port=3,ip_dst=224.0.0.80,action=enqueue:2:1')
os.system('ovs-ofctl add-flow s4 eth_type=0x800,in_port=2,ip_dst=224.0.0.80,action=enqueue:3:1')
