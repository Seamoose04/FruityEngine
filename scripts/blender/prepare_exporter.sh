#!/bin/bash
set -e
cd "$(dirname "$0")"
rm -f blender_exporter.zip
zip -r blender_exporter.zip blender_exporter/
echo "Done: blender_exporter.zip"
