from pyvisual import *
import subprocess

# subprocess.run(['make', 'test'])
# subprocess.run(['./check'])

pyvi = PyVi("data/transient.pyvi")

#pyvi.save_section_plots(0, 'figs/')

sec = pyvi.display_all_sections()