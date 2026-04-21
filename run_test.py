#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import subprocess
import os
import io
import pexpect
import signal
from pygdbmi.gdbcontroller import GdbController

def target_extended_reset(gdbmi: GdbController) -> None:
    """Reset target board using more than one monitor reset command.
    This is intended for use in automatic runs like CI or in test suite.
    The purpose of this sequence is to eliminate spurious errors.
    """
    gdbmi.write("monitor reset")  # ordinary reset
    gdbmi.write("monitor reset 0")  # core & peripherals via SYSRESETREQ & VECTRESET bit
    gdbmi.write("monitor reset 1")  # the core only, not peripherals
    gdbmi.write("monitor reset 8")  # core & peripherals via SYSRESETREQ bit only
    gdbmi.write("monitor reset")  # ordinary reset

def run_test_project(
    remote_gdb_server,
    project_bin,
    src_file_name,
    src_file_line,
    test_result_var_name="test_result",
):
    gdbmi = GdbController(command=["gdb-multiarch", "--interpreter=mi2"])
    try:
        gdbmi.write(f"target extended-remote {remote_gdb_server}")
        gdbmi.write(f"file {project_bin}")
        target_extended_reset(gdbmi)
        gdbmi.write("load")
        gdbmi.write(f"b {src_file_name}:{src_file_line}")
        gdbmi.write("continue", timeout_sec=7)

        # Wait for remote gdb
        stopped = False
        max_iterations = 1000
        iterations = 0
        while not stopped and iterations < max_iterations:
            responses = gdbmi.get_gdb_response(timeout_sec=5)
            for msg in responses:
                if msg["type"] == "notify" and msg["message"] == "stopped":
                    stopped = True
            iterations += 1

        if not stopped:
            raise TimeoutError("Debugger did not stop within expected time")

        test_result = gdbmi.write(f"-data-evaluate-expression {test_result_var_name}")
        value = None
        for msg in test_result:
            if msg["type"] == "result" and msg["message"] == "done":
                payload = msg.get("payload", {})
                if "value" in payload:
                    value = payload["value"]

        assert value == "true", f"Test execution errors: \n test_result = {value}"
    finally:
        gdbmi.exit()

def test_n7s_spw():
    remote_gdb_server = os.getenv("SAMRH71_REMOTE_GDBSERVER", default="127.0.0.1:2331")
    run_test_project(remote_gdb_server, 'build/test/test.elf', 'main.c', '198')

if __name__ == "__main__":
    test_n7s_spw()
