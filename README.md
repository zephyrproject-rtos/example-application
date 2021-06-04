# Zephyr Example Application

This repository contains a Zephyr example application. The main purpose of this
repository is to serve as a reference on how to structure Zephyr based
applications. Some of the features demonstrated in this example are:

- Basic application skeleton
- [Custom boards][board_porting]
- Custom [devicetree bindings][bindings]
- Out-of-tree [drivers][drivers]
- Example CI configuration (using Github Actions)

This repository is versioned together with the [Zephyr main tree][zephyr]. This
means that every time that Zephyr is tagged, this repository is tagged as well
with the same version number, and the [manifest](west.yml) entry for `zephyr`
will point to the corresponding Zephyr tag. For example, `example-application`
v2.6.0 will point to Zephyr v2.6.0. Note that the `main` branch will always
point to the development branch of Zephyr, also `main`.

[board_porting]: https://docs.zephyrproject.org/latest/guides/porting/board_porting.html
[bindings]: https://docs.zephyrproject.org/latest/guides/dts/bindings.html
[drivers]: https://docs.zephyrproject.org/latest/reference/drivers/index.html
[zephyr]: https://github.com/zephyrproject-rtos/zephyr

## Getting Started

Before getting started, make sure you have a proper Zephyr development
environment. You can follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html).

### Initialization

The first step is to initialize the workspace folder (``my-workspace``) where
the ``example-application`` and all Zephyr modules will be cloned. You can do
that by running:

```shell
# initialize my-workspace for the example-application (main branch)
west init -m https://github.com/zephyrproject-rtos/example-application --mr main my-workspace
# update Zephyr modules
cd my-workspace
west update
```

### Build & Run

The application can be built by running:

```shell
west build -b $BOARD -s app
```

where `$BOARD` is the target board. The `custom_plank` board found in this
repository can be used. Note that Zephyr sample boards may be used if an
appropriate overlay is provided (see `app/boards`).

A sample debug configuration is also provided. You can apply it by running:

```shell
west build -b $BOARD -s app -- -DOVERLAY_CONFIG=debug.conf
```

Note that you may also use it together with `rtt.conf` if using Segger RTT. Once
you have built the application you can flash it by running:

```shell
west flash
```
