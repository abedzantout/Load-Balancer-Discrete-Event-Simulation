package cmpsfinal;

module NetworkNode
{
    parameters:
        @display("i=device/device");

    gates:
        output toLoadBalancer @loose;
        input torModule;

    submodules:
        TrafficGenerator: trafficGen;
        RecvModule: ReceiveModule;
        
    connections allowunconnected:
        TrafficGenerator.to_out --> RecvModule.fromTrafficGenerator;
		RecvModule.out --> toLoadBalancer;
		torModule --> RecvModule.fromLoadBalancer;
		RecvModule.outToTgen --> TrafficGenerator.fromRcvModule;

}