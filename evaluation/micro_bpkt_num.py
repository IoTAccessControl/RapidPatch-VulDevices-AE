import numpy as np
from pylab import rcParams
import seaborn as sns
import matplotlib.pyplot as plt

# sns.set_style("white")
style_label = "default"

green = "mediumseagreen"
red = "salmon"
blue = "steelblue"

def bar_figure1(ax):
    y_pos = [0.38, 1.56, 1.80, 2.14, 2.48]
    n_groups = [1, 4, 8, 16, 32]
    index = np.arange(len(n_groups))
    width = 0.2
    ax.bar(index, y_pos, width, align='center', color=green) #'#B9E0A5'
    # ax.set_yticks(y_pos)
    ax.set_xticks(index)
    ax.set_xticklabels(n_groups)
    # ax.set_xlabel('Performance')
    ax.set_title('Dispatching delay of different patch number')

def box_figure2(ax):
    # Random test data
    np.random.seed(123)
    # all_data = [np.random.normal(0, std, 100) for std in range(1, 4)]
    # print(all_data)
    trigger_ti = [3.9, 3.5, 2.2]
    patch_ti = [[1.7, 1.6, 14.7,  2.0, 1.5, 4.4, 1.8, 5.5, 1.8, 1.4, 1.6, 3.9], 
                [1.4, 1.35, 11.8, 1.65, 1.3, 3.6, 1.5, 4.7, 1.5, 1.2, 1.38, 2.8],
                [1, 0.9, 1.1, 5.8, 0.8, 1.8, 1, 1.4, 0.9, 0.8, 0.9, 1.3]
                ]
    all_data = [[0] * 11 for t in range(3)]
    for i in range(3):
        for j in range(11):
            all_data[i][j] = trigger_ti[i] + patch_ti[i][j]
    # rectangular box plot  
    bplot1 = ax.boxplot(all_data,
                         vert=True,   # vertical box aligmnent
                         patch_artist=True)   # fill with colo
    edge_color = red # "#B9E0A5"
    for box in bplot1["boxes"]:
        # box.set(color=edge_color)
        box.set_facecolor("#F19C99")
    ax.set_xticklabels(["NRF52840", "STM32L475", "STM32F429"])
    ax.set_title('Total patch delay in various devices')

def stacked_figure3(ax):
    N = 5
    Triggering = [3.9] * N
    Dispatching = [0.5] * N
    Executing = [1.7, 1.6, 2.0, 1.5, 4.4]
    ind = np.arange(N)    # the x locations for the groups
    width = 0.35       # the width of the bars: can also be len(x) sequence

    B1 = [0] * N
    B2 = [0] * N
    for i in range(N):
        B1[i] = Triggering[i]
        B2[i] = B1[i] + Dispatching[i]

    p1 = ax.bar(ind, Triggering, width, color= green) #'#B9E0A5'
    p2 = ax.bar(ind, Dispatching, width, bottom=B1, color=red) #'#F19C99'
    p3 = ax.bar(ind, Executing, width, bottom=B2, color=blue) #'#A9C4EB'

    # ax.set_ylabel('Scores')
    ax.set_title('Total patch delay for different CVEs')
    ax.set_xticks(ind)
    ax.set_xticklabels(['C1', 'C2', 'C4', 'C5', 'C6'])
    ax.set_yticks(np.arange(0, 14, 2))
    ax.legend(('Triggering', 'Dispatching', 'Executing'),
        loc='upper left', mode='expand', ncol=3)


def draw_figure():
    (fig_width, fig_height) = plt.rcParams['figure.figsize']
    fig_size = [fig_width * 2, fig_height / 2]
    fig, axes = plt.subplots(ncols=3, nrows=1, num=style_label,
                             figsize=fig_size, squeeze=True)
    axes[0].set_ylabel("Time (us)")
    bar_figure1(axes[0])
    box_figure2(axes[1])
    stacked_figure3(axes[2])
    plt.subplots_adjust(left=0.045, bottom=0.107, right=0.995, top=0.9, wspace=0.09)
    plt.show()

draw_figure()