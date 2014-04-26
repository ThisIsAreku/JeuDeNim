#!/bin/bash
if [ "$(tail -c1 "$1"; echo x)" != $'\nx' ]; then
    echo "" >>"$1"
fi