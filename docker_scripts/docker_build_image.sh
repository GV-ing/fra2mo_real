#!/bin/bash
cd "$(dirname "$0")"

# Start the Docker image build
rm -rf ./tmp_sources
mkdir -p ./tmp_sources
# Definisci il nome dell'immagine
IMAGE_NAME="fra2mo_real_image"

if [ -d "../src" ]; then
  cp -r ../src/* ./tmp_sources
else
  echo "Error: Source directory ../src does not exist. Exiting."
  exit 1
fi

echo "Inizio la build dell'immagine Docker: $IMAGE_NAME"


if docker build -t $IMAGE_NAME -f ./Dockerfile .; then
  rm -rf ./tmp_sources
  echo "Build completed. Start the container with: ./docker_run_container.sh"
else
  rm -rf ./tmp_sources
  echo "Rerun ./docker_build_image.sh and try again."
fi