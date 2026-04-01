#!/usr/bin/python3
# generate config.h from config.json

import argparse, math, json
from dataclasses import dataclass

# obj
lut = {
    '+': 'KEYPAD_ADD',
    '-': 'KEYPAD_SUBTRACT',
    'RET': 'RETURN',
    '.': "PERIOD",
    '/': "SLASH",
    '*': "KEYPAD_MULTIPLY",
}
@dataclass
class ConfigKey:
    """A key from config.json"""
    row: int
    col: int
    code: str

    def from_label(row, col, label):
        label = label.upper()
        if label in lut:
            label = lut.get(label)
        label = f"HID_KEY_{label}"
        return ConfigKey(row,col,label) # TODO

# set up args
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--infile", help="input config.json file location")
parser.add_argument("-o", "--outfile", help="output config.h file location")
parser.add_argument("-v", "--verbose", help="print useless logging", action="store_true")
args = parser.parse_args()

# verbose print
def vprint(s):
    if args.verbose:
        print(s)

vprint("verbose logging is enabled")

# config.h header
header = """// this file generated for parakeyt
#pragma once
#include "class/hid/hid.h"

// parameters
#define THRESH 50
#define NKRO 6
#define POLL 1

"""

# convert json string to config.h
def convert(json_s: str) -> (str, bool):
    # start
    vprint("converting json")
    cfg = json.loads(json_s);
    s = ""
    isvalid = True

    rows = set()
    cols = set()

    # internal rep.
    vprint("deriving internal representation")
    internal_keys = []
    if 'keys' in cfg:
        for key in cfg.get('keys'):
            x = math.floor(int(key.get('x')))  # TODO bad
            y = math.floor(int(key.get('y')))  # TODO bad
            internal_keys.append(ConfigKey.from_label(y, x, key.get('label')))
            rows.add(x)
            cols.add(y)
    else:
        print('WARN: no mapping defined. firmware will not compile!')
        isvalid = False

    # header
    s += header

    # adc cnt
    vprint("calculating IO map")
    adcs = math.ceil(len(cols) % 8)
    drivers = math.ceil(len(rows) % 8)

    s += "// IO expanders\n"
    s += f"#define ADC_CNT {adcs}\n"
    s += f"#define DRIVER_CNT {drivers}\n"

    s += "#define ADC_ADDRS { 0x17 }\n" # TODO
    s += "#define DRIVER_ADDRS { 0x16 }\n" # TODO
    s += "\n"

    # dimensions
    vprint("dimensions")
    s += "// dimensions\n"
    s += "#define LAYERS 1\n"
    s += f"#define ROWS {len(rows)}\n"
    s += f"#define COLUMNS {len(cols)}\n"
    s += "\n"

    # rowmap
    vprint("generating row mapping")
    s +="// rows: {driver, pin}\n"

    s += "#define ROWS_MAP { "
    for row in rows:
        s += f"{{ 0, {7 - row} }}, "
    s = s[:len(s)-2]
    s += " }\n"
    s += "\n"

    # colmap
    vprint("generating column mapping")
    s +="// cols: {driver, pin}\n"

    s += "#define COLS_MAP { "
    for col in cols:
        s += f"{{ 0, {col} }}, "
    s = s[:len(s)-2]
    s += " }\n"
    s += "\n"

    # keycode map
    vprint("generating keycode mapping")
    keymap = [["HID_KEY_NONE"] * len(cols) for _ in range(len(rows))]
    for key in internal_keys:
        keymap[key.row][key.col] = key.code
        vprint(f"{key.row}, {key.col}, {key.code}")

    s += "// keymap[layers][rows][cols]\n"
    s += "#define KEYCODE_MAP { { \\\n"
    for r in keymap:
        s += "{ "
        for c in r:
            s += f"{c}, "
        s = s[:len(s)-2]
        s += " }, \\\n"
    s = s[:len(s)-4]
    s += "  \\\n} }\n"

    return (s, isvalid)

# run
if args.infile != args.outfile != None:
    s = ""
    # read infile to string
    vprint(f"reading {args.infile}")
    with open(args.infile, 'r') as f:
        data = f.read()
        # convert
        s, isvalid = convert(data)
        if isvalid:
            vprint("config is probably valid")
        else:
            print("output config may be malformed!")

    # output to outfile
    vprint(f"writing {args.outfile}")
    with open(args.outfile, "w+") as f:
        f.write(s)
    print("conversion done")
else:
    print("argument error")

exit()

