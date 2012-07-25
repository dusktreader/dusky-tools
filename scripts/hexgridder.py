#! /usr/bin/env python

from argparse import ArgumentParser
from math import log

def main():

    parser = ArgumentParser( usage="usage: %(prog)s [ options ]" )

    # Set the directory in which to work
    parser.add_argument( '-s', '--side_size',
                         action='store', default=6, type=int,
                         help="Set the size of a side of the hex grid to SIZE", metavar='SIZE' )

    args = parser.parse_args()
    
    n = args.side_size ** 2
    print n
    step = int( log( n, 10 ) ) + 1
    
    grid_string = ''
    for i in range( n ):
        if i % args.side_size == 0:
            grid_string += '\n' + ' ' * ( i / args.side_size ) * step
        grid_string += '{0:0>{step}}'.format( i, step=step )
        grid_string += ' ' * step
    
    print grid_string
        
        

if __name__ == '__main__':
    main()
