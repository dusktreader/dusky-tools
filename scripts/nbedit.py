#! /usr/bin/env python

import os, sys
import subprocess as sp
from argparse import ArgumentParser

def main():

    parser = ArgumentParser( usage="usage: %(prog)s [ options ] [ editor_args ( including target file ) ]" )

    # Set the directory in which to work
    parser.add_argument( '-e', '--editor',
                         action='store', default='gedit',
                         help="Set the selected editor to ED", metavar='ED' )

    # Set the file name for the bundle
    parser.add_argument( '-l', '--log',
                         action='store', default='/dev/null',
                         help="Redirect stdout from the editor to LOG", metavar='LOG' )

    # Set the file name for the bundle
    parser.add_argument( '-r', '--err',
                         action='store', default='/dev/null',
                         help="Redirect stderr from the editor to ERR", metavar='ERR' )

    # Set the file name for the bundle
    parser.add_argument( '-s', '--superuser',
                         action='store_true',
                         help="Run the editor as the super user (root)" )

    # Set the file name for the bundle
    parser.add_argument( '-v', '--verbose',
                         action='store_true',
                         help="Print too much information" )
    
    # Set the name of the file to edit
    parser.add_argument( 'editor_args',
                         nargs='*' )

    args = parser.parse_args()
    
    if args.verbose:
        print >> sys.stderr, "Using editor: %s" % args.editor
        if args.superuser == True:
            print >> sys.stderr, "Running as Super User"
        print >> sys.stderr, "Redirecting stdout from editor to: %s" % args.log
        print >> sys.stderr, "Redirecting stderr from editor to: %s" % args.err
        print >> sys.stderr, "Passing as args to editor: %s" % argstring
    
    command = 'which {editor} > /dev/null'
    command = command.format( editor = args.editor )
    
    if sp.call( command, shell=True ) != 0:
        parser.error( "No such editor found: %s" % args.editor )
        
    command = '{superuser} nohup {editor} {argstring} > {log} 2> {err} < /dev/null &'
    command = command.format( superuser = "gksu" if args.superuser == True else "",
                              editor    = args.editor,
                              argstring = ' '.join( args.editor_args ),
                              log       = args.log,
                              err       = args.err )
    
    if sp.call( command, shell=True ) != 0:
        parser.error( "Error opening edtior" )
        
        

if __name__ == '__main__':
    main()
