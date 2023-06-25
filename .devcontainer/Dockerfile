# Base Image (ci-base)

FROM ubuntu:22.04

ARG UID=1000
ARG GID=1000
ARG USERNAME=mainUser
ARG USER_UID=$UID
ARG USER_GID=$GID

# Set default shell during Docker image build to bash
SHELL ["/bin/bash", "-c"]

# Set non-interactive frontend for apt-get to skip any user confirmations
ENV DEBIAN_FRONTEND=noninteractive

# Install base packages
RUN apt-get -y update && \
	apt-get -y upgrade && \
	apt-get install --no-install-recommends -y \
		software-properties-common \
		lsb-release \
		autoconf \
		automake \
		bison \
		build-essential \
		ca-certificates \
		cargo \
		ccache \
		chrpath \
		cmake \
		cpio \
		device-tree-compiler \
		dfu-util \
		diffstat \
		dos2unix \
		doxygen \
		file \
		flex \
		g++ \
		gawk \
		gcc \
		gcovr \
		git \
		git-core \
		gnupg \
		gperf \
		gtk-sharp2 \
		help2man \
		iproute2 \
		lcov \
		libcairo2-dev \
		libglib2.0-dev \
		libgtk2.0-0 \
		liblocale-gettext-perl \
		libncurses5-dev \
		libpcap-dev \
		libpopt0 \
		libsdl1.2-dev \
		libsdl2-dev \
		libssl-dev \
		libtool \
		libtool-bin \
		locales \
		make \
		net-tools \
		ninja-build \
		openssh-client \
		parallel \
		pkg-config \
		python3-dev \
		python3-pip \
		python3-ply \
		python3-setuptools \
		python-is-python3 \
		qemu \
		rsync \
		socat \
		srecord \
		sudo \
		texinfo \
		unzip \
		valgrind \
		wget \
		ovmf \
		xz-utils \
		thrift-compiler

# Install multi-lib gcc (x86 only)
RUN if [ "${HOSTTYPE}" = "x86_64" ]; then \
	apt-get install --no-install-recommends -y \
		gcc-multilib \
		g++-multilib \
	; fi

# Install i386 packages (x86 only)
RUN if [ "${HOSTTYPE}" = "x86_64" ]; then \
	dpkg --add-architecture i386 && \
	apt-get -y update && \
	apt-get -y upgrade && \
	apt-get install --no-install-recommends -y \
		libsdl2-dev:i386 \
	; fi

# Initialise system locale
RUN locale-gen en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Install Python dependencies
RUN python3 -m pip install -U --no-cache-dir pip && \
	pip3 install -U --no-cache-dir wheel setuptools && \
	pip3 install --no-cache-dir pygobject && \
	pip3 install --no-cache-dir \
		-r https://raw.githubusercontent.com/zephyrproject-rtos/zephyr/master/scripts/requirements.txt \
		-r https://raw.githubusercontent.com/zephyrproject-rtos/mcuboot/master/scripts/requirements.txt \
		GitPython imgtool junitparser numpy protobuf PyGithub \
		pylint sh statistics west && \
	pip3 check

# Clean up stale packages
RUN apt-get clean -y && \
	apt-get autoremove --purge -y && \
	rm -rf /var/lib/apt/lists/*

# Create User
RUN groupadd -g $GID -o $USERNAME \
	&& useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME && usermod -a -G dialout,plugdev $USERNAME \
	&& passwd -d $USERNAME \
	&& mkdir /home/$USERNAME/bin && chown $USERNAME:$USERNAME /home/$USERNAME/bin \
	&& echo '$USERN ALL = NOPASSWD: ALL' > /etc/sudoers.d/$USERNAME \
	&& chmod 0440 /etc/sudoers.d/$USERNAME \
	&& usermod -aG sudo $USERNAME

USER $USERNAME
ARG PROJECT_WORK_DIR=zephyr_docker_example_application

# Download SDK
RUN cd ~ && wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.0/zephyr-sdk-0.16.0_linux-x86_64.tar.xz \
	&& wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.0/sha256.sum | shasum --check --ignore-missing \
	&& tar xvf zephyr-sdk-0.16.0_linux-x86_64.tar.xz \
	&& cd zephyr-sdk-0.16.0 \
	&& ./setup.sh -t all \
	&& ./setup.sh -c \
	&& ./setup.sh -h

# Install zephy dependencies
RUN pip3 install --user -U west && echo 'export PATH=~/.local/bin:"$PATH"' >> ~/.bashrc && source ~/.bashrc \
	&& west init ~/zephyrproject && cd ~/zephyrproject && west update \
	&& west zephyr-export \
	&& pip3 install --user -r ~/zephyrproject/zephyr/scripts/requirements.txt


RUN	sudo apt-get -y update && \
	sudo apt-get -y upgrade && \
	sudo apt-get install --no-install-recommends -y \
		bash-completion \
		clang-format \
		openocd \
		vim \
	&& sudo ln -s /usr/bin/clang-format /usr/bin/clang-format-9

ENTRYPOINT ["sh", "-c", "scripts/docker_setup.sh && bash"]