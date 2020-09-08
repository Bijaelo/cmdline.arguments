# About the docker folder

This folder contains a simple docker images for reproducibility. It contains 2 images:

1. **dockerimage.build**
1. **dockerimage.test**

dockerimage.build is an image based on rocker/rstudio:version. This image is used as a development environment for the package and has all the necessary dependencies pre-installed.  
dockerimage.test is build upon dockerimage.build and is intended for testing that the package succesfully installs and passes various tests on a given version of R with given version of dependencies. 
To make sure that dockerimage.test is reproducible the image will pull the specific version from the github repository bijaelo/cmdline.arguments.

Versions of dockerimage.test that succesfully build and passes all tests are automatically pushed to dockerhub at [bijaelo/cmdline.arguments:pkg.ver-R.ver](https://hub.docker.com/search?q=bijaelo&type=image) and are thus easily usable on azure, aws, valohai etc. using `docker pull bijaelo/cmdline.arguments:pkg.ver-R.ver`. A complete list of compatibility for the latest release is additionally also available at the top of the [github page](https://github.com/Bijaelo/cmdline.arguments)
