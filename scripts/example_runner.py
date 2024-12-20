# Copyright (c) 2024 Basalte bv
#
# SPDX-License-Identifier: Apache-2.0

from runners.core import RunnerCaps, ZephyrBinaryRunner


class ExampleRunner(ZephyrBinaryRunner):
    """Dummy example runner."""

    def __init__(self, cfg, param):
        super().__init__(cfg)
        self.param = param

    @classmethod
    def name(cls):
        return "example"

    @classmethod
    def capabilities(cls):
        return RunnerCaps(commands=({"simulate"}))

    @classmethod
    def do_add_parser(cls, parser):
        parser.add_argument("--param", action="store", help="An example parameter")

    @classmethod
    def do_create(cls, cfg, args):
        return cls(cfg, param=args.param)

    def do_run(self, command, **kwargs):
        self.logger.info(f"Running {command} on {self.param}")
