#! /usr/bin/env python

import os
import re
import subprocess

def run_tests():
	expected_regex = re.compile("EXPECTED: (?P<result>.*)")

	for program in os.listdir("./interpreter/programs"):
		# Fetch the expected result(s)
		f = open(program)
		expected_results = [m.group('result') for l in f.readlines() for m in [expected_regex.search(l)] if m]

		# Execute the program
		p = subprocess.Popen(['TosLang', '-interpret', program], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		output, err = p.communicate()

		# Report
		test_passed = output == expected_results
		print "TEST: {}: {}".format(program, "PASSED" if test_passed else "FAILED")

if __name__ == 'main'
	run_tests()