#! /usr/bin/env python
import os, sys
from optparse import OptionParser

def main():

    parser = OptionParser( usage="usage: %prog [ options ] args" )

    # Set the interactive interpreter that you want to use
    parser.add_option( '-i', '--interpreter', type='string', action='store', dest='interpreter', default='ipython',
                       help="Set the interactive interpreter to INTP", metavar='INTP' )

    ( options, args ) = parser.parse_args()
    
    if len( parser.largs ) > 0:
        sys.argv = parser.largs[ 1: ]
        execfile( parser.largs[0] )
        
    else:
        os.system( options.interpreter )
        
if __name__ == '__main__':
    main()
