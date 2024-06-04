# Zephyr Example Docker Environment

This repository is based off of the zephyr example application repo https://github.com/zephyrproject-rtos/example-application.
This repository has the following added feature.

- Docker container to manage and isolate your workspace
- west autocomplete
- vscode cortex debugger integration
- Auto format on save using clang-format

# Configuring The Docker Environment
The configureation files for this docker environment are in `/.devcontainer`.
- ``devcontainer.json``: This is file is used by the vscode "Dev Containers" extension.
    This file selects the docker-compose file you will be using as well as settings to
    use in the container and extensions to be installed into the container after
    the docker setup portion is complete.
- ``docker-compose.yml``: This file points to the Dockerfile you are using and configures
    args for it. You can modify the USERNAME arg to configure the name of the default user
    in the dev container. It also contains a list of volumes to be bound to the container.
- ``Dockerfile``: This contains all of the setup scripts to install all the dependencies
    for you dec container. This contains things like the zephyr-sdk version you are using.
- ``west.yml``: This file is used to setup your zephyr environment and contains things like
    the zephyr version you are using and the modules you want installed. Note: if you make
    changes to this file in order for them to take affect you will likely have to do a clean
    rebuild of you dev container. To do this hit ctrl+shift+p then type/select "Dev Containers:
    Rebuild Container Without Cache". This will take significantly longer than rebuilding
    with cache so try to do this as infrequently as possible.

# Building The Docker Container
To get started with your docker container simply clone this repository and open it
in vscode. The make sure you have the "Dev Containers" vscode extension installed.
Make sure you have docker and docker-compose installed (instructions for installing
docker can be found here https://docs.docker.com/engine/install/ubuntu/). Once you
have everything installed you will need to run the "Dev Container: Rebuild and
Reopen in Container" task (you can do this by clicking on the blue box in the lower
left corner of vscode and then selecting the appropriate task).

# Zephyr Example Application

This repository contains a Zephyr example application. The main purpose of this
repository is to serve as a reference on how to structure Zephyr-based
applications. Some of the features demonstrated in this example are:

- Basic [Zephyr application][app_dev] skeleton
- [Zephyr workspace applications][workspace_app]
- [Zephyr modules][modules]
- [West T2 topology][west_t2]
- [Custom boards][board_porting]
- Custom [devicetree bindings][bindings]
- Out-of-tree [drivers][drivers]
- Out-of-tree libraries
- Example CI configuration (using Github Actions)
- Custom [west extension][west_ext]

This repository is versioned together with the [Zephyr main tree][zephyr]. This
means that every time that Zephyr is tagged, this repository is tagged as well
with the same version number, and the [manifest](west.yml) entry for `zephyr`
will point to the corresponding Zephyr tag. For example, the `example-application`
v2.6.0 will point to Zephyr v2.6.0. Note that the `main` branch always
points to the development branch of Zephyr, also `main`.

[app_dev]: https://docs.zephyrproject.org/latest/develop/application/index.html
[workspace_app]: https://docs.zephyrproject.org/latest/develop/application/index.html#zephyr-workspace-app
[modules]: https://docs.zephyrproject.org/latest/develop/modules.html
[west_t2]: https://docs.zephyrproject.org/latest/develop/west/workspaces.html#west-t2
[board_porting]: https://docs.zephyrproject.org/latest/guides/porting/board_porting.html
[bindings]: https://docs.zephyrproject.org/latest/guides/dts/bindings.html
[drivers]: https://docs.zephyrproject.org/latest/reference/drivers/index.html
[zephyr]: https://github.com/zephyrproject-rtos/zephyr
[west_ext]: https://docs.zephyrproject.org/latest/develop/west/extensions.html

## Getting Started

Before getting started, make sure you have a proper Zephyr development
environment. Follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).

### Initialization

1. Install docker and docker-compose. Follow the installation guide at https://docs.docker.com/compose/install/.

2. Install the "Dev Containers" extension in vscode.

3. Open this repository in vscode. Select the blue icon in the lower left corner and select "Reopen in Container" from the menu.



### Building and running

To build the application, run the following command:

```shell
west build -b $BOARD app
```

where `$BOARD` is the target board.

You can use the `custom_plank` board found in this
repository. Note that Zephyr sample boards may be used if an
appropriate overlay is provided (see `app/boards`).

A sample debug configuration is also provided. To apply it, run the following
command:

```shell
west build -b $BOARD app -- -DOVERLAY_CONFIG=debug.conf
```

Once you have built the application, run the following command to flash it:

```shell
west flash
```
