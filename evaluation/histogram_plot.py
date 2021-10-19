import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np

mpl.rcParams.update({'font.size': 11, 'mathtext.fontset': 'stix'})

patch_types = ["without_patch", "dynamic_patch"]
app_types = ["coap", "usbmass", "mqtt"]
ylabels = ["Number of Request", "Number of I/O", "Number of PUBLISH"]
xlabels = ["Histogram of CoAP GET Request Latency [µs]", "Histogram of USB Mass Storage I/O Latency [s]", "Histogram of MQTT PUBACK Latency [s]"]
labels = ["Without Patch", "Dynamic Patch Point"]
colors = ["c", "orange"]
xlims = [(0, 400), (0.007, 0.012), (0, 0.6)]

n_rows = 2
n_cols = 3
fig, axes = plt.subplots(n_rows, n_cols, figsize=(15,6), sharex=False, sharey=False,  squeeze=False)

clocks = []

for row_num in range(n_rows):
    for col_num in range(n_cols):
        ax = axes[row_num][col_num]

        with open("data/histogram_" + app_types[col_num] + "_" + patch_types[row_num] + ".txt", "r", encoding="utf-8") as ifile:
            lines = ifile.readlines()
            clocks = [float(x) for x in lines]
            clocks = np.array(clocks)

            ax.hist(clocks, bins=20, color=colors[row_num], edgecolor='k', alpha=0.65, label=labels[row_num])

            ax.set_ylabel(ylabels[col_num])
            ax.set_xlabel(xlabels[col_num])
                
            ax.axvline(np.mean(clocks), color='indigo', linestyle='-', linewidth=2, label='Mean {:.5f}'.format(np.mean(clocks)))
            ax.axvline(np.median(clocks), color='r', linestyle='dashed', linewidth=2, label='Median {:.5f}'.format(np.median(clocks)))
            ax.axvline(np.percentile(clocks, 99.5), color='b', linestyle='-.', linewidth=2, label='P99.5 {:.5f}'.format(np.percentile(clocks, 99.5)))

            ax.set_xlim(xlims[col_num][0], xlims[col_num][1])
            # min_ylim, max_ylim = ax.get_ylim()
            # ax.text(np.mean(clocks)*1.1, max_ylim*0.85, 'Mean: {:.2f}'.format(clocks.mean()))
            # ax.text(np.median(clocks)*0.45, max_ylim*0.85, 'Median: {:.2f}'.format(clocks.mean()), bbox=dict(facecolor='red', alpha=0.5))

            plt.sca(ax)
            plt.legend(loc=1, fontsize=10)

plt.subplots_adjust(hspace=0.4, left=0.05, right=0.99, top=0.95, bottom=0.1)
# plt.subplots_adjust(hspace=0.4)
plt.show()
fig.savefig("macro_overhead_evaluation.pdf")

