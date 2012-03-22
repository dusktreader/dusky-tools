#!/usr/bin/env python
# -*- coding: utf-8 -*-

#############################################################################
#                                                                           #
# Note that the terminal profile itself must be setup correctly,            #
# And some compiz settings must be enabled as well.                         #
#                                                                           #
# See http://maketecheasier.com/terminal-as-transparent-wallpaper-in-ubuntu #
# for a basic guide                                                         #
#                                                                           #
#############################################################################

from optparse import OptionParser
import os
from subprocess import Popen
from time import sleep
import elementtree.ElementTree as ET

def main():
    parser = OptionParser()
    parser.add_option( "-d", "--delay", dest="delay", type="int", default=0,
                       help="Wait DELAY seconds before running script", metavar="DELAY" )
    parser.add_option( '-k', '--prekill', action='store_true', dest='prekill', default=False,
                       help="Kill gnome-terminal before launching" )
    parser.add_option( '-t', '--title', dest='title', type='string', default='transparentbackgroundterminal',
                       help='Capture all terminals with TITLE', metavar='TITLE' )
    ( options, args ) = parser.parse_args()
    
    if os.environ.get( 'DESKTOP_SESSION' ) != 'gnome':
        parser.error( 'This script only functions correctly in the Gnome Desktop Environment' )
    
    if options.prekill:
        os.system( 'pkill gnome-terminal' )
        sleep( 1 )
    
    if options.delay < 0:
        parser.error( "delay may not be negative" )
        
    sleep( options.delay )
            
    procs = []
    
    home_path = os.path.expanduser( '~' )    
    cfg_path = os.path.join( home_path, '.bgterm/bgterm.xml' )
    
    tree = ET.parse( cfg_path )
    root = tree.getroot()
    for terminal_cfg in root.findall( 'terminal' ):
        terminal_dict = {}
        width  = int( terminal_cfg.get( 'width',   80 ) )
        height = int( terminal_cfg.get( 'height',  40 ) )
        xoff   = int( terminal_cfg.get( 'xoff',   100 ) )
        yoff   = int( terminal_cfg.get( 'yoff',    40 ) )
        cmd    = terminal_cfg.get( 'cmd' )
        
        proc_cmd = 'gnome-terminal --window-with-profile=%s --geometry=%ix%i+%i+%i' % ( options.title,
                                                                                        width, height, xoff, yoff )
        if cmd != None:
            proc_cmd += ' -x %s' % cmd
            
        procs.append( Popen( [ proc_cmd ], shell=True ) )

# @todo  add a sigterm propagation to terminals

if __name__ == '__main__':
    main()

