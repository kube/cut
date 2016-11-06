#!/usr/bin/env node

      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

import parseArguments from './parseArguments'

const options = parseArguments(process.argv.slice(2))

console.log(options)
