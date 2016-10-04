from subprocess import call
from shutil import copyfile
import os, os.path
import yaml
import numpy as np
import sys

current_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.abspath(os.path.join(current_path,"../../tools")))
import sumatra_tracking.run_simulator as st


def modify_phase(phase):
    config_data["stimulus"]["phase"] = float(phase)

def modify_spatial_freq(d):
    config_data["stimulus"]["kId"] = int(d)

def modify_temp_freq(d):
    config_data["stimulus"]["wId"] = int(d)

def modify_diameter(d):
    config_data["stimulus"]["maskSize"] = float(d)


def modify_arc(a):
    config_data["relay"]["Krc"]["spatial"]["a"] = float(a)

def modify_wrc(w):
    config_data["relay"]["Krc"]["w"] = float(w)


def modify_aic(a):
    config_data["interneuron"]["Kic"]["spatial"]["a"] = float(a)

def modify_wic(w):
    config_data["interneuron"]["Kic"]["w"] = float(w)


def modify_aig(a):
    config_data["interneuron"]["Kig"]["spatial"]["a"] = float(a)

def modify_wig(w):
    config_data["interneuron"]["Kig"]["w"] = float(w)


def modify_ari(a):
    config_data["relay"]["Kri"]["spatial"]["a"] = float(a)

def modify_wri(w):
    config_data["relay"]["Kri"]["w"] = float(w)



#read config file--------------------------------------------------------------
options = sys.argv[1:]
record_label = options[-1]
config_file = os.path.abspath(os.path.join(current_path, record_label+".yaml"))
copyfile(os.path.abspath(os.path.join(current_path,"firingSynchrony.yaml")), config_file)

with open(config_file, 'r') as stream:
    config_data = yaml.load(stream)


#parameters---------------------------------------------------------------------
w_ic = [0, 0.8, 2.4, 4.0]
w_rc = [0, 0.18, 0.54, 0.9]
spatial_freqs = range(0, 15)
#run simulator--------------------------------------------------------------------

counter= 0
modify_diameter(1.69)
for wi, wr in zip(w_ic,w_rc):
    modify_wic(wi)
    modify_wrc(wr)
    for Kd in spatial_freqs:
        modify_temp_freq(Kd)
##########################################################
        with open(config_file, 'w') as stream:
            yaml.dump(config_data, stream)

        run_id = '{0:04}'.format(counter)
        st.run_simulator(config_file, record_label, run_id)
        counter+=1
os.remove(config_file)
##########################################################
