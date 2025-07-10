# IVK - An Renesas SuperH4 ISA tester

// todo : image

This is a small project designed to provide an application for Casio
calculators that allows to view the behavior of the CPU during the
execution of specific instructions.

The project was created to aid in debugging the MQ project, an emulator for
Casio calculators under development. The MQ project aims to provide support
for most of the add-ins that the community has built over the last two
decades. For more context, you can explore the repository of the
[MQ project](https://git.planet-casio.com/Lephenixnoir/mq).

---

## Todo

- [ ] support `#imm` opcodes
- [ ] support `STC`/`STS` opcodes
- [ ] support `LDC`/ `LDS` opcodes
- [ ] support data access (`@Rn`)opcodes
- [ ] support DSP instruction
- [ ] support USB interface with `mq`
- [ ] support fx9860/fxcp400 device (only GUI rework needed)

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

Otherwise, you can manually build the project by using the `fxsdk` tool from
the `gint` ecosystem (since the project is based on this kernel). See how to
install the whole toolchain here.

## How to use?

The keyboard integration is not great for now, but usable:
- `UP`/`DOWN` / `EXE` - select in list
- `XOT`/`FRAC` - change the `Rn` register ID
- `LOG`/`F->D` - change the `Rm` register ID
- `LN`/ `SIN` / `COS` / `TAN` - M,Q,S,T bit toggle
- `(` / `)` - change `Rn` data
- `,` / `->` - change `Rm` data
- `EXE` - start executing the instruction
- `EXIT` - return to the selection list
- `MENU` - return to OS menu

## How it works?

I use a lot of unconventional assembly shenanigans to get it to work. The main
idea behind this project is to dynamically generate a small binary stub with
an arbitrary interface on which `r0` contain the `Rn` data and `r1` the `Rm`
data. A small prologue is generated to move content of `r0` and `r1` into the
desired `Rn` and `Rm` register, then the target instruction is generated, then
an epoligue is also generated to restore the output of Rn and Rm in `r0` and
`r1`.

```asm
// generated stub when Rm ID is not R0
mov     r1, rm
mov     r0, rn
...
mov     rn, r0
rts
mov     rm, r1
```
Note that if targets register collide with the epilogue, an other
variation is generated.

```asm
// generated stub when Rm is R0
mov     r0, r2
mov     r1, rm  // <-- assured r1 -> r0
mov     r2, rn
...
mov     rn, r2
mov     rm, r1
rts
mov     r2, r0
```

You can check the generation of epilogue/prologue here.

The execution of any stub are performed atomically, meaning that no
interruption can interrupt the execution (unless an exception occur). Thus,
you cannot select the `r15` for any tests (some checks are performed during
the generation of the stub and before the execution).

Concerning which instruction are supported, you can check the
`instruction-cpu.def` file that contain all of the description of any
instructions. This file is processed using the
`script/generate_instruction.py` which generate all "trampoline" stubs for
each instruction and the instruction information list displayed in the main
menu.

```asm
// exemple of "trampoline" for the instruction `mul.l rn, rm`
ivk_declare_instruction(mull, inmi, 0b0000, ivk_rn, ivk_rm, 0b0111)
```
You can check the `ivk_declare_instruction` macro here for more information.

So, if you want to add a new instruction, simply add a line in this
file and it will automatically added (note that not all instructions are
supported for now).

## Why IVK?

Because I was convinced that `invocate` was spelled `invokate`, and since I
have used this word a lot inside header and source files…I chose to keep some
letters instead of fixing the word. It’s a small quirk, but it’s part of the
project’s identity.
