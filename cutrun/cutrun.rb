#!/usr/bin/env ruby

      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

$__CUTRUN_FOLDER__ = File.dirname(
  File.symlink?(__FILE__) ?
    File.readlink(__FILE__) : __FILE__)

# Set cutrun folder as loadpath
$:.unshift $__CUTRUN_FOLDER__

module CutRun
  VERSION = "0.0.0a"
end

require 'arguments_parser'
require 'context'

options = ArgumentsParser.parse

# Add Cut headers to include_path
options['include_path'] << "#{$__CUTRUN_FOLDER__}/../include"

cut = CutRun::Context.new options
cut.run
