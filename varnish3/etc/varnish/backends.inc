backend backendname1 {
        .host = "back.end.ip.addr";
}

backend backendname2 {
        .host = "back.end.ip.addr";
}

#Director Example
#A Varnish director is a group of backends used as a single backend. 
#This is useful when more than a webserver attends a virtualhost.
#Varnish directors is also utilized as 'load-balance'.
#In both cases, 'backendname' declared on Varnish Virtualhost is replaced by 'directorname'

#director directorname round-robin {
#	{
#		.backend = backendname1;
#	}
#	{
#		.backend = backendname2;
#	}
#}
