vcl 4.0;

# Backends
include "/etc/varnish/backends.inc";

# ACLs
acl purge {
        "localhost";
        "127.0.0.1";
        # "10.xxx.xxx.0"/24;
}

acl header {
        "localhost";
        # "192.168.0.xxx"/24;
}

# Configuracoes de Acesso
sub vcl_recv {
        # Enquanto o backend nao entrega um objeto novo (latencia na geracao) entregamos o antigo por grace tempo
        # Precisa ser setado em req.grace e obj.grace para ter efeito
        # http://varnish.projects.linpro.no/ticket/355
        # http://varnish.projects.linpro.no/changeset/2392
#	if (backend.healthy) {
#		set req.grace = 30s;
#	}
#	else {
#		set req.grace = 1h;
#		}
#	}
	#set req.grace = 360000s;
#	set req.grace = 1d;
	call normalize_accept_encoding;
        call fix_forwarded_for;

        # SLB Check
        if (req.url == "/healthcheck_varnish.html") {
          #error 200  "Ok";
          return(synth(200, "Ok"));
        }

        # Include dos VirtualHosts
        include "/etc/varnish/virtualhosts.inc";


}

sub vcl_hash {

        if (req.http.user-agent ~ "Alcatel|Benq|BlackBerry|blackberry|DoCoMo|Ericsson|Gradiente|HPiPaq|HTC|Htc|htc|hTC|KDDI|KWC|MOT|LG|Palm|QTEK|SAGEM|SAM|SAGEM|SCH|SAGEM|SEC|SGH|SonyEricsson|Vodafone|ZTE|kyok|nokia|NOKIA|Nokia|mot|Mot|Iris|iris|IRIS|iPhone|Android|SymbianOS") {
                set req.http.X-Device = "Mobile";
                hash_data(req.http.user-agent);
                hash_data(req.http.X-Device);
        }
#       else {
                # Remove os headers da requisicao antes de repassar ao backend
#               include "/etc/varnish/remove-headers-request.inc";
#       }

        # Vital para virtualhosts
        hash_data(req.url);
        hash_data(req.http.host);

        #return (hash);
        return (lookup);
}

sub vcl_hit {
	#if (req.request == "PURGE") {
	if (req.method == "PURGE") {
                #set obj.ttl = 0s;
		ban(req.url) ;
                #error 200 "Purged.";
                return(synth(200,"Purged."));
        }
}

sub vcl_miss {
#        if (req.request == "PURGE") {
        if (req.method == "PURGE") {
# ver
                #error 404 "Not in cache.";
                return(synth(404,"Not in cache."));
        }
}

# TTL dos caches
#sub vcl_fetch {
sub vcl_backend_response {
        # Enquanto o backend nao entrega um objeto novo (latencia na geracao) entregamos o antigo por grace tempo
        # Precisa ser setado em req.grace e obj.grace para ter efeito
        # http://varnish.projects.linpro.no/ticket/355
        # http://varnish.projects.linpro.no/changeset/2392
        set beresp.grace = 3d;

        if (!beresp.ttl>0s) {
                set beresp.ttl = 5s;
        }

        else {
                if (beresp.status == 404) {
                        set beresp.ttl = 5s;
                }
                else {
                        # Se for cacheable mas com ttl muito baixo vindo do backend
                        # Se preferir, da pra setar novo ttl somente para quem envia ttl baixo ou alto demais
                        if (beresp.ttl < 120s) {
                                set beresp.ttl = 120s;
                        }
                }
        }

        # Remove os headers dos backends antes de popular o cache
        include "/etc/varnish/remove-headers-backend.inc";

        return (deliver);
}

sub vcl_deliver {
        if (obj.hits > 0) {
                set resp.http.X-Cache = "HIT";
        } else {
                set resp.http.X-Cache = "MISS";
        }

        if (client.ip ~ header) {
                set resp.http.X-Served-By = server.hostname;
                set resp.http.X-Client-IP = client.ip;
                set resp.http.X-Cache-Hits = obj.hits;
        } else {
                unset resp.http.X-Server-ID;
        }

        # set resp.http.X-Cache-Hits = obj.hits;
        set resp.http.X-Age = resp.http.Age;
        unset resp.http.Age;

        # Remove os headers do varnish antes de entregar
        include "/etc/varnish/remove-headers-varnish.inc";
}

# Custom, at our own risk functions
include "/etc/varnish/functions.inc";
