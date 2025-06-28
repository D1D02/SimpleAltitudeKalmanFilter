import tkinter as tk
from tkinter import messagebox, filedialog
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
import pandas as pd
import subprocess
import os
import datetime

DELTA_TIME = 0.250
PROJECT_ROOT = './'
EXAMPLE_CSV_PATH = os.path.join(PROJECT_ROOT, 'test', 'example.csv')
FILTERED_CSV_PATH = os.path.join(PROJECT_ROOT, 'test', 'filtered.csv')
EXECUTABLE_PATH = os.path.join(PROJECT_ROOT, 'build', 'example') # Path of build C example

def run_c_program_and_plot():

    try:
        # Running make to compile cose
        print(f"Compiling C code: {PROJECT_ROOT}...")
        make_process = subprocess.run(['make'], cwd=PROJECT_ROOT, check=True, 
                                      capture_output=True, text=True)
        print("Output 'make':\n", make_process.stdout)
        if make_process.stderr:
            print("Error 'make':\n", make_process.stderr)
        print("'make' executed correctly.")

        # Running /build/example
        print(f"Running example '{EXECUTABLE_PATH}'...")
        c_program_process = subprocess.run([EXECUTABLE_PATH], check=True, 
                                          capture_output=True, text=True)
        print("Output C:\n", c_program_process.stdout)
        if c_program_process.stderr:
            print("Error running C:\n", c_program_process.stderr)
        print("C executed correctly.")
        

        # Upload CSV files
        print(f"Loading csv files '{EXAMPLE_CSV_PATH}' e '{FILTERED_CSV_PATH}'...")
        
        # Upload example.csv
        df_example = pd.read_csv(EXAMPLE_CSV_PATH, header=0) 
        df_example.rename(columns={'Alt(A)': 'Alt_Unfiltered', 'Acc(z)': 'Acc_Unfiltered'}, inplace=True)
        
        # Upload filtered.csv
        df_filtered = pd.read_csv(FILTERED_CSV_PATH, header=0)
        df_filtered.rename(columns={'Alt': 'Alt_Filtered', 'Vel': 'Vel_Filtered', 'Acc': 'Acc_Filtered'}, inplace=True)

        print("Uploaded data.")

        # Plot data
        plot_data(df_example, df_filtered)

    except subprocess.CalledProcessError as e:
        messagebox.showerror("Execution error.", 
                             f"Error during execution of command:\n{e.cmd}\nOutput:\n{e.stdout}\nErrore:\n{e.stderr}")
        print(f"Error during execution of command: {e}")
    except FileNotFoundError as e:
        messagebox.showerror("File not found", 
                             f"Wrong paths.\nErrore: {e}")
        print(f"File not found: {e}")
    except pd.errors.EmptyDataError:
        messagebox.showerror("CSV erro", "csv file wrong format.")
        print("Error: csv file wrong format.")
    except Exception as e:
        messagebox.showerror("Unknown error", f"Unexpected error: {e}")
        print(f"Unexpected error: {e}")

def plot_data(df_example, df_filtered):

    global canvas, toolbar, fig, ax

    # Clear previous graph
    if 'fig' in globals() and fig is not None:
        plt.close(fig) # Close the old figure

    fig, ax = plt.subplots(3, 1, figsize=(12, 10), sharex=True) 
    fig.suptitle('Comparing Raw and Filtered data', fontsize=16)
    time_axis_example = df_example.index * DELTA_TIME

    # Altitude
    ax[0].plot(time_axis_example, df_example['Alt_Unfiltered'], label='Altitude', color='blue', alpha=0.7)
    ax[0].plot(time_axis_example, df_filtered['Alt_Filtered'], label='Altitude filtered', color='red', linestyle='--')
    ax[0].set_ylabel('Altitude')
    ax[0].legend()
    ax[0].grid(True, linestyle=':', alpha=0.6)

    # Acceleration
    ax[1].plot(time_axis_example, df_example['Acc_Unfiltered'], label='Acceleration', color='green', alpha=0.7)
    ax[1].plot(time_axis_example, df_filtered['Acc_Filtered'], label='Acceleration filtered', color='purple', linestyle='--')
    ax[1].set_ylabel('Acceleration')
    ax[1].legend()
    ax[1].grid(True, linestyle=':', alpha=0.6)

    # Velocity
    ax[2].plot(time_axis_example, df_filtered['Vel_Filtered'], label='Velocity filtered', color='orange')
    ax[2].set_xlabel('Time (250ms)')
    ax[2].set_ylabel('Velocity')
    ax[2].legend()
    ax[2].grid(True, linestyle=':', alpha=0.6)

    plt.tight_layout(rect=[0, 0.03, 1, 0.96]) 

    if 'canvas' in globals() and canvas is not None:
        canvas.get_tk_widget().destroy()
    if 'toolbar' in globals() and toolbar is not None:
        toolbar.destroy()

    canvas = FigureCanvasTkAgg(fig, master=plot_frame)
    canvas_widget = canvas.get_tk_widget()
    canvas_widget.pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    toolbar = NavigationToolbar2Tk(canvas, plot_frame)
    toolbar.update()
    canvas.draw()

def take_screenshot():

    if 'fig' in globals() and fig is not None:
        timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = f"screenshot_{timestamp}.png"
        
        filepath = filedialog.asksaveasfilename(defaultextension=".png", 
                                                 initialfile=filename,
                                                 filetypes=[("PNG files", "*.png"), ("All files", "*.*")],
                                                 title="Save Screenshot")
        if filepath:
            try:
                fig.savefig(filepath, dpi=300, bbox_inches='tight')
                messagebox.showinfo("Screenshot", f"Screenshot saved as:\n{filepath}")
            except Exception as e:
                messagebox.showerror("Error", f"Impossibile to save screenshot: {e}")
    else:
        messagebox.showwarning("No plot.", "No plot to save.")

def close_app():

    if messagebox.askyesno("Close", "Are you sure?"):
        root.quit()
        root.destroy()


root = tk.Tk()
root.title("Simple Kalman Altitude Filter")
root.geometry("1000x800") 

button_frame = tk.Frame(root)
button_frame.pack(side=tk.TOP, pady=10)

btn_run_plot = tk.Button(button_frame, text="Run C and Plot", command=run_c_program_and_plot)
btn_run_plot.pack(side=tk.LEFT, padx=10)

btn_screenshot = tk.Button(button_frame, text="Screenshot", command=take_screenshot)
btn_screenshot.pack(side=tk.LEFT, padx=10)

btn_close = tk.Button(button_frame, text="Close", command=close_app)
btn_close.pack(side=tk.LEFT, padx=10)

plot_frame = tk.Frame(root)
plot_frame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True, padx=10, pady=10)

fig = None
ax = None
canvas = None
toolbar = None

root.mainloop()

root.protocol("WM_DELETE_WINDOW", close_app)
