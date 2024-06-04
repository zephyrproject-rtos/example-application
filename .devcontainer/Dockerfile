# Zephyr base docker image
FROM ubuntu:22.04

# the zephyr repo version is set in .devcontainer/west.yml
ARG ZEPHYR_SDK_VERSION=0.16.5-1

# These args must be passed in from the docker-compose.yml file.
ARG USERNAME
ARG PROJECT_NAME

ARG UID=1000
ARG GID=1000
ARG USER_UID=$UID
ARG USER_GID=$GID
ARG ZEPHYR_PROG_DIR=/home/${USERNAME}/
ARG ZEPHYR_REPO_DIR=/home/${USERNAME}/zephyr
ARG PROJECT_DIR=${ZEPHYR_PROG_DIR}/${PROJECT_NAME}

WORKDIR ${PROJECT_DIR}

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

# Create user account
RUN groupadd --gid $USER_GID $USERNAME \
	&& useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME && usermod -a -G dialout,plugdev $USERNAME \
	&& echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
	&& chmod 0440 /etc/sudoers.d/$USERNAME \
	&& cp /etc/skel/.bashrc /home/$USERNAME/ \
	&& chown -R $USERNAME:$USERNAME /home/$USERNAME/
USER $USERNAME

# Download Zephyr SDK
RUN cd ~ && wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VERSION}/zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-x86_64.tar.xz \
	&& wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VERSION}/sha256.sum | shasum --check --ignore-missing \
	&& tar xvf zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-x86_64.tar.xz \
	&& cd zephyr-sdk-${ZEPHYR_SDK_VERSION} \
	&& ./setup.sh -t all \
	&& ./setup.sh -c \
	&& ./setup.sh -h

# Instal west
RUN pip3 install --user -U west \
	&& echo 'export PATH=~/.local/bin:"$PATH"' >> ~/.bashrc

# Initalize Zephyr workspace
ARG ZEPHYR_MANIFEST_FILE=${ZEPHYR_PROG_DIR}/west.yml
COPY west.yml ${ZEPHYR_MANIFEST_FILE}
RUN cat ${ZEPHYR_MANIFEST_FILE}
RUN	cd  ${PROJECT_DIR} \
	&& west init --mf ${ZEPHYR_PROG_DIR}/west.yml -l \
	&& cd ${ZEPHYR_PROG_DIR} && west update \
	&& west zephyr-export \
	&& pip3 install --user -r ${ZEPHYR_PROG_DIR}/zephyr/scripts/requirements.txt \
    && west completion bash | sudo tee /etc/bash_completion.d/west-completion.bash > /dev/null

# Setup command history volume
RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" \
	&& sudo mkdir /commandhistory \
	&& sudo touch /commandhistory/.bash_history \
	&& sudo chown -R $USERNAME /commandhistory \
	&& sudo echo $SNIPPET >> "/home/$USERNAME/.bashrc"

# Show git branch in terminal
RUN echo "export GIT_PS1_SHOWDIRTYSTATE=1" >> ~/.bashrc \
	&& echo "source /usr/lib/git-core/git-sh-prompt" >> ~/.bashrc \
	&& echo "export PS1='\\[\\033[01;34m\\]\\u \\[\\033[01;32m\\]\\W\\[\\033[00;36m\\]\$(__git_ps1)\\[\\033[01;33m\\] \\$\\[\\033[01;0m\\] '" >> ~/.bashrc

# Final container tweaks. Add all additional apt packages to this list to minimize dev container rebuild time.
RUN sudo apt-get -y update && \
	sudo apt-get -y upgrade && \
	sudo apt-get install --no-install-recommends -y \
		bash-completion \
		clang-format \
		gcc-arm-none-eabi \
		openocd \
		vim \
	&& sudo ln -s /usr/bin/clang-format /usr/bin/clang-format-9

ENTRYPOINT ["sh", "-c", "bash"]