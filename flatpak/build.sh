#!/bin/bash

flatpak-builder  --user --install build-dir remc2.flatpak.json --force-clean

# NOTE: run via
#       flatpak run com.github.thobbsinteractive.magic-carpet-2-hd 
