package cmpsfinal;
//
// Author: Ahmad R. Dhaini
// CMPS 284 - Computer Networks
// Fall 2014-15
// American University of Beirut
//
simple trafficGen
{
    parameters:
		int packet_size;
		int traffic_data_rate;
        @display("i=block/source");
        @class(trafficGen);
    gates:
        output to_out;
        input fromRcvModule;
}