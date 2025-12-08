import itertools


def implement_read_unsigned(bitdepth, endian, mode):
    typename = f"uint{bitdepth}_t"
    sizeof = bitdepth // 8
    print(
        f"ReadStatus reader_{mode}U{bitdepth}_{endian}(Reader* reader, {typename}* out)"
    )
    print("{")
    print("    Slice slice;")
    print(f"    TRY(reader_peekSlice(reader, {sizeof}, &slice));")
    print("")
    print(f"    *out = bitcastU{bitdepth}_{endian}(slice.slice);")
    if mode == "peek":
        print("    return ReadStatus_Ok;")
    else:
        print(f"    return reader_skip(reader, sizeof);")
    print("}")
    print("")


if __name__ == "__main__":
    bitdepths = [16, 32, 64]
    endianness = ["LE", "BE"]
    modes = ["peek", "take"]

    # implement_read_unsigned(16, "LE", "peek")

    for bitdepth, endian, mode in itertools.product(bitdepths, endianness, modes):
        implement_read_unsigned(bitdepth, endian, mode)
