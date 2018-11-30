#!/usr/bin/env python3

import os
import subprocess
from dot import *

def foo() :
	return "abc"

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

def update_manifest(current_components, new_components) :
	print("Start to update repo-manifest :")

if __name__=="__main__":
	import argparse

	parser = argparse.ArgumentParser(description="maintain the repo-manifest automatically")
	parser.add_argument("--platform", default="arm", help="specify the workspace you are working in")
	parser.add_argument("--entry", default="Esmeralda", help="specify the entry to build the whole project")
	args = parser.parse_args()

	#workspace = get_workspace()
	workspace = "/home/abb/cnws"
	if not workspace :
		print("Can't get workspace!")
		exit(-1)

	dot_file = workspace + "/obj/" + ("armbuildroot" if args.platform == "arm" else "linux") + "/" + args.entry + "/repo.dot"

	dot = Dot(dot_file)
	dot.parse_dot()
	dot.component_pairs
	dot.components

	update_manifest(dot.all_components_in_reverse_order, ["Terra"])

	exit(0)
