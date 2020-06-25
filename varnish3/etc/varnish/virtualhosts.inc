###########################################################
# HOSTS - BOLSA
###########################################################


# VirtualHost site xpto
if (req.http.host ~ "^(www\.)?mysite.com(\.br)?$") {
        set req.backend = backend_name;
	
	call entrega;
}

###########################################################
# POST / PURGE
###########################################################

# Se for um POST deixa passar sem fazer cache
elsif (req.request == "POST") {
        return (pass);
}

# Se for um PURGE verifica IP e faz purge
elsif (req.request == "PURGE") {
        if (!client.ip ~ purge) {
                error 405 "Cache: Purge Not allowed.";
        }
        ban_url(req.url);
        # ver
        error 200 req.url;
}

else {
        error 403 "Cache: Request not allowed.";
}
