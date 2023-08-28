# birbd
Source code sharing daemon for [birb](https://github.com/birb-linux/birb)

Birbd runs in the background waiting for other computers to reach out and ask for source code for some specific package. If a matching tarball is found, it can be downloaded with rsync etc. from the server. The matching is done with a filename and md5 checksum. The connection isn't encrypted, so don't send anything sensitive over it.

## Dependencies
- [birb](https://github.com/birb-linux/birb)
- boost
- openssl
