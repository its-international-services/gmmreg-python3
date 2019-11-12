# GMMreg Python 3

This repository contains library GMMreg (https://github.com/bing-jian/gmmreg) adjusted to be used with Python 3.

This instruction was only tested on Ubuntu 18.04. The library was tested on Ubuntu 18.04 and Debian 9.

## Prerequisites

### GCC

To build and install the library, it's required to install GCC first:

```sh
sudo apt-get install gcc
```

#### Python dependencies

To use the library in Python, the following dependencies are required:

```sh
pip install numpy
pip install matplotlib
pip install scipy
pip install configobj
```

## Installation

```sh
git clone https://github.com/its-international-services/gmmreg-python3
cd gmmreg-python3/Python/
python setup.py build
python setup.py install
```

## Usage

```sh
import gmmreg
```
