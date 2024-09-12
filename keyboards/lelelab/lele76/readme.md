# lelelab/lele76

![lelelab/lele76](imgur.com image replace me!)

This sourcecode is my attempt to create a working firmware for the LeleLab Y2K. The original sourcecode is outdated / working with an unknown previous version of QMK. As long as the developer of the original sourcecode does not open his work to the public these files can be taken as a starting point to create a custom version of the firmware.

-   Keyboard Maintainer: [LeleLab](https://www.lelelab.work/)
-   Hardware Supported: Currently unknown
-   Hardware Availability: A complete kit can be purchased at [LeleLab](https://www.lelelab.work/).

Make example for this keyboard (after setting up your build environment):

    make lelelab/lele76:default

Flashing example for this keyboard:

    make lelelab/lele76:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

The default way to access the LeleLab Y2K bootloader is by using the "BOOT" and "RESET" keys below the acrylic top plate.

1. Hold down the BOOT key and do **not** release it.
2. Press and release the RESET key.
3. The bootloader will start up - you can release the BOOT key now as well.

## Useful commands

The `make` examples from above are not the way that I do things. I usually use the `qmk` command and that works just fine.

**Build**

```bash
qmk compile -kb lelelab/lele76 -km default
```

**Keymap Overview**

Show keymap as defined in info.json.

```bash
qmk info -kb lelelab/lele76 -km default -l
```

**Set default**

Set default keyboard and keymap:

```bash
qmk config user.keyboard=lelelab/lele76
qmk config user.keymap=default
```

After this `qmk compile` is enough to build the sourcecode.
