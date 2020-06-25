if (req.url ~ "^/[^?]+\.(jpeg|jpg|png|gif|swf|ico|js|css|txt|gz|zip|lzma|bz2|tgz|tbz|html|htm)(\?.*|)$") {
        unset req.http.cookie;
        set req.url = regsub(req.url, "\?.*$", "");
}
