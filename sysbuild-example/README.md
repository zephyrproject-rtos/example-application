# Example Sysbuild Project

The aim of this folder is to demonstrate a typical sysbuild project from the ground-up.

## Build

```
cd my-workspace/example-application
west build --sysbuild sysbuild-example \
    -DEXTRA_DTC_OVERLAY_FILE=(pwd)/sysbuild-example/overlays/nucleo_f413zh_partitions.overlay \
    -Dmcuboot_EXTRA_DTC_OVERLAY_FILE=(pwd)/sysbuild-example/overlays/nucleo_f413zh_partitions.overlay \
    -Dmfg_image_EXTRA_DTC_OVERLAY_FILE=(pwd)/sysbuild-example/overlays/nucleo_f413zh_partitions.overlay \
    -Ddfu_app_EXTRA_DTC_OVERLAY_FILE=(pwd)/sysbuild-example/overlays/nucleo_f413zh_partitions.overlay
```

