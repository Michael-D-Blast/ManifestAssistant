#!/usr/bin/env python3

import os, sys
import subprocess
from dot import *


def get_workspace() :
	output = subprocess.getstatusoutput("repo home")
	
	if not output[0] == 0 :
		print("repo home failed!")
		return None

	if not "Build" in output[1] :
		print("We're not in a workspace!")
		return None
	else :
		return os.path.abspath(os.path.join(output[1], ".."))

def get_new_components(current_component_names) :
    selected_components = []

    while True :
        selected_components.clear()

        index = 1
        for cn in current_component_names :
            print("{}) {}".format(index, cn))
            index += 1

        user_input = input("Please select the components you'd like to bump (sperate the numbers with space) : ")

        input_numbers = user_input.split()
        for n in input_numbers :
            selected_components.append(current_component_names[int(n) - 1])

        print("The components you selected are :")
        for sc in selected_components :
            print(sc)
        
        user_input = input("Please confirm the components you input (y:confirmed/n:re-select/q:quit from program) : ")

        while user_input not in ('y', 'n', 'q') :
            user_input = input("Invalid input : {}. Please confirm the components you input (y:confirmed/n:re-select/q:quit from program) : ".format(user_input))

        if user_input == 'y' :
            return selected_components
        elif user_input == 'n' :
            continue
        elif user_input == 'q' :
            sys.exit(1)
        else :
            assert False, "Invalid input"

def update_manifest(current_components, new_components) :
	print("Start to update repo-manifest :")

if __name__=="__main__":
	import argparse

	parser = argparse.ArgumentParser(description="maintain the repo-manifest automatically")
	parser.add_argument("--platform", default="arm", help="specify the workspace you are working in")
	parser.add_argument("--entry", default="Esmeralda", help="specify the entry to build the whole project")
	args = parser.parse_args()

#	#workspace = get_workspace()
#	workspace = "/home/abb/cnws"
#	if not workspace :
#		print("Can't get workspace!")
#		exit(-1)
#
#	dot_file = workspace + "/obj/" + ("armbuildroot" if args.platform == "arm" else "linux") + "/" + args.entry + "/repo.dot"
#
#	dot = Dot(dot_file)
	dot = Dot("repo.dot")
	dot.parse_dot()
	dot.component_pairs

	selected_components = get_new_components(dot.component_names)

	update_manifest(dot.all_components_in_reverse_order, selected_components)

	exit(0)
