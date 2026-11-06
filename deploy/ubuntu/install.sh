#!/usr/bin/env bash
# Installs yip's web build on the Ubuntu box from an uploaded folder (deploy_ubuntu.ps1
# sends it to /tmp/yip-upload): the files under /opt/yip/web, Caddy's snippet in
# /etc/caddy/sites/kazin.dev.d/ (Caddy restarted only when the snippet changed: its
# admin API is off). Static files only: no service, no user.
#   sudo bash install.sh [<upload folder>]
set -euo pipefail
SRC="${1:-/tmp/yip-upload}"
SNIPPET=/etc/caddy/sites/kazin.dev.d/yip.caddy

install -d -m 755 /opt/yip

# The web build swapped whole (a visitor never gets half of two builds)
rm -rf /opt/yip/web.new
cp -r "$SRC/web" /opt/yip/web.new
chmod -R a+rX /opt/yip/web.new
rm -rf /opt/yip/web.old
[ -d /opt/yip/web ] && mv /opt/yip/web /opt/yip/web.old
mv /opt/yip/web.new /opt/yip/web
rm -rf /opt/yip/web.old

if ! cmp -s "$SRC/yip.caddy" "$SNIPPET"; then
	[ -f "$SNIPPET" ] && cp "$SNIPPET" "$SNIPPET.bak"
	install -m 644 "$SRC/yip.caddy" "$SNIPPET"
	if ! caddy validate --config /etc/caddy/Caddyfile --adapter caddyfile >/tmp/yip-caddy.log 2>&1; then
		echo "(!) Caddy refused the snippet:"; tail -5 /tmp/yip-caddy.log
		if [ -f "$SNIPPET.bak" ]; then mv -f "$SNIPPET.bak" "$SNIPPET"; else rm -f "$SNIPPET"; fi
		exit 1
	fi
	rm -f "$SNIPPET.bak"
	systemctl restart caddy
	echo "Caddy: snippet installed, restarted"
fi

ls -l /opt/yip/web
