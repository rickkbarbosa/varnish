###########################################################
# HOSTS - EXAMPLE
###########################################################


# VirtualHost site
#Bypass - Ex: AdRotate Tracker
if (req.url ~ "clicktracker.php" || req.url ~ "banners") {
	set client.identity = req.http.x-forwarded-for;
	return (pipe);
}

if (req.http.host ~ "^(www\.)?site\.com(\.br)?$" ) {
        set req.backend_hint = mybackend;

	set client.identity = req.http.x-forwarded-for;
	
        #Configuração do Wordpress
        include "/etc/varnish/wordpress.inc";

	call entrega;
}

# VirtualHost another site [This will pass]
if (req.http.host ~ "^another\.site\.com(\.br)?$" ) {
        
        return (pipe);

        call entrega;
}


###########################################################
# POST / PURGE
###########################################################

# Se for um POST deixa passar sem fazer cache
elsif (req.method == "POST") {
        return (pass);
}

# Se for um PURGE verifica IP e faz purge
elsif (req.method == "PURGE") {
        if (!client.ip ~ purge) {
                #error 405 "Cache: Purge Not allowed.";
		return(synth(405, "Cache: Purge Not Allowed"));
        }
        ban(req.url);
        # ver
	return(synth(200, req.url));
}

else {
        #error 403 "Cache: Request not allowed.";
        return(synth(403, "Cache: request not allowed"));
}
