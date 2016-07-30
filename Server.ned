package cmpsfinal;

module Server
{
    parameters:
        int address;
        int weight;
        @display("i=device/server;bgb=406,250");

    gates:
        input inServer;
        output outServer;

    submodules:
        queue: Queue;
        processor: Processor;
        
    connections:
        inServer --> queue.in;
        processor.outToLb --> outServer;
        queue.outToProcessor --> processor.inFromQueue;

}