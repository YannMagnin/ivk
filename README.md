# IVK - An Renesas SuperH4 ISA tester

// todo : image

This is a small project designed to provide an application for Casio
calculators that allows users to view the behavior of the CPU during the
execution of specific instructions.

The project was created to aid in debugging the MQ project, an emulator for
Casio calculators under development. The MQ project aims to provide support
for most of the add-ins that the community has built over the last two
decades. For more context, you can explore the repository of the
[MQ project](https://git.planet-casio.com/Lephenixnoir/mq).

---

## How to install?

Select the add-in file depending on your calculator and install the file on
the root folder of the device via classic USB transfer.

A special manipulation may be required on some devices where the add-in
support has been discontinued.

| Device    | File          | CFW |
|:---------:|:-------------:|:---:|
| `fx9860`  | `ivk.g1a`     | N/a |
| `fxcg50`  | `ivk.g3a`     | N/a |
| `fxcg50`  | `ivk.g3a`     | [MPM](https://www.planet-casio.com/Fr/forums/topic18534-1-mpm-mod-add-ins-math.html) |
| `fxcp400` | `ivk-hh2.bin` | [HollyHock-3](https://github.com/ClasspadDev/hollyhock-3) |

## How to use?

todo

## How it works?

I use a lot of unconventional shenanigans and "technique de rat" (like we say
in French) to get it to work.
todo : what have i done?!

## Why IVK?

Because I was convinced that `invocate` was spelled `invokate`, and since I
have used this word a lot inside header and source files…I chose to keep some
letters instead of fixing the word. It’s a small quirk, but it’s part of the
project’s identity.
