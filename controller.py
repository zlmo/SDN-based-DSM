# Copyright (C) 2011 Nippon Telegraph and Telephone Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import logging
import struct

from ryu.base import app_manager
from ryu.app.wsgi import ControllerBase
from ryu.controller import mac_to_port
from ryu.controller import ofp_event
from ryu.controller.handler import MAIN_DISPATCHER,CONFIG_DISPATCHER
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ether
from ryu.ofproto import inet
from ryu.ofproto import ofproto_v1_3
from ryu.lib import igmplib
from ryu.lib import dpid as dpid_lib
from ryu.lib.dpid import str_to_dpid
from ryu.lib.packet import packet
from ryu.lib.packet import ethernet
from ryu.lib.packet import ipv4
from ryu.lib.packet import igmp
from ryu.lib.packet import arp
from ryu.lib import hub
from operator import attrgetter

from ryu.topology import event, switches
from ryu.topology.api import get_switch, get_link
from collections import defaultdict

import networkx as nx


class SimpleSwitch13(app_manager.RyuApp):
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(SimpleSwitch13, self).__init__(*args, **kwargs)
        self.mac_to_port = {}
        self.topology_api_app=self
        self.net=nx.DiGraph()
        self.switches={}
        self.links={}
        self.nodes={}
        self.dp_action={}
        self.group_mem={}
        self.group_mem_num={}
        self.ip_num=0
        self.ipv4_src=None
        self.ipv4_dst=None
        

    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, ev):
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        match = parser.OFPMatch()
        actions = [parser.OFPActionOutput(ofproto.OFPP_CONTROLLER, ofproto.OFPCML_NO_BUFFER)]
        self.add_flow2(datapath, 0, match, actions)
    

    def add_flow3(self, datapath, priority, match, actions):
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        inst = [parser.OFPInstructionActions(ofproto.OFPIT_APPLY_ACTIONS,actions)]
        mod = parser.OFPFlowMod(datapath=datapath, priority=priority, match=match, instructions=inst)
        datapath.send_msg(mod)
    def add_flow(self, datapath, src, dst, src_ip, dst_ip, actions):
        print str(datapath)+":"+str(src)+"<->"+str(dst)        
    
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        inst = [parser.OFPInstructionActions(ofproto.OFPIT_APPLY_ACTIONS,
                                             actions)]
        match = parser.OFPMatch(eth_src=src, eth_dst=dst
                , eth_type = 0x800, ipv4_src=src_ip, ipv4_dst=dst_ip)
        mod = parser.OFPFlowMod(datapath=datapath, cookie=0, cookie_mask=0, table_id=0, command=ofproto_v1_3.OFPFC_ADD, 
                            idle_timeout=0, hard_timeout=0, priority=10, buffer_id = ofproto_v1_3.OFP_NO_BUFFER, flags=ofproto_v1_3.OFPFF_SEND_FLOW_REM,
                            match=match, instructions=inst)
        datapath.send_msg(mod)
    def send_group_mod(self, datapath):
        ofproto = datapath.ofproto
        ofp_parser = datapath.ofproto_parser
        dpid=datapath.id


        weight_1 = 50
        watch_port = ofproto_v1_3.OFPP_ANY
        watch_group = ofproto_v1_3.OFPQ_ALL


        num=len(self.dp_action[dpid])
        for i in range(0,num):
            actions=self.dp_action[dpid][i]
            bucket_1=ofp_parser.OFPBucket(weight_1, watch_port, watch_group, actions)
            buckets=[]
            buckets.append(bucket_1)
        group_id = 50
        req = ofp_parser.OFPGroupMod(datapath, ofproto.OFPGC_ADD,
                                     ofproto.OFPGT_SELECT, group_id, buckets)

        datapath.send_msg(req)
        
    
    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def _packet_in_handler(self, ev):
        msg = ev.msg
        datapath = msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        dpid = datapath.id
        in_port = msg.match['in_port']

        pkt = packet.Packet(msg.data)
        eth = pkt.get_protocols(ethernet.ethernet)[0]
        pkt_igmp=pkt.get_protocol(igmp.igmp)
        pkt_ipv4 = pkt.get_protocol(ipv4.ipv4)
        dst = eth.dst
        src = eth.src
        mc_dst=0
        mc_src=0
        mulcast_addr='0.0.0.0'

        Num=0
        self.dp_action=defaultdict(list)
        self.mac_to_port.setdefault(dpid, {}) 
        self.group_mem.setdefault(mulcast_addr,{})
        self.group_mem_num[mulcast_addr]=0
        if src not in self.net.nodes():
            self.net.add_node(src)
            self.net.add_edge(dpid,src,{'port':in_port})
            self.net.add_edge(src,dpid,{'port':in_port})
	    #self.net.add_edge(dpid,src,'port'=in_port})
            #self.net.add_edge(src,dpid,'port'=in_port})
        if pkt_ipv4:
            self.ipv4_src = pkt_ipv4.src
            self.ipv4_dst = pkt_ipv4.dst
        if pkt_igmp:
            if (igmp.IGMP_TYPE_REPORT_V3 == pkt_igmp.msgtype):
                record_num=pkt_igmp.record_num
                for i in range (0,record_num):
                    mulcast_addr=pkt_igmp.records[i].address
                    
                    if mulcast_addr in self.group_mem:
                        src_num=self.group_mem_num[mulcast_addr]
                        
                        self.group_mem.setdefault(mulcast_addr,{})[src_num]=src
                        src_num=src_num+1
                        self.group_mem_num[mulcast_addr]=src_num    
                    else:
                        self.group_mem.setdefault(mulcast_addr,{})[0]=src
                        self.group_mem_num[mulcast_addr]=1 
            
        else:                  
            if self.ipv4_dst in self.group_mem: 
                dst_num=self.group_mem_num[self.ipv4_dst]
                for i in range(0,dst_num):
                    dst_of_group=self.group_mem[self.ipv4_dst][i]  
                    path=nx.shortest_path(self.net,src,dst_of_group)                
            else:
                out_port = ofproto.OFPP_FLOOD
                actions = [parser.OFPActionOutput(out_port)] 
                data = None
                if msg.buffer_id == ofproto.OFP_NO_BUFFER:
                    date = msg.data
                out = parser.OFPPacketOut(datapath=datapath, buffer_id=msg.buffer_id, in_port=in_port,actions=actions, data=data)
                datapath.send_msg(out)          

    @set_ev_cls(event.EventSwitchEnter)
    def get_topology_data(self, ev):
        switch_list = get_switch(self.topology_api_app, None)
        self.switches=[switch.dp.id for switch in switch_list]
        links_list = get_link(self.topology_api_app, None)
        self.links=[(link.src.dpid,link.dst.dpid,{'port':link.src.port_no}) for link in links_list]
         
        for switch in switch_list:    
            self.nodes[switch.dp.id] = switch.dp  
        self.net.add_nodes_from(self.switches)
        self.net.add_edges_from(self.links)
        linkss=[(link.dst.dpid,link.src.dpid,{'port':link.dst.port_no}) for link in links_list]
        self.net.add_edges_from(linkss)
        
