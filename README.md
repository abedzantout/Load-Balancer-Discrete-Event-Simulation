#Load-Balancer-Discrete-Event-Simulation
#Load Balancers & Discrete Event Simulations

##ABSTRACT
During development of network architecture, researchers in most cases use simulations for evaluation of their product before implementing it in a real world scenario. This is as true in the area of communication networks as it is in the area of aircraft. The assignment provided by Dr Dhaini is an invaluable lesson for undergraduates students (such as ourselves), as it allows us to explore network architecture and simulation from the very core of the field: the design and software implementation of a real-life simple network topology. This project also allowed us to have a firm understanding of discrete event simulation, which extends to nearly all fields that require simulations to test products, not only the network field.We present the network topology required using the OMNeT++ discrete event simulator, which enables efficient modeling of large-scale network architecture.

This project was conducted under the supervision of Professor [Ahmad Dhaini](http://staff.aub.edu.lb/~ad57/).

###Categories and Subject Descriptors
I.6.7 [Simulation and Modeling]: Simulation Support Systems—Environments
I.6.8 [Simulation and Modeling]: Types of Simulation—Discrete event

####General Terms
Simulations, Simulation Environment, Design

####Keywords
Discrete event simulation, OMNeT++




##INTRODUCTION

Communication systems are growing in size and complexity, and govern almost every aspect of our lives. We notice people around us always connected, and the emergence of the Internet of Things and the never ending growth of the Internet and communication systems will shape the lives of people even further in the future.Being able to simulate a network is a key skill that every software architect and engineer needs to acquire in order for him to join the journey of designing and building a more connected world. 

The purpose of this project was of two fold: first to make us familiar with discrete event simulation, a powerful technique used in multiple industries; second, learn how a load balancers work.

##OMNET++ AND THE NETWORK DESIGN“

OMNeT++ is an extensible, modular, component-based C++ simulation library and framework, primarily for building network simulators.” [omnetpp.org]

After we were assigned to implement the network topology, a fair amount of time was dedicated to the only purpose of understanding discrete event simulations. 

This project was tested on OS X and Windows 10, and was slightly modified for each to work.

##Programming Approach

In the simulation.cc file we have 4 classes, each with two functions: handleMessage(cMessage args) which executes when a module receives a message from another module; and initialize() which executes at the beginning of the simulation.

###omnetpp.ini

In the omnetpp.ini, the user can specify the following parameters: the number of servers, the network data rate, packet size, the queue capacity, the processing power of each server, simulation time, the number of packets to be sent over the network, and finally the algorithm to be applied in the simulation for the distribution of the packets. For the latter, the user needs to chose between:
Random (enter 0), 
Round Robin (enter 1), 
Weighted Round Robin(enter 2),
Least Connections(enter 3).

####Important Note: 

When changing the number of servers, it is important for the tester to add the address of those servers sequentially. The number of servers is changed by updating NetworkMain.numberOfServers, and the addresses of the newly added servers should be added and updated accordingly as follows:

NetworkMain.servers[i].address = i + 1

Where i is the index of the array of servers that starts from 0.To remove a server, just decrement theNetworkMain.numberOfServers by the desired number, and remove/comment out each server’s address.

Each server can have its own weight, but the sum of the weights must be equal to 3N where N is the total number of servers.Each server can also have its own capacity (queue size ) and its own processing power.

##Final Word

The simulation provided is based on the effort of two individuals that worked really hard to  implement the required topology, each one on a different operating system. The effort was mutual and the relation and collaboration between both of the group members contributed to the success of the provided implementation


##License

Copyright 2016 [Abed Zantout](https://github.com/abedzantout/), [Abbas Baydoun](https://github.com/abbasb717)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
