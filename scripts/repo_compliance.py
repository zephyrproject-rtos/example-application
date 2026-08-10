#!/bin/env python3
# Copyright (c) 2026 Analog Devices, Inc.
# SPDX-License-Identifier: Apache-2.0

# This script runs compliance checks on the repository for coding standards.
# It imports logic from Zephyr's own check_compliance.py script, but overrides
# checks that are specific to Zephyr's own repository structure

import os
import sys
from pathlib import Path

ZEPHYR_BASE = os.environ.get("ZEPHYR_BASE", str(Path(__file__).resolve().parents[2] / "zephyr"))
sys.path.insert(0, str(Path(ZEPHYR_BASE) / "scripts" / "ci"))

import check_compliance  # noqa: E402


class SampleCheck(check_compliance.ComplianceTest):
    """
    Example of a custom compliance check that can be added to the CI checks run in this repo.
    This check simply passes when run, unless a file in the root of the repo
    called "fail.txt" exists. If present, the check will fail and print a message to the console.
    """

    name = "SampleCheck"
    doc = "Sample check that fails if fail.txt exists in the root of the repo"

    def run(self):
        """
        Run the sample check.
        """
        fail_file = Path(check_compliance.GIT_TOP) / "fail.txt"
        if fail_file.exists():
            self.failure("SampleCheck failed because fail.txt exists in the root of the repo.")
            return False


# Run compliance checks using Zephyr's check_compliance.py script
check_compliance.main(sys.argv[1:])
