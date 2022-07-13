import matplotlib.pyplot as plt
import numpy as np

# use ggplot style for more sophisticated visuals
plt.style.use('ggplot')


class plotter:
    def __init__(self, title="", limits=False, y_label=""):
        self.size = 100
        self.x_vec = np.linspace(0, 1, self.size+1)[0:-1]
        self.y_vec = np.zeros(len(self.x_vec))
        self.identifier = title
        self.limits = limits
        self.y_label = y_label

        # init plot things
        plt.ion()
        fig = plt.figure(figsize=(13, 6))
        ax = fig.add_subplot(111)
        # create a variable for the line so we can later update it
        self.line1, = ax.plot(self.x_vec, self.y_vec, '-o', alpha=0.8)
        # update plot label/title
        plt.ylabel(y_label)
        plt.title(title)
        if not limits == False:
            plt.ylim(limits)
        plt.show()

    def add_plot_point(self, point):
        new_value = 0
        try:
            new_value = float(point)
        except ValueError:
            pass
        self.y_vec[-1] = new_value

        # after the figure, axis, and line are created, we only need to update the y-data
        self.line1.set_ydata(self.y_vec)

        if self.limits == False:  # Autoscale if enabled
            if np.min(self.y_vec) <= self.line1.axes.get_ylim()[0] or np.max(self.y_vec) >= self.line1.axes.get_ylim()[1]:
                plt.ylim([np.min(self.y_vec)-np.std(self.y_vec),
                         np.max(self.y_vec)+np.std(self.y_vec)])

        # this pauses the data so the figure/axis can catch up
        plt.pause(0.1)

        self.y_vec = np.append(self.y_vec[1:], 0.0)
