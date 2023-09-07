# BRIGHT 🔆
## Initialization

The first step is to initialize the workspace folder (``my-workspace``) where
the ``bright`` and all Zephyr modules will be cloned. Run the following
command:

```shell
# initialize my-workspace for the bright (main branch)
west init -m hhttps://github.com/UmairAsghar-max/bright.git --mr main my-workspace
# update Zephyr modules
cd my-workspace
west update
```

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

### Testing

To execute Twister integration tests, run the following command:

```shell
west twister -T tests --integration
```
