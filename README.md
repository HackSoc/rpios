# Building

Fairly self explanatory. (Just run `make`). You'll need a suitable toolchain if not building on Arm: such as the [Arm embedded toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

In a kinda ugly fashion there's a bundled qemu in the repo, as stock qemu doesn't correctly emulate the raspberry pi. Eventually I'll pull that out into a submodule and upload the source, rather than distributing a binary... lol.
