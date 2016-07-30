#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ctime>
#include <math.h>
#include <vector>
#include "trafficGen.h"
#include "statusUpdate_m.h"
#include <omnetpp.h>



class ReceiveModule : public cSimpleModule{

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

};




class LoadBalancer : public cSimpleModule{

protected:
    //virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

};



class Processor : public cSimpleModule{

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

};



class Queue : public cSimpleModule{

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

};



// Module Definitions
Define_Module(LoadBalancer);
Define_Module(ReceiveModule);
Define_Module(Queue);
Define_Module(Processor);




// Queue vector for server queues
typedef std::vector<cQueue> QueueVector;




// Global variables

// used for Round Robin algorithm
int shu = 0;

// Initialized to the number of servers chosen in the ini file
int numberOfServers = 0;

time_t simulationStart = 0;

int *availableSpace; // Load Balancer will use this array for the least connection algorithm
int *serverWeights;  // this array holds the weight of each server, serverWeights[0] is the weight of server 0, etc ...
int *serverWeightCounter; // counter for the above array
int *serverInfo; // hold information about each server for the out.dat file
int cap; // The capacity of the current server
int currentServer = 0;
int in = 0;



// Initialization boolean variables are used to
// initialize arrays and other objects that need to be
// initialized only once. Since the initialize() method is called multiple times
// for each module, we used this mechanism to force some variables to be initialized once.
bool initialization_bool1 = true;
bool initialization_bool2 = true;
bool initialization_bool3 = true;
bool initialization_bool4 = true;
bool initialization_bool5 = true;




QueueVector qvector;           // Vector of queues (Server queues)
int algorithm = 0;                // Specifies the algorithm, check omnetpp.ini file for details on this variable
int numberOfPacketsBeforeEnding; // Specifies the number of packets to be sent before ending the simulation (X)
int packetCounter = 0;          // packet counter, increments every time a packet is sent
int processingPower = 0;        // Processing power of server
int packetSize = 0;


bool processing = false;
bool configured = false;

int counter = 0;
int counter2 = 0;

bool *isProcessing; // A boolean array that specifies if isProcessing[i] is currently processing a request or not
                    // where i is the currentServer


/*
 * The server processor initially initializes some variables and set some Global parameters
 * It also sends a status update message telling the load balancer the capacity of it's queue
 */

void Processor::initialize(){

    numberOfServers = this->getParentModule()->getParentModule()->par("numberOfServers");
    processingPower = this->par("processingPower");
    cap = this->getParentModule()->getSubmodule("queue")->par("capacity");

    if(initialization_bool1){availableSpace = (int*)malloc(numberOfServers*sizeof(int)); initialization_bool1=false;}
    if(initialization_bool2){serverWeights = (int*)malloc(numberOfServers*sizeof(int)); initialization_bool2=false;}
    if(initialization_bool3){serverInfo = (int*)malloc(numberOfServers*3*sizeof(int)); initialization_bool3=false;}
    if(initialization_bool5){

        isProcessing = (bool*)malloc(numberOfServers*sizeof(bool)); initialization_bool5=false;
        int j;
        for(j=0;j<numberOfServers;j++){
            isProcessing[j] = false;
        }
    }


    if(initialization_bool4){
        int i;
        serverWeightCounter = (int*)malloc(numberOfServers*sizeof(int));
        for(i=0;i<numberOfServers;i++){
            serverWeightCounter[i] = 0;
        }

        initialization_bool4 = false;
    }

    serverInfo[counter2] = processingPower;
    counter2++;
    serverInfo[counter2] = this->getParentModule()->par("weight");
    counter2++;
    serverInfo[counter2] = this->getParentModule()->getSubmodule("queue")->par("capacity");
    counter2++;

    serverWeights[in] = this->getParentModule()->par("weight");
    in++;

        statusUpdate *status = new statusUpdate("statusMessage");
        status->setServerId(this->getParentModule()->par("address"));
        status->setAvailableSpace(cap);
        send(status, "outToLb");
}


/*
 * The processor's handleMessage method receives a message from the Queue of the server
 * If the queue sent a status Message , it forwards it to the load balancer
 * else, it pops the request off the queue and processes it (simulated by usleep)
 */

void Processor::handleMessage(cMessage *msg){

    int serverAddress = this->getParentModule()->par("address");
    processingPower = this->par("processingPower");

    if(msg->isSelfMessage()){
        isProcessing[serverAddress-1] = false;
        delete msg;

    }else{

            if(strcmp("statusMessage", msg->getName()) == 0){

                send(msg, "outToLb");

            }else{

                // process request

                // first int is the processing power in bits/sec
                // second int is packet size in bits
                // the double "timeToSleepInSeconds" is the time to sleep in seconds.
                int ppInBitsPerSecond = processingPower * (int)pow(10.0,6.0);
                int packetSizeInBits = packetSize * 8;

                double timeToSleepInSeconds = packetSizeInBits/(double)ppInBitsPerSecond;

                // schedule self message to process request.
                cMessage *m = new cMessage("serverSelfMsg");
                scheduleAt(simTime() + timeToSleepInSeconds, m);
                isProcessing[serverAddress-1] = true;



            }

    }


}



/*
 * The Receive module initially initializes some global parameters
 */

void ReceiveModule::initialize(){
    numberOfPacketsBeforeEnding = this->getParentModule()->getParentModule()->par("numberOfPacketsBeforeEnding");
    packetSize = this->getParentModule()->getSubmodule("TrafficGenerator")->par("packet_size");
    simulationStart = time(0);

}


/*
 * The receive module can receive a message from the load balancer
 * telling it that it is done configuring the network
 * or it can receive a message from the traffic generator
 * If it receives a message from the load balancer, it forwards that message to the traffic generator
 * telling it to start generating packets
 * otherwise if it receives a message from the traffic generator, it forwards that message
 * along to the load balancer.
 */

void ReceiveModule::handleMessage(cMessage *msg){

    int simTimeBeforeEnd = this->getParentModule()->getParentModule()->par("simulationTime");



    if((time(0) - simulationStart) > simTimeBeforeEnd){
        endSimulation();
    }

    if(strcmp("doneconfig", msg->getName()) == 0){


        cMessage *m = new cMessage("doneconfig");
        m->setKind(100);
        send(m, "outToTgen");


    }else{
        send(msg ,"out");
        packetCounter++;
        if(packetCounter == numberOfPacketsBeforeEnding){endSimulation();};
        // end the simulation after sending a certain number of packets
    }

}




/*
 * It is in the handleMessage method of the Load Balancer where all the magic happens
 *  Here lies the forwarding algorithm, which determines which path the connection request will take upon arrival
 *  and to which server it wll be forwarded.
 */

void LoadBalancer::handleMessage(cMessage *msg){


    algorithm = this->getParentModule()->par("algorithm");
    int gatesize = gateSize("toServer");

    // tell node that network configuration is done
    // update occupancy information of servers
    if(strcmp("statusMessage", msg->getName()) == 0){

          statusUpdate *stm = dynamic_cast<statusUpdate*>(msg);

          if(!configured){

              availableSpace[stm->getServerId()-1] = stm->getAvailableSpace();

              counter++;

              if(counter == numberOfServers){
                  cMessage *m = new cMessage("doneconfig");
                  send(m, "toNode");
                  this->bubble("CONFIGURATION DONE");
                  configured = true;
              }

          }else{

                 availableSpace[stm->getServerId()-1] = stm->getAvailableSpace();

          }




    }



     if(strcmp("Connection Request", msg->getName()) == 0){

            if(algorithm == 0){
                // Random
                int random = rand() % gatesize;
                send(msg,"toServer", random);

            }else if(algorithm == 1){

                   // Round Robin
                    send(msg, "toServer", shu);
                    shu++;

                    if(shu == gatesize){
                        shu = 0;
                    }

            }else if(algorithm == 2){

                // Weighted Round Robin
                send(msg, "toServer", currentServer);
                serverWeightCounter[currentServer] += 1;

                if(serverWeightCounter[currentServer] == serverWeights[currentServer]){

                        currentServer++;
                        serverWeightCounter[currentServer-1] = 0;
                        if(currentServer == numberOfServers){currentServer=0;}
                }

           }else if(algorithm == 3){

               // Least Connections

               int max = availableSpace[0];
               int leastConnections = 0;
               int i;
               for(i=0;i<numberOfServers;i++){
                   if(availableSpace[i] > max){max=availableSpace[i];leastConnections=i;}
               }

               if(max == availableSpace[0]){
                   // Random
                     int random = rand() % gatesize;
                     send(msg,"toServer", random);
               }else{
                   // send to server with least connections (server with most available space)
                   send(msg,"toServer", leastConnections);
               }

           }


      }



}



// Choose any module to implement finish()
// finish will free all allocated memory and output server information to a file.







/*
 * When the Queue Receives a message
 * it adds this message to the queue
 * it then checks if the processor is processing a request
 * if it is, the message remains in the queue
 * if it is not, the message is popped off the queue
 * and send it to the processor for processing
 */



void Queue::initialize(){

    numberOfServers = this->getParentModule()->getParentModule()->par("numberOfServers");
    cap = par("capacity");

    cQueue queue;
    qvector.push_back(queue);

}


void Queue::handleMessage(cMessage *msg){

    int serverAddress = this->getParentModule()->par("address");

    cap = par("capacity");

    // if queue is full, drop received packet.
    if(qvector.at(serverAddress-1).length() >= cap){
        delete msg;
    }else{

            qvector.at(serverAddress-1).insert(msg);

            if(!isProcessing[serverAddress-1]){
                cMessage *m = (cMessage*) qvector.at(serverAddress-1).pop();
                send(m, "outToProcessor");
            }else{

                if(algorithm == 3){

                    statusUpdate *stm = new statusUpdate("statusMessage");
                    stm->setServerId(serverAddress-1);

                    stm->setAvailableSpace(cap - (qvector.at(serverAddress-1).length()));
                    send(stm, "outToProcessor");

                }

            }

    }

}






void LoadBalancer::finish(){

    // free all allocated memory
    free(availableSpace);
    free(serverWeights);
    free(serverWeightCounter);
    free(isProcessing);



    // Write Server information to output file
    int serverInfoCounter = 0;
    ofstream out;
    out.open("out.dat", ios::out | ios::trunc);
    int i;
    for(i=0;i<=numberOfServers;i++){

        if(i==0){
            out << "Server\t\tProcessing Rate\t\tWeight\t\tQueue Size\n";
        }else{

            out << "  S" << i << "\t\t";
            int x;
            for(x=0;x<3;x++){
                if(x!=2){out << serverInfo[serverInfoCounter] << "\t\t\t\t\t\t";}
                else{
                    out << serverInfo[serverInfoCounter];
                }
                serverInfoCounter++;
            }

            out << "\n";

        }


    }

    out.close();

}
