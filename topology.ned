package cmpsfinal;

network NetworkMain
{
    parameters:
        int numberOfServers = default(3);
        // The algorithm int paremeter here defines the algorithm.
        // 0 --> Random
        // 1 --> Round Robin
        // 2 --> Weighted Round Robin
        // 3 --> Least Connections
        int algorithm = default(0);
        int simulationTime = default(20);
        int numberOfPacketsBeforeEnding = default(15);
        //default distances as requested
        double distanceNetNodeToLd @unit(km) = default(10km);
        double distanceLdToServers @unit(km) = default(5km);

    types:
        //these are hardcoded and will be changed later according to specification
        channel netNodeToLd extends ned.DatarateChannel
        {
            datarate = 1Gbps;
            double dataRate @unit(b) = 1000000000b;
            double packetSize @unit(b) = 8000b; // 8000 bits/s
            double speedOfLight @unit(km) = 300000km;
            // delay = transimisson time + propogation time
            //The multiplication by 1s is to convert the unit from none to s (as in seconds)
            delay = (packetSize/dataRate*1s) + (distanceNetNodeToLd/speedOfLight*1s);
        }
        channel ldToServers extends ned.DatarateChannel
        {
            datarate = 1Mbps;
            double dataRate @unit(b) = 8000000b;
            double packetSize @unit(b) = 8000b;
            double speedOfLight @unit(km) = 300000km;
            // delay = transimisson time + propogation time
            delay = (packetSize/dataRate*1s) + (distanceLdToServers/speedOfLight*1s);
        }

    submodules:
        netWorkNode: NetworkNode;
        loadBalancer: LoadBalancer;
        servers[numberOfServers]: Server;
    connections allowunconnected:
        netWorkNode.toLoadBalancer  --> netNodeToLd --> loadBalancer.fromNetNode;
        
        for i=0..sizeof(servers)-1 {
            loadBalancer.toServer++ --> ldToServers --> servers[i].inServer;
            servers[i].outServer -->  ldToServers  --> loadBalancer.fromServer++;
        }

		loadBalancer.toNode --> netNodeToLd --> netWorkNode.torModule;

}
