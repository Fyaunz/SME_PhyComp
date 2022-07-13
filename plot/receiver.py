import serial
import argparse
from plot.plotter import plotter
import numpy as np


def main(target, baud, title, limits, y_label):

    print(target, baud, "dsafd")

    plotterInstance = plotter(title, limits, y_label)

    with serial.Serial(target, baud, timeout=1) as ser:
        while True:
            line = ser.readline()

            decoded = line.decode('utf-8')
            print(decoded)
            plotterInstance.add_plot_point(decoded)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Compute difference between two situations.')
    parser.add_argument('target',  type=str,
                        help='Target, e.g. /dev/ttyUSB0')

    parser.add_argument('baud', type=int, help='Baud rate')
    parser.add_argument('-t', '--title', type=str,
                        help='Title for the graph', default="My Graph")

    parser.add_argument("-l", '--limits', type=int,
                        help='limits if for scaling, leave empty for autoscaling', nargs="*")

    parser.add_argument("-y", '--y-label', type=str,
                        help='label for y axis')

    args = parser.parse_args()

    limits = vars(args).get("limits")

    if limits == None or not len(limits) == 2:
        print("given != 2 values for limits, using autoscaling instead")
        limits = False

    values = vars(args)

    main(values.get("target"), values.get("baud"),
         values.get("title"), limits, values.get("y_label"))
