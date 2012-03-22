#!/usr/bin/env python

import os, sys

def main():
	args = ""
	args += "--lang=english "
	args += "--theme=elementary "
	args += "--cpu=16 "
	args += "--swap "
	args += "--updates "
	args += "--clock=lucky "
	args += "--hd=mix "
	args += "--hdtemp1=sdc "
	args += "--hdtemp2=sdb "
	args += "--proc=5 "
	args += "--nvidia "
	args += "--calendar "
	args += "--network "
	args += "--ubuntu "
	#args += "--banshee=glassy "
	os.system( "conky-colors " + args )

if __name__ == '__main__':
	main()

	    
