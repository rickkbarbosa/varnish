sub normalize_accept_encoding {
        if (req.http.Accept-Encoding) {
                if (req.http.Accept-Encoding ~ "gzip") {
                        set req.http.Accept-Encoding = "gzip";
                }
                elsif (req.http.Accept-Encoding ~ "deflate") {
                        set req.http.Accept-Encoding = "deflate";
                }
                else {
                        # unkown algorithm
                        remove req.http.Accept-Encoding;
                }
        }
}
sub fix_forwarded_for {
        remove req.http.X-Forwarded-For;
        set req.http.X-Forwarded-For = client.ip;
}
sub entrega {
        include "/etc/varnish/remove-cookies.inc";
        if (req.request ~ "(GET|HEAD)") {
          return (lookup);
        }
        if (req.request ~ "(POST)") {
          return (pass);
        }
}
