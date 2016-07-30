/*
 * trafficGen.cc
 *
 *
 * Author: Ahmad R. Dhaini
 * CMPS 284 - Computer Networks
 * Fall 2015-16
 * American University of Beirut
 *
 */

#include "trafficGen.h"

Define_Module(trafficGen);

trafficGen::trafficGen()
{

}



trafficGen::~trafficGen()
{

}


/* Initialize Module */
void trafficGen::initialize(int stage)
{
    // set configuration paramters

    data_arrival_rate = par("traffic_data_rate");
    STOP = true;
    DATAPktSize = par("packet_size");

    interArrival_time = 0;

    /* Start trafficGen*/
    if(!STOP)
        startGenerator();
}



/*
 * Handle Message and run proper trafficGen according to the traffic type
 */
void trafficGen::handleMessage(cMessage* msg)
{

    if(msg->getKind() == 100){startGenerator();}

	if (STOP)
		return;

   // Create and send packet based on type
     createAndSendPacket(getRandomPacketLength(),simTime());

    /*Schedule Next Packet based on exponential data arrival */
     cMessage *m = new cMessage("selfmsg");
    scheduleAt(simTime() + exponential(interArrival_time), m);
}

/* Finish Events */
void trafficGen::finish()
{
	
}

/*
 * Start trafficGen
 */
void trafficGen::startGenerator()
{

	/* This function call is required if calling a function of one module from another module */
	Enter_Method("trafficGen::startGenerator()");
	STOP = false;

	// Calculate inter-arrival time
	calcInterArrival(data_arrival_rate, DATAPktSize, DATAPktSize);


	if (interArrival_time == 0)
	{
		return;
	}
	// Schedule an event for sending the first packet.
	// Self Generated Msg
	ev << "\nTHE INTER_ARRIVAL TIME IS: " << interArrival_time << "\n";
	SELF* self_msg = new SELF();
	self_msg->setKind(1); // 0: for Conn Request, 1: Self message

    scheduleAt(simTime() + exponential(interArrival_time), self_msg);

}


/* Stop trafficGen */
void trafficGen::stopGenerator()
{
	/* This function call is required if calling a function of one module from another module */
	Enter_Method("trafficGen::stopGenerator()");

	STOP = true;
}

/* Calculate Inter-Arrival based on the transmission rate */
void trafficGen::calcInterArrival(long double data_rate, int minSize, int maxSize)
{

    ev << "\n\nVARIABLES: datarate: " << data_rate << ", minSize: " << minSize << ", maxSize: " << maxSize << "\n\n";

	/* Calculate interArrival_time */
	long double packetsPerSec = (long double)(data_rate / (double)(getMeanPacketLength(minSize,maxSize)*8.0));

	ev << "PACKETS PER SEC: " << packetsPerSec << "\n";

	if (data_rate == 0)
	{
		interArrival_time = 0;
		ev << "interarrivaltime set to 0 (nothing)\n";
	}
	else{
		interArrival_time = (long double)(1.0/packetsPerSec);
		ev << "interarrivaltime set to " << interArrival_time << "\n";
	}
}




/* Get Random Packet Length */
int trafficGen::getRandomPacketLength()
{
	return ((int)intuniform(DATAPktSize,DATAPktSize));
}

/* Get Mean Packet Length */
int trafficGen::getMeanPacketLength(int minSize, int maxSize)
{
	return (minSize + maxSize)/2;
}

/* Create packet to be sent */
void trafficGen::createAndSendPacket(int length, simtime_t timeStamp)
{
	/* Create New Packet */
	connReq* packet = new connReq("Connection Request");

	packet->setPacketSize(length);	//dataLength field in packet (Bytes)
    packet->setTimestamp(timeStamp);
 	packet->setKind(0); //0 for Conn Request

   /* Send request to load balancer gate */
	// TODO: change the 0 to the corresponding delay
    sendDelayed(packet,0,"to_out");
}
