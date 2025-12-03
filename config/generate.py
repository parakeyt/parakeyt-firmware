#!/usr/bin/python3
# generate config.h from config.json

import argparse, math, json

# set up args
parser = argparse.ArgumentParser()
parser.add_argument("verb", help="specify action ('test' or 'convert')")
parser.add_argument("-i", "--infile", help="input config.json file location")
parser.add_argument("-o", "--outfile", help="output config.h file location")
parser.add_argument("-v", "--verbose", help="print useless logging", action="store_true")
args = parser.parse_args()

# verbose print
def vprint(s):
    if args.verbose:
        print(s)

vprint("verbose logging is enabled")

# conversion consts
PINS_PER_ADC = 8
PINS_PER_DRIVER = 8
MAX_SENSORS_PER_DRIVER = 24

# get column by position
# RET col: int
col_cnt = 0
col_map = {}
def get_col_from_pos(pos):
    global col_map, col_cnt
    key = str(math.floor(pos[0]))
    if not (key in col_map):
        col_map[key] = col_cnt
        col_cnt += 1;
    return col_map[key]

# get row by position
# RET row: int
row_cnt = 0
row_map = {}
def get_row_from_pos(pos):
    global row_map, row_cnt
    key = str(math.floor(pos[1]))
    if not (key in row_map):
        row_map[key] = row_cnt
        row_cnt += 1;
    return row_map[key]

# convert json string to config.h
# RET (cfg: string, isvalid: bool)
def convert(json_s):
    # clear
    global col_cnt, row_cnt, row_map, col_map
    col_cnt = row_cnt = 0
    col_map = {}
    row_map = {}

    # start
    vprint("converting json")
    cfg = json.loads(json_s);
    s = ""
    isvalid = True

    # key layers
    vprint('deriving key layers')
    if 'mapping' in cfg:
        m_len = len(cfg.mapping[0])
        trail = ", \\\n"

        s += "#define KEY_LAYERS = ["
        for mapping in cfg.mapping:
            if len(mapping) != m_len:
                vprint(f"mapping len {len(mapping)} != {m_len}")
                print('WARN: no mapping defined. firmware may not function!')
                isvalid = False

            # TODO get every1 on the KC stuff ima use
            s += mapping.to_string()
            s += trail
        s = s[:-1*len(trail)] # remove trailing comma
        s += "]\n"
    else:
        print('WARN: no mapping defined. firmware will not compile!')
        isvalid = False

    # pinout
    vprint('deriving pinout')
    if 'keys' in cfg:
        trail = ", "

        # sort keys to maintain alg precondition
        keys = sorted(cfg['keys'], key=lambda x: 1000*x['pos'][0] + x['pos'][1])

        # key pinout
        s += "#define KEY_PINS = ["
        for key in keys:
            row = get_row_from_pos(key['pos'])
            col = get_col_from_pos(key['pos'])
            s += f"[{row}, {col}]"
            s += trail
        s = s[:-1*len(trail)]
        s += "]\n"

        # n adcs
        n_adcs = math.ceil(col_cnt / PINS_PER_ADC)
        s += f"#define ADC_CNT {n_adcs}\n"

        # n drivers
        n_drivers = math.ceil(row_cnt / PINS_PER_DRIVER)
        s += f"#define DRIVER_CNT {n_drivers}\n"
    else:
        print('WARN: no keys defined. firmware will not compile!')
        isvalid = False

    # sanity check
    vprint('performing sanity checks')
    if isvalid and (len(cfg.keys != cfg.mapping[0])):
        print('WARN: mismatch between number of keys and mapping. firmware will not compile!')
        isvalid = False

    return (s, isvalid)


# run a single test
failures = []
passes = []
def test(name, j, r):
    global failures, passes
    print(name)
    c, _ = convert(j)
    if c == r:
        passes.append(name)
        print('PASS')
    else:
        failures.append(name)
        print("EXPECTED:\n\n")
        print(r)

        print("\n\nGOT:\n\n")
        print(c)

        print("\n\nFAILURE")

# run tests
def run_tests():
    # simple keys
    j = '{"keys": [ {"pos": [0,1,2], "size": 1 }, {"pos": [3,4,5], "size": 1 } ]}'
    r = '''#define KEY_PINS = [[0, 0], [1, 1]]
#define ADC_CNT 1
#define DRIVER_CNT 1
'''
    test("simple keys", j, r)

    print("\n\nTest summary:")
    for p in passes:
        print(f"PASS: {p}")
    print()
    for f in failures:
        print(f"FAILURE: {f}")

# run
if args.verb == "test":
    print("running tests")
    run_tests()
elif args.verb == "convert" and args.infile != args.outfile != None:
    s = ""
    # read infile to string
    vprint(f"reading {infile}")
    with open(infile, 'r') as f:
        data = f.read()
        # convert
        s, isvalid = convert(data)
        if isvalid:
            vprint("config is probably valid")
        else:
            print("output config may be malformed!")

    # output to outfile
    vprint(f"writing {outfile}")
    with open("Output.txt", "w+") as f:
        f.write(s)
    print("conversion done")
else:
    print("please specify verb as either 'test' or 'convert' with in/out files")

exit()

