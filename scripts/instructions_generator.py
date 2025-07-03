"""
instruction_generator   - generate instruction from declaration file
"""
from typing import Any
from pathlib import Path
import sys
import re

#---
# Internals
#---

class _ivkException(Exception):
    """ internal exceptin """

def _ivk_gen_asm(info: Any, line: Any) -> None:
    """ generate ASM information """
    test = ['x', 'x', 'x', 'x', ['i','i','i','i']]
    for i, x in enumerate(line['opcode'].split('.', 4)):
        test[i] = f"0b{x}"
        if x[0] not in 'nm':
            continue
        test[4][i] = x[0]
        test[i] = f"ivk_r{x[0]}"
    info['asm'].append((
        line['opname'].replace('.',''),
        ''.join(test[4]),
        *(test[0:4]),
    ))

def _ivk_gen_c(info: Any, line: Any) -> Any:
    """ generate C information
    """
    info['c'].append((
        line['opname'],
        line['opdesc'],
        f"__{line['opname'].replace('.','')}_ivk_exec",
    ))

def _ivk_gen_file_asm(prefix: Path, asm: Any) -> None:
    """ generate ASM file
    """
    asm_content = (
        '//---\n'
        '// declare all instructions IVK trampoline\n'
        '//---\n'
        '#include "./ivk/ivk_utils.h"\n'
        '.text\n'
        '\n'
    )
    for op in asm:
        asm_content += f"ivk_declare_instruction({', '.join(op)})\n"
    asm_path = prefix/'instructions_cpu.S'
    asm_path.unlink(missing_ok=True)
    with open(asm_path, 'x', encoding='utf-8') as asm_file:
        asm_file.write(asm_content)

def _ivk_gen() -> None:
    """ generate instruction information
    """
    decl_path = Path(f"{__file__}/../../src/instruction-cpu.def").resolve()
    if not decl_path.exists():
        raise _ivkException("Unable to find the declaration file")
    with open(decl_path, 'r', encoding='utf8') as decl_file:
        decl = decl_file.read()
    info = {
        'asm': [],
        'c': [],
    }
    line_re = re.compile(
        r'(?P<opcode>(([10]{4}|n{4}|m{4})\.){3}([10]{4}|n{4}|m{4}))\s+'
        r'(?P<opname>([\w\.]+))\s+'
        r'(?P<opdesc>("(.)+"))'
    )
    for i, line in enumerate(decl.split('\n')):
        if not (line := line.strip()):
            continue
        if line.startswith('//'):
            continue
        if not (line_info := line_re.match(line)):
            raise _ivkException(f"unable to parse the line {i}")
        _ivk_gen_asm(info, line_info)
        _ivk_gen_c(info, line_info)
    print(info)
    _ivk_gen_file_asm(decl_path.parent, info['asm'])

#---
# Public
#---

if __name__ == '__main__':
    try:
        _ivk_gen()
        print('instruction information generated!')
        sys.exit(0)
    except _ivkException as err:
        print("\033[32m{err}\033[0m")
        sys.exit(1)
