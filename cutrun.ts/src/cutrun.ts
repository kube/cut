#!/usr/bin/env node

      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

import * as program from 'commander'
import { createContext } from './Context'

const collect = (value: string, arr: string[]) =>
  arr.concat(value)

program
  .version('0.0.1')
  .usage('[options] <file ...>')
  .option('-I, --include-path <path>', 'Add include path', collect, [])
  .option('-L, --library-path <path>', 'Add library path', collect, [])
  .option('-l, --library <library>', 'Add library', collect, [])
  .option('-r, --reporter <reporter>', 'Set reporter')
  .option('--only-expand-macros', 'Only expand macros and output source')

const cutrunArgs = program.parse(process.argv)
console.log(cutrunArgs)
