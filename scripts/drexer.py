#! /usr/bin/env python

import os, sys
import subprocess
from argparse import ArgumentParser
from ConfigParser import ConfigParser

def main():

    default_archive_dir = '.'
    default_extension_dir = '.'
    
    try:
        config = ConfigParser() 
        config.read( os.path.dirname( __file__ ) + os.sep + 'drexer.cfg' )
        default_archive_dir = config.get( 'defaults', 'archive_dir' )
        default_sites_dir   = config.get( 'defaults', 'extension_dir' )
    except:
        pass

    parser = ArgumentParser( description="Install drupal modules or themes",
                             usage="usage: %(prog)s [ options ] module0_expr, module1_expr, ..." )

    parser.add_argument( '-a', '--archive_dir',
                         action='store', default=default_archive_dir,
                         help="Set the source folder that holds the Drupal extension archive(s) to DIR", metavar='DIR' )

    parser.add_argument( '-e', '--extension_dir',
                         action='store', default=default_extension_dir,
                         help="Set the install base folder for the Drupal extension(s) to DIR", metavar='DIR' )

    parser.add_argument( '-M', '--install_as_modules',
                         action='store_true', default=True,
                         help="Tell drexer to install extension(s) as modules(s) (installs as module(s) by default)" )

    parser.add_argument( '-T', '--install_as_themes',
                         action='store_false', dest='install_as_modules',
                         help="Tell drexer to install extension(s) as theme(s) (installs as module(s) by default)" )

    # Set the file name for the bundle
    parser.add_argument( '-v', '--verbose',
                         action='store_true',
                         help="Print too much information" )
    
    # Set the name of the file to edit
    parser.add_argument( 'extensions',
                         nargs='*' )

    args = parser.parse_args()
    
    if args.install_as_modules:
        install_dir = args.extension_dir + os.sep + 'modules'
    else:
        install_dir = args.extension_dir + os.sep + 'themes'
    
    if args.verbose:
        print >> sys.stderr, "Looking for Drupal extensions in: {archive_dir}".format( archive_dir = args.archive_dir )
        print >> sys.stderr, "Installing Drupal extensions as {ext_type} to:  {install_dir}".format(
            ext_type = "modules" if args.install_as_modules else "themes",
            install_dir = install_dir )
        
    if len( args.extensions ) == 0:
        parser.error( "No extensions to install!" )
        
    file_list = os.listdir( args.archive_dir )
        
    for extension in args.extensions:
    
        if args.verbose:
            print >> sys.stderr, "Looking for extensions matching base: {extension}".format( extension = extension )
    
        matches = [ f for f in file_list if extension in f ]
        
        if len( matches ) == 0:
            print >> sys.stderr, "Skipping {extension}".format( extension = extension )
            if args.verbose:
                print >> sys.stderr, "No archives in {archive_dir} match {extension}".format(
                    archive_dir = args.archive_dir,
                    extension = extension )
            continue
            
        elif len( matches ) > 1:
            print >> sys.stderr, "Skipping {extension}".format( extension = extension )
            if args.verbose:
                message  = "Inspecific extension description for {extension} \n" 
                message += "  In {archive_dir}, {extension} matches:       \n"
                message += "    {matches}"
                message  = message.format( extension = extension,
                                           archive_dir = args.archive_dir,
                                           matches = ' '.join( matches ) )
                print >> sys.stderr, message
            continue
            
        extension = matches[0]
        
        if args.verbose:
            print >> sys.stderr, "Found match in: {extension}".format( extension = extension )
        
        command = ''
        
        if extension.endswith( '.tar.gz' ) or extension.endswith( '.tgz' ):

            flag = 'xz'
            if args.verbose:
                flag += 'v'
                
            command = 'tar -{flag} -f {archive_dir}{sep}{extension} -C {install_dir}'
            command = command.format( flag = flag,
                                      archive_dir = args.archive_dir,
                                      sep = os.sep,
                                      extension = extension,
                                      install_dir = install_dir )
        
        elif extension.endswith( '.tar' ):

            flag = 'x'
            if args.verbose:
                flag += 'v'
                
            command = 'tar -{flag} -f {archive_dir}{sep}{extension} -C {install_dir}'
            command = command.format( flag = flag,
                                      archive_dir = args.archive_dir,
                                      sep = os.sep,
                                      extension = extension,
                                      install_dir = install_dir )
        
        elif extension.endswith( '.zip' ):

            flag = ''
            if args.verbose:
                flag += 'v'
                
            command = 'unzip -{flag} {archive_dir}{sep}{extension} -d {install_dir}'
            command = command.format( flag = flag,
                                      archive_dir = args.archive_dir,
                                      sep = os.sep,
                                      extension = extension,
                                      install_dir = install_dir )
        
        else:
            print >> sys.stderr, "Skipping {extension}".format( extension = extension )
            if args.verbose:
                print >> sys.stderr, "Unknown archive extension for {extension}".format( extension = extension )
            continue
            
        if args.verbose:
            print >> sys.stderr, "Calling command: {command}".format( command = command )
            
        subprocess.call( command, shell=True )
    
    if args.verbose:
        print >> sys.stderr, "Done"        
        
        

if __name__ == '__main__':
    main()
