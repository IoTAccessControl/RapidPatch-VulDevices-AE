import statistics

cycles = []
in_cycles = []
out_cycles = []
ebpf_cycles = []

times = []
in_times = []
out_times = []
ebpf_times = []

def get_info(val_list):
    min_val = min(val_list)
    max_val = max(val_list)

    return (min_val, max_val , int(statistics.mean(val_list)), int(statistics.median(val_list)))



def profile_parse(filename):
    cycles = []
    in_cycles = []
    out_cycles = []
    ebpf_cycles = []

    times = []
    in_times = []
    out_times = []
    ebpf_times = []
    print(filename)
    with open(filename, "r", encoding='utf-8') as ifile:
        lines = ifile.readlines()
        for line in lines:
            if "cycle:" in line:
                line = line.strip("␍␊\n")
                index = line.find("cycle:")
                subline = line[index:]
                fregs = subline.split(" ")
                cycles.append(int(fregs[1]))
                times.append(int(fregs[3]))

    # print(cycles)
    # print(times)
    for i in range(len(cycles)):
        if i%3 == 0:
            in_cycles.append(cycles[i])
            ebpf_cycles.append(cycles[i + 1])
            out_cycles.append(cycles[i + 2])

            in_times.append(times[i])
            ebpf_times.append(times[i + 1])
            out_times.append(times[i + 2])

    print("Switch in cycles")
    print(get_info(in_cycles))
    print("eBPF cycles")
    print(get_info(ebpf_cycles))
    print("Switch out cycles")
    print(get_info(out_cycles))
    print("Switch in time")
    print(get_info(in_times))
    print("eBPF time")
    print(get_info(ebpf_times))
    print("Switch out time")
    print(get_info(out_times))

profile_parse("data/nrf52840_dynamic_micro_profile.txt")
profile_parse("data/nrf52840_fixed_micro_profile.txt")
profile_parse("data/stm32f429_dynamic_micro_profile.txt")
profile_parse("data/stm32f429_fixed_micro_profile.txt")