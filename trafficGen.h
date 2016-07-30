/*
 * trafficGen.h
 *
 *
 * Author: Ahmad R. Dhaini
 * CMPS 284 - Computer Networks
 * Fall 2015-16
 * American University of Beirut
 *
 */

#ifndef _TRAFFICGEN_H_
#define _TRAFFICGEN_H_

#include "connReq_m.h"
#include "SELF_m.h"
#include <omnetpp.h>
#include <iostream>
#include <string>

using namespace std;

/**
 * trafficGen - Generated class
 */
class trafficGen : public cSimpleModule
{
	private:
		bool STOP;

	protected:

		// Parameters
		int DATAPktSize;
		long double data_arrival_rate;
		long double interArrival_time;	//inter arrival time length of traffic bursts

		// Functions
		virtual void initialize(int stage);
		virtual void handleMessage(cMessage *msg);
		virtual void finish();

		virtual void calcInterArrival(long double data_rate, int minSize, int maxSize);
		virtual void createAndSendPacket(int length, simtime_t timeStamp);

		virtual int getRandomPacketLength();
		virtual int getMeanPacketLength(int minSize, int maxSize);

	public:
		trafficGen();
		virtual ~trafficGen();
		virtual void startGenerator();
		virtual void stopGenerator();

};

#endif /* _TRAFFICGEN_H_ */
