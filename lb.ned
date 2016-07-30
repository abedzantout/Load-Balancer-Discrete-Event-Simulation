package cmpsfinal;

simple LoadBalancer {
    
    parameters:
        @display("i=abstract/router");
        
	gates:
	    input fromNetNode;
	    input fromServer[];
        output toServer[]; 
        output toNode;
}