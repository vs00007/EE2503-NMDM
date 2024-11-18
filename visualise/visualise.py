from pyvisual import *
import subprocess

subprocess.run(['make', 'test'])
subprocess.run(['./check'])

pyvi = PyVi("visualise/data.pyvi")

sec = pyvi.display_all_sections()

sec.show()