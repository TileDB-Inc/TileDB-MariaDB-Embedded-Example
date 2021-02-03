FROM ubuntu:20.04
LABEL maintainer="support@tiledb.io"

ENV AWS_EC2_METADATA_DISABLED=true \
 DEBIAN_FRONTEND=noninteractive \
 TZ=GMT

RUN apt-get update \
 && apt-get install -y \
 gosu \
 pwgen \
 tzdata \
 gcc \
 g++ \
 build-essential \
 libasan5 \
 bison \
 chrpath \
 cmake \
 gdb \
 gnutls-dev \
 libaio-dev \
 libboost-dev \
 libdbd-mysql \
 libjudy-dev \
 libncurses5-dev \
 libpam0g-dev \
 libpcre3-dev \
 libreadline-gplv2-dev \
 libstemmer-dev \
 libssl-dev \
 libnuma-dev \
 libxml2-dev \
 lsb-release \
 perl \
 psmisc \
 zlib1g-dev \
 libcrack2-dev \
 cracklib-runtime \
 libjemalloc-dev \
 libsnappy-dev \
 liblzma-dev \
 libzmq3-dev \
 uuid-dev \
 ccache \
 git \
 wget \
 libcurl4-openssl-dev \
 && rm -rf /var/lib/apt/lists/*

ENV MTR_MEM=/tmp \
 MARIADB_VERSION="mariadb-10.4.17"

WORKDIR /tmp

ARG MYTILE_VERSION="0.7.0"

ARG TILEDB_VERSION="2.2.3"

RUN git clone https://github.com/TileDB-Inc/TileDB-MariaDB.git -b ${MYTILE_VERSION} MyTile

RUN git clone https://github.com/TileDB-Inc/TileDB.git -b ${TILEDB_VERSION} TileDB \
 && cd TileDB \
 && mkdir -p build && cd build \
 && cmake -DTILEDB_VERBOSE=OFF -DTILEDB_S3=ON -DTILEDB_AZURE=ON -DTILEDB_GCS=ON -DTILEDB_SERIALIZATION=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr .. \
 && make -j$(nproc) \
 && make -C tiledb install \
 && cd /tmp

# Install curl after building tiledb
RUN apt-get update && apt-get install -y \
 libcurl4 \
 libcurl4-openssl-dev \
 && rm -rf /var/lib/apt/lists/*

RUN wget https://downloads.mariadb.org/interstitial/${MARIADB_VERSION}/source/${MARIADB_VERSION}.tar.gz \
 && tar xf ${MARIADB_VERSION}.tar.gz \
 && mv /tmp/MyTile ${MARIADB_VERSION}/storage/mytile \
 && cd ${MARIADB_VERSION} \
 && mkdir builddir \
 && cd builddir \
 && cmake -DCMAKE_INSTALL_PREFIX=/opt/server -DWITH_EMBEDDED_SERVER=ON -DPLUGIN_INNODB=NO -DPLUGIN_INNOBASE=NO -DPLUGIN_TOKUDB=NO -DPLUGIN_ROCKSDB=NO -DPLUGIN_MROONGA=NO -DPLUGIN_SPIDER=NO -DPLUGIN_SPHINX=NO -DPLUGIN_FEDERATED=NO -DPLUGIN_FEDERATEDX=NO -DPLUGIN_CONNECT=NO -DPLUGIN_MYTILE=YES -DCMAKE_BUILD_TYPE=Release .. \
 && make -j$(nproc) \
 && make install \
 && cd /tmp
