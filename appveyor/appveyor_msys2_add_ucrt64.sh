if [[ $(grep -L "ucrt64" /etc/pacman.conf) ]]; then

  cat >> /etc/pacman.conf << EOF
[ucrt64]
Include = /etc/pacman.d/mirrorlist.ucrt64
EOF

  cat > /etc/pacman.d/mirrorlist.ucrt64 << EOF
## Primary
Server = https://repo.msys2.org/mingw/ucrt64/
## Mirrors
Server = https://downloads.sourceforge.net/project/msys2/REPOS/MINGW/ucrt64/
Server = https://www2.futureware.at/~nickoe/msys2-mirror/mingw/ucrt64/
Server = https://mirror.yandex.ru/mirrors/msys2/mingw/ucrt64/
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/ucrt64/
Server = http://mirrors.ustc.edu.cn/msys2/mingw/ucrt64/
Server = http://mirror.bit.edu.cn/msys2/mingw/ucrt64/
Server = https://mirror.selfnet.de/msys2/mingw/ucrt64/
Server = https://mirrors.sjtug.sjtu.edu.cn/msys2/mingw/ucrt64/
Server = https://mirror.jmu.edu/pub/msys2/mingw/ucrt64/
Server = https://ftp.cc.uoc.gr/mirrors/msys2/mingw/ucrt64/
Server = https://ftp.acc.umu.se/mirror/msys2.org/mingw/ucrt64/
Server = https://mirrors.piconets.webwerks.in/msys2-mirror/mingw/ucrt64/
Server = https://quantum-mirror.hu/mirrors/pub/msys2/mingw/ucrt64/
Server = https://mirrors.dotsrc.org/msys2/mingw/ucrt64/
Server = https://mirror.ufro.cl/msys2/mingw/ucrt64/
Server = https://mirror.clarkson.edu/msys2/mingw/ucrt64/
Server = https://ftp.nluug.nl/pub/os/windows/msys2/builds/mingw/ucrt64/
Server = https://download.nus.edu.sg/mirror/msys2/mingw/ucrt64/
Server = https://ftp.osuosl.org/pub/msys2/mingw/ucrt64/
Server = https://fastmirror.pp.ua/msys2/mingw/ucrt64/
EOF
fi
