backend mybackend {
        .host = "back.end.ip.addr";
	.port = "80";
#	.probe = {
#		.request = "GET /index.php HTTP/1.1"
#			   "Host: www.site.com"
#			   "Connection: Close";
#		.interval = 5s;
#		.timeout = 1s;
#		.window = 5;
#		.threshold = 3;
#	}
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
