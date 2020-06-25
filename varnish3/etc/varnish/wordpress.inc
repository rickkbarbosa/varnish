# Configuracao para ser utilizada com sites Wordpress
# Faz lookup de todos estaticos incluindo os que estao em wp-admin
if (req.url ~ "^.*\.(jpeg|jpg|png|gif|swf|ico|js|css|txt|gz|zip|lzma|bz2|tgz|tbz|html|htm).*$") {
        unset req.http.cookie;
        return (hash);
}

# Faz um pass direto ao backend para os seguintes casos
if (req.url ~ "wp-(login|login\.php|admin|comments-post|admin-ajax\.php)" || req.url ~ "(preview=true|/nextgen-gallery/admin)" || req.url ~ "login|cadastro|cadastrar-filho|nova-lista|meus-filhos|minhas-listas" || req.http.cookie ~ "wordpress_logged" || req.url ~ "top-10-addcount.js.php" || req.url ~ "adrotate") {
        return (pipe);
}

# Faz cache e tira cookie de qualquer outra coisa que nao for wp-admin
else {
        unset req.http.cookie;
        return (hash);
}

