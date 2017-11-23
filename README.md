# Why x86_64
* new CPU support virtualization
* high performance
* total new cpu model from i386 model
## run demo
run examples-v2/hello/hello_world_c in qemu

boot with grub multiboot

the osloader :  https://github.com/mzeric/osloader
```
set timeout=1
set default=0

menuentry "rtems"{

	multiboot /osloader
	module /hello.exe
	boot
}


```

![qemuserial](https://github.com/mzeric/rtems-x86_64/raw/master/doc/qemu_serial.png)

## Support Status

* x86_64 bsp
* multithread
* pass All RTEMS testsuites


## Build

build the toolchain

```
cd rtems-source-builder
cd rtems
../source-builder/sb-set-builder --prefix=/opt/rtems/toolchains/x86_64 5/rtems-x86_64

```

add "/opt/rtems/toolchains/x86_64/bin" to $PATH

bootstrap the source

```
cd rtems-x86_64
./bootstrap -c && ./bootstrap -p
../rtems-source-builder/source-builder/sb-bootstrap
```

build rtems

```
mkdir build
cd build
../rtems/configure --prefix=/opt/rtems/x86_64 --target=x86_64-rtems5 --disable-cxx
```

### rtems-source-builder
use rtems-source-builder to build x86_64-rtems5-xxx


This is the Real-Time Executive for Multiprocessing Systems (RTEMS).
The version number for this software is indicated in the VERSION file.

See the documentation manuals in doc/ with daily builds available online at
http://rtems.org/onlinedocs/doc-current/share/rtems/html/ and released builds
at http://www.rtems.org/onlinedocs/releases/ for information on building,
installing, and using RTEMS. The INSTALL file tells you to come back here.

See the RTEMS Wiki at http://devel.rtems.org for community knowledge and
tutorials.

RTEMS Doxygen available at http://www.rtems.org/onlinedocs/doxygen/cpukit/html

The RTEMS Project maintains mailing lists which are used for most
discussions:

* For general-purpose questions related to using RTEMS, use the
  rtems-users ml: https://lists.rtems.org/mailman/listinfo/users
* For questions and discussion related to development of RTEMS, use the
  rtems-devel ml: https://lists.rtems.org/mailman/listinfo/devel

See https://devel.rtems.org/ to view existing or file a new issue
report ticket.

## TODO
