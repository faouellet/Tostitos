#! /usr/bin/env python

import os
import re
import subprocess
import sys

import pdb

def run_tests(test_dir, exec_dir):
	expected_regex = re.compile("EXPECTED: (?P<result>.*)")
	
	for program in os.listdir(test_dir):
		# Fetch the expected result(s)
		f = open(os.path.join(test_dir, program))
		expected_results = [m.group('result') for l in f.readlines() for m in [expected_regex.search(l)] if m]

		# Execute the program
		p = subprocess.Popen([os.path.join(exec_dir, 'TosLang'), '-interpret', program], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		output, err = p.communicate()

		# Report
		test_passed = output == expected_results
		print "TEST: {}: {}".format(program, "PASSED" if test_passed else "FAILED")


if __name__ == "__main__":
	if len(sys.argv) != 3:
		print "ERROR"
	run_tests(sys.argv[1], sys.argv[2])