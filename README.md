# birbd
Source code sharing daemon for [birb](https://github.com/birb-linux/birb)

Birbd runs in the background waiting for other computers to reach out and ask for source code for some specific package. If a matching tarball is found, birbd will send it over from the /var/cache/distfiles directory. The matching is done with a filename and md5 checksum.

## Dependencies
- [birb](https://github.com/birb-linux/birb)
- boost
- openssl
