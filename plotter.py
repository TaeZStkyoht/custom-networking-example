import pandas as pd
import matplotlib.pyplot as plt
import time
import os
import signal
import sys

FILE_PATH = "build/bin/result.csv"

running = True

def signal_handler(sig, frame):
    global running
    running = False
    print("Shutting down...")

signal.signal(signal.SIGINT, signal_handler)

# Clean start
plt.close('all')
plt.ion()

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))

last_mtime = 0

while running:
    try:
        # Exit if window is closed
        if not plt.fignum_exists(fig.number):
            print("Window closed by user.")
            break

        mtime = os.path.getmtime(FILE_PATH)

        if mtime != last_mtime:
            last_mtime = mtime

            df = pd.read_csv(FILE_PATH)

            if df.shape[1] < 4:
                print("CSV must contain at least 4 columns.")
                continue

            # Get column names dynamically
            columns = df.columns

            # First 3 columns → providers queue
            col1 = df[columns[0]]
            col2 = df[columns[1]]
            col3 = df[columns[2]]

            # 4th column → processor queue
            col4 = df[columns[3]]

            # Clear axes
            ax1.clear()
            ax2.clear()

            # Top plot: Providers queue
            ax1.plot(col1, label=columns[0])
            ax1.plot(col2, label=columns[1])
            ax1.plot(col3, label=columns[2])
            ax1.set_title("Providers Queue")
            ax1.legend()
            ax1.grid()

            # Bottom plot: Processor priority queue
            ax2.plot(col4, label=columns[3])
            ax2.set_title("Processor Priority Queue")
            ax2.legend()
            ax2.grid()

            plt.tight_layout()

            print("Plot updated.")

        # Always refresh UI (even if file unchanged)
        fig.canvas.draw_idle()
        fig.canvas.flush_events()

        time.sleep(1)

    except Exception as e:
        print("Error:", e)
        time.sleep(1)

# Graceful shutdown
plt.ioff()
plt.close(fig)
sys.exit(0)
