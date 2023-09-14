# birbd
Source code sharing daemon for [birb](https://github.com/birb-linux/birb)

Birbd runs in the background waiting for other computers to reach out and ask for source code for some specific package. If a matching tarball is found, it can be downloaded with rsync etc. from the server. The matching is done with a filename and md5 checksum. The connection isn't encrypted, so don't send anything sensitive over it.

## Dependencies
- boost
- openssl

## Setup
Setting up a birbd service requires some things both on the client side and server side

### Server
- Compile and install birbd
       - If you want to use the included systemd service, install birbd with `make PREFIX=/usr install-systemd`
- Create a /var/cache/distfiles directory and download/copy any tarballs you want to share there
- Create a birbd user and give it access to /var/cache/distfiles
- Create ssh keys for the birbd user with `ssh-keygen`

### Client
- Compile and install birbd
- Create ssh keys for the root user with `ssh-keygen`
- Copy the public key of the root user to the birbd server with `ssh-copy-id`
- Add the IP-address of the birbd server to /etc/birbd.conf
