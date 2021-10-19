import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np

# mpl.rcParams.update({'font.size': 16, 'font.family': 'STIXGeneral', 'mathtext.fontset': 'stix'})
mpl.rcParams.update({'font.size': 12, 'mathtext.fontset': 'stix'})

boards = ["NRF52840", "STM32F429", "STM32L475"]
tests = ["USB Mass Storage", "CoAP Server"]
xlabels = ["Data I/O Times", "GET Request Times"]
ylabels = ["Latency [s]", "Latency [ms]"]

def plot_USBmass_without_patch():
    filesize = []
    timeused = []
    with open("data/perf_usbmass_without_patch.txt", "r", encoding="utf-8") as ifile:
        lines = ifile.readlines()
        for line in lines:
            line = line.split(" ")
            filesize.append(int(line[0]))
            timeused.append(float(line[1]))

    plt.figure(figsize=(10,7)) # 10 is width, 7 is height
    plt.plot(filesize, timeused, marker='o', label='Without patch', color='orange')  # blue stars
    plt.title('USB mass storage performance')  
    plt.xlabel('Data/kB')
    plt.ylabel('Time/s')
    # plt.xlim(0, 6)
    # plt.ylim(0, 10)
    plt.legend(loc='best')
    plt.show()

def plot_COAP_without_patch():
    filesize = []
    timeused = []
    with open("data/perf_coap_without_patch.txt", "r", encoding="utf-8") as ifile:
        lines = ifile.readlines()
        for line in lines:
            line = line.split(" ")
            filesize.append(int(line[0]))
            timeused.append(float(line[1]))

    plt.figure(figsize=(10,7)) # 10 is width, 7 is height
    plt.plot(filesize, timeused, marker='o', label='Without patch', color='orange')  # blue stars
    plt.title('CoAP GET request performance')  
    plt.xlabel('Request Number')
    plt.ylabel('Time/ms')
    # plt.xlim(0, 6)
    # plt.ylim(0, 10)
    plt.legend(loc='best')
    plt.show()

# plot_COAP_without_patch()
# plot_USBmass_without_patch()

def format_times(x, pos=None):
    x = int(x)
    if str(x)[-1] == '1':
        return str(x) + "st"
    elif str(x)[-1] == '2':
        return str(x) + "nd"
    elif str(x)[-1] == '3':
        return str(x) + "rd"
    else:
        return str(x) + "th"

n_rows = 2
n_cols = 3
fig, axes = plt.subplots(n_rows,n_cols, figsize=(13,6), sharex=False, sharey=False)

for row_num in range(n_rows):
    for col_num in range(n_cols):
        ax = axes[row_num][col_num]
        transize = []
        timeused = []
        with open("data/_" + str(row_num) + "_" + str(col_num) + ".txt", "r", encoding="utf-8") as ifile:
            lines = ifile.readlines()
            for line in lines:
                line = line.split(" ")
                transize.append(int(line[0]))
                timeused.append(float(line[1]))
        transize = np.array(transize)
        timeused = np.array(timeused)
        if row_num == 0:
            ax.plot(transize, timeused, marker='o', markersize=4, label='Without Patch', color='orange')
            ax.plot(transize, timeused + 0.1, marker='*', markersize=4, label='Fixed Patch Point', color='blue')
            ax.plot(transize, timeused + 0.2, marker='d', markersize=4, label='eBPF VM', color='yellow')
            ax.plot(transize, timeused + 0.3, marker='>', markersize=4, label='eBPF JIT', color='green')
        else:
            ax.plot(transize, timeused, marker='o', markersize=4, label='Without Patch', color='orange')
            ax.plot(transize, timeused + 10, marker='*', markersize=4, label='Fixed Patch Point', color='blue')
            ax.plot(transize, timeused + 20, marker='d', markersize=4, label='eBPF VM', color='yellow')
            ax.plot(transize, timeused + 30, marker='>', markersize=4, label='eBPF JIT', color='green')            
        if row_num == 0:
            ax.set_title(boards[col_num])
        ax.set_xlabel(xlabels[row_num])
        ax.xaxis.set_major_formatter(format_times)
        plt.sca(ax)
        plt.xticks(rotation=45, fontsize=9)
        # ax.set_ylabel(ylabels[row_num])

axes[0][0].set_ylabel(tests[0] + "\n" + ylabels[0])
axes[1][0].set_ylabel(tests[1] + "\n" + ylabels[1])

lines, labels = fig.axes[-1].get_legend_handles_labels()
# fig.suptitle('HotPatch Overhead Evaluation', fontsize=18)
fig.tight_layout()
# axes[0][2].legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
# fig.legend(lines, labels, bbox_to_anchor=(0, 1, 1, 0), loc="lower left", mode="expand", ncol=4)
fig.legend(lines, labels, mode="expand", ncol=4, loc="lower center", bbox_to_anchor=(0.09, 0, 0.88, 0.5))
plt.subplots_adjust(top=0.9, bottom=0.2, hspace=0.6)
plt.show()
fig.savefig("overhead-evaluation.pdf")

