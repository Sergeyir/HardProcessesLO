# Installation tutorial

For simplicity we will consider arbitrary path $PACKAGE_PATH for the directory path where we will place all packages required for this project. This consideration is not a requirement and you can configure the paths yourself the way it is convenient for you. 

**Recommended $PACKAGE_PATH**: ~/Packages

**Later if you see $PACKAGE_PATH replace it with your path.** Or add it in your profile (~/.bashrc or ~/.zshrc, or other)

And don't forget to **create** this path first with the following command

```sh
mkdir -p $PACKAGE_PATH
```

## ROOT6

Install the [required dependencies](https://root.cern/install/dependencies/).

Then install ROOT6 via package manager (if available, [see here](https://root.cern/install/)) or pre-compiled release for your platform [here](https://root.cern/install/all_releases/).

### Pre-compiled release

If your option is the installation of pre-compiled release, after downloading place the .tar.gz file in $PACKAGE_PATH. After heading in $PACKAGE_PATH run to extract

```sh
tar -xvzf root*.tar.gz
```

Now extracted files are located at $PACKAGE_PATH/root which you should export in your profile (usually ~/.bashrc or ~/.zshrc; or other) to add search option in CMake or for python3 support. To do this as well as add easily accessible root executable add the following lines to your profile

```sh
export ROOT_PATH=$PACKAGE_PATH/root

source $ROOT_PATH/bin/thisroot.sh
```

### Installation with package manager

With package manager compiled root binaries will be located at /bin and libraries at /lib. In this case no $ROOT_PATH is not needed to be added to your profile.

### ROOT6 compilation

Alternatively, you can also compile ROOT6 yourself, however it will take quite a lot of computational power and might take a long time to compile. This step is **not recommended** if you do not have enough experience compiling packages from source.

## LHAPDF6

Download the source code from [here](https://lhapdf.hepforge.org/downloads/). Place the .tar.gz file in $PACKAGE_PATH. After heading there run to extract files

```sh
tar -xvzf LHAPDF-6*.tar.gz
```

Then head into LHAPDF-6.X.X directory (6.X.X is the version you downloaded, replace X for your version). First run to configure

```sh
mkdir build && ./configure --prefix=`pwd`/build
```

Then compile the code with the following command

```sh
make install -j
```

Lastly add the following lines to your profile (replace 6.X.X with your LHAPDF6 version and python3.XX with the version of python you compiled LHAPDF with - look for $PACKAGE_PATH/build/lib/python* directory)

```sh
export LHAPDF6_PATH=$PACKAGE_PATH/LHAPDF-6.X.X/build/
export LD_LIBRARY_PATH=$LHAPDF6_PATH/lib:$LD_LIBRARY_PATH
export PATH=$LHAPDF6_PATH/bin:$PATH
```

Also add the path to \_\_init.py\_\_ located somewhere in $LHAPDF6_PATH/lib/python3.XX/. This path might change with the distro, python, and LHAPDF versions. The resulting line you need to add might look like the following

```sh
export PYTHONPATH=$LHAPDF6_PATH/lib/python3.XX/site-packages:$PYTHONPATH
```

or

```sh
export PYTHONPATH=$LHAPDF6_PATH/lib/python3.XX/dist-packages/lhapdf:$PYTHONPATH
```

## PYTHIA8

Download the source code from [here](https://pythia.org/). Place the .tar.gz file in $PACKAGE_PATH. After heading there run to extract files

```sh
tar -xvzf pythia8*.tgz
```

Then head into pythia8XXX directory (8XXX is the version you downloaded, replace X for your version). First run to configure (you might need to change /bin/python-config to /bin/python3-config)

```sh
./configure --with-python-config=/bin/python-config --with-lhapdf6-config=$LHAPDF6_PATH/bin/lhapdf-config
```

Now compile the source code with the following command

```sh
make
```

After the compilation is finished successfully, add the following lines to your profile (replace X to match your pythia8 version number)

```sh
export PYTHIA8_PATH=$PACKAGE_PATH/pythia8XXX
export PYTHONPATH=$PYTHIA8_PATH/lib:$PYTHONPATH
export LD_LIBRARY_PATH=$PYTHIA8_PATH/lib:$LD_LIBRARY_PATH
```

## FASTJET3

Install [swig](https://www.swig.org/) package first (it can be installed with package manager). After, download the source code from [here](https://fastjet.fr/). Place the .tar.gz file in $PACKAGE_PATH. After heading there run to extract files

```sh
tar -xvzf fastjet-3*.tar.gz
```

Then head into fastjet-3.X.X directory (3.X.X is the version you downloaded, replace X for your version). First run to generate Makefile


```sh
mkdir build && ./configure --prefix=`pwd`/build --enable-pyext
```

After compile the code by running

```sh
make install -j
```

Lastly add to the your profile (replace 3.X.X and 3.XX with your fastjet and python version numbers respectively)

```sh
export FASTJET3_PATH=$PACKAGE_PATH/fastjet-3.X.X/build
export PYTHONPATH=$FASTJET3_PATH/lib/python3.XX/site-packages:$PYTHONPATH
export LD_LIBRARY_PATH=$FASTJET3_PATH/lib:$LD_LIBRARY_PATH
```

## How to check the installation

In python run

```py
import lhapdf
import pythia8
import fastjet
```

If no errors occurred, the installation was successful
