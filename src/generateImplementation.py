import itertools


def implement_read_integer(mode, shortType, containerType, size, endian):
    functionName = f"reader_{mode}{shortType}_{endian}"
    bitcastCall = f"bitcast{shortType}_{endian}(slice.slice)"

    print(f"ReadStatus {functionName}(Reader* reader, {containerType}* out)")
    print("{")
    print("    Slice slice;")
    print(f"    TRY(reader_peekSlice(reader, {size}, &slice));")
    print("")
    print(f"    *out = {bitcastCall};")
    if mode == "peek":
        print("    return ReadStatus_Ok;")
    else:
        print(f"    return reader_skip(reader, {size});")
    print("}")
    print("")


def declare_read_integer(mode, shortType, containerType, size, endian):
    functionName = f"reader_{mode}{shortType}_{endian}"
    print(f"ReadStatus {functionName}(Reader* reader, {containerType}* out);")

if __name__ == "__main__":
    bitdepths = [16, 32, 64]
    signedness = [("U", "uint"), ("I", "int")]

    types = [
        [f"{prefix}{bitdepth}", f"{c_type_prefix}{bitdepth}_t", bitdepth // 8]
        for bitdepth in bitdepths
        for prefix, c_type_prefix in signedness
    ]

    types += [
        ["I24", "int32_t", 3],
        ["U24", "uint32_t", 3],
        ["F32", "float", 4],
        ["F64", "double", 8],
    ]

    # stringly typed development
    endianness = ["LE", "BE"]
    modes = ["peek", "take"]
    signedness = ["signed", "unsigned"]

    for T, endian, mode in itertools.product(types, endianness, modes):
        # implement_read_integer(mode, T[0], T[1], T[2], endian)
        declare_read_integer(mode, T[0], T[1], T[2], endian)
