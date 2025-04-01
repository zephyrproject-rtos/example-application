# Copyright (c) 2025 James Walmsley <james@fullfat-fs.co.uk>
# SPDX-License-Identifier: Apache-2.0

ExternalZephyrProject_Add(
    APPLICATION mfg_image
    SOURCE_DIR ${APP_DIR}/mfg_image
)

ExternalZephyrProject_Add(
    APPLICATION dfu_app
    SOURCE_DIR ${APP_DIR}/dfu_app
)

add_dependencies(${DEFAULT_IMAGE} mfg_image)
add_dependencies(${DEFAULT_IMAGE} dfu_app)

