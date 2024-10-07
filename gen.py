import matplotlib.pyplot as plt
import numpy as np

# Test times
python_times = [326.8029332, 4121.93388, 232.4093046, 3902.92045, 2268.56321, 25741.4084]
cpfin_times = [3, 200.4, 13, 191, 116, 1504]

# Calculate speedups
speedups = [py_time / my_time for py_time, my_time in zip(python_times, cpfin_times)]

# Number of tests
tests = np.arange(len(python_times))

# Bar width
bar_width = 0.35

# Create the plot
fig, ax = plt.subplots(figsize=(10, 6))

# Plot Python times
bars1 = ax.bar(tests, python_times, bar_width, label='Python Times', color='blue')

# Plot your system's times
bars2 = ax.bar(tests + bar_width, cpfin_times, bar_width, label='CPFin Times', color='green')

# Annotate the speedup on the bars
for i, (py_time, cpfin_time, speedup) in enumerate(zip(python_times, cpfin_times, speedups)):
    ax.annotate(f'x{speedup:.1f}',  # Display the speedup value
                xy=(i + bar_width/2, max(py_time, cpfin_time)),  # Position the annotation
                xytext=(0, 3),  # Offset slightly above the bar
                textcoords="offset points",
                ha='center', va='bottom', fontsize=10, color='black')

# Add labels, title, and legend
ax.set_xlabel('Test Case')
ax.set_ylabel('Time (ms)')
ax.set_title('Python vs CPFin Times with Speedup Annotations')
ax.set_xticks(tests + bar_width / 2)
ax.set_xticklabels([f'Test {i+1}' for i in range(len(python_times))])
ax.legend()

# Show the plot
plt.tight_layout()
plt.show()
