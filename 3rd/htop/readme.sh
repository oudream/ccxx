

# source code : master 20191115
https://github.com/hishamhm/htop


# alpine
apk add ncurses-dev


# ubuntu libncurses6-dev, but now libncurses5-dev
apt install libncurses5-dev


#
cd /opt/ddd/ccpp/ccxx/3rd/htop/htop && \
./autogen.sh && ./configure --disable-unicode
