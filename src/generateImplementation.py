import itertools


def implement_read_integer(bitdepth, endian, mode, signedness):
    if signedness == "signed":
        typename = f"uint{bitdepth}_t"
        shortType = f"U{bitdepth}"
    else:
        typename = f"int{bitdepth}_t"
        shortType = f"I{bitdepth}"

    functionName = f"reader_{mode}{shortType}_{endian}"
    bitcastCall = f"bitcast{shortType}_{endian}(slice.slice)"

    sizeof = bitdepth // 8
    print(f"ReadStatus {functionName}(Reader* reader, {typename}* out)")
    print("{")
    print("    Slice slice;")
    print(f"    TRY(reader_peekSlice(reader, {sizeof}, &slice));")
    print("")
    print(f"    *out = {bitcastCall};")
    if mode == "peek":
        print("    return ReadStatus_Ok;")
    else:
        print(f"    return reader_skip(reader, {sizeof});")
    print("}")
    print("")


if __name__ == "__main__":
    bitdepths = [16, 32, 64]
    # stringly typed development
    endianness = ["LE", "BE"]
    modes = ["peek", "take"]
    signedness = ["signed", "unsigned"]

    implement_read_integer(16, "LE", "peek", "signed")
    implement_read_integer(64, "BE", "take", "unsigned")

    # for bitdepth, endian, mode in itertools.product(bitdepths, endianness, modes):
    #    implement_read_unsigned(bitdepth, endian, mode)
