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
        line['opname'].replace('.','').replace('/', ''),
        ''.join(test[4]),
        *(test[0:4]),
    ))

def _ivk_gen_c(info: Any, line: Any) -> Any:
    """ generate C information
    """
    opname = line['opname']
    opname = opname.replace('.', '')
    opname = opname.replace('/', '')
    info['c'].append((
        f"\"{line['opname']}\"",
        f"{line['opdesc']}",
        f"__{opname}_ivk_exec",
    ))

def _ivk_gen_file_asm(prefix: Path, asm: Any) -> None:
    """ generate ASM file
    """
    asm_content = (
        '//---\n'
        '// declare all instructions IVK tracpoline\n'
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

def _ivk_gen_file_c(prefix: Path, c: list[tuple[str,str,str]]) -> None:
    """ generate the GUI information
    """
    c_content = (
        '//---\n'
        '// declare all instructions GUI information\n'
        '//---\n'
        '#include <stddef.h>\n'
        '#include <stdint.h>\n'
        '#include "ivk/instruction.h"\n'
        '\n'
        '// all external symbols needed (assumed defined)\n'
    )
    for op in c:
        c_content += f"extern uintptr_t {op[2]};\n"
    c_content += '\n'
    c_content += '// define all instruction information\n'
    c_content += 'struct instruction_gui_info instruction_table[] = {\n'
    for op in c + [('NULL', 'NULL', 'NULL')]:
        addr = f"(void*)&{op[2]}" if op[2] != 'NULL' else op[2]
        c_content +=  '    {\n'
        c_content += f"        .opname = {op[0]},\n"
        c_content += f"        .desc = {op[1]},\n"
        c_content += f"        .trampoline = {addr},\n"
        c_content +=  '    },\n'
    c_content += '};'
    c_path = prefix/'instructions_cpu.c'
    c_path.unlink(missing_ok=True)
    with open(c_path, 'x', encoding='utf-8') as c_file:
        c_file.write(c_content)

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
        r'(?P<opname>([\w\.\/]+))\s+'
        r'(?P<opdesc>("(.)+"))'
    )
    for i, line in enumerate(decl.split('\n')):
        if not (line := line.strip()):
            continue
        if line.startswith('//'):
            continue
        if not (line_info := line_re.match(line)):
            raise _ivkException(f"unable to parse the line {i}: {line}")
        _ivk_gen_asm(info, line_info)
        _ivk_gen_c(info, line_info)
    _ivk_gen_file_asm(decl_path.parent, info['asm'])
    _ivk_gen_file_c(decl_path.parent, info['c'])

#---
# Public
#---

if __name__ == '__main__':
    try:
        _ivk_gen()
        sys.exit(0)
    except _ivkException as err:
        print(f"\033[32m{err}\033[0m")
        sys.exit(1)
