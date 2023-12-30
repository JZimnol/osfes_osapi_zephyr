# OSfES OSAPI Zephyr port

This is a Zephyr implementation of OSAPI for Operating Systems for Embedded
Systems university classes.

## Running

Download this repository to you zephyr project directiory (e.g.
`~/zephyrproject`). Then run:
```
west config manifest.path osfes_osapi_zephyr
west config manifest.file west.yml
west update
```

Also, ensure that these options are checked in the `prj.conf` file:
```
CONFIG_OSAPI_USE_ZEPHYR=y
CONFIG_OSAPI_WITH_CUNIT=y
```

Then run:
```
west build -b <board_name>
west flash
```

## CUnit tests

To be able to run unit tests using CUnit, a special (and primitive) patch has
been prepared (`cunit_patch.patch`). This patch is being applied each build
automatically.
