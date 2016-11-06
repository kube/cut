#!/usr/bin/env node

      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

import * as mkdirp from 'mkdirp'
import parseArguments from './parseArguments'
import { Context, createContext } from './context'

const initialContext = createContext()
const args = process.argv.slice(2)

const createOutFolder = (context: Context) =>
  new Promise((resolve, reject) =>
    mkdirp(context.outFolder, (err, made) =>
      err ? reject(err) : resolve()
    )
  )

parseArguments(initialContext)(args)
  .then(createOutFolder)
  .then(name =>
    console.log(name)
  )
