
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

import when from 'when-switch'
import { set } from 'monolite'
import { compose } from 'ramda'
import * as Promise from 'bluebird'

import {
  Context, createContext,
  setOnlyExpandMacros, setReporter,
  addIncludePath, addLibraryPath, addLibrary, addFile
} from './context'

/**
 * State of the Parser
 */
type ParsingState = {
  context: Context,
  flagToComplete: string | null
}

const isFlag = (arg: string) =>
  arg.startsWith('-')

const getFlagName = (arg: string) =>
  arg.startsWith('--') ? arg : arg.charAt(1)

/**
 * Get complete flag name if alias or return same if not an alias
 */
const getCompleteFlagName = (flag: string) =>
  when(flag)
    .is('-I', '--include-path')
    .is('-L', '--library-path')
    .is('-l', '--library')
    .else(flag)

/**
 * Complete previous flag with current argument value
 */
const completeFlag = (state: ParsingState) => (value: string) =>
  new Promise<ParsingState>((resolve, reject) =>
    when(state.flagToComplete)
      .is('--include-path', () => resolve(
        set(state, _ => _.context)(context =>
          addIncludePath(context)(value)
        )
      ))
      .is('--library-path', () => resolve(
        set(state, _ => _.context)(context =>
          addLibraryPath(context)(value)
        )
      ))
      .is('--library', () => resolve(
        set(state, _ => _.context)(context =>
          addLibrary(context)(value)
        )
      ))
      .is('--reporter', () => resolve(
        set(state, _ => _.context)(context =>
          setReporter(context)(value)
        )
      ))
      .else(() => reject(
        new Error(`Unknown flag ${state.flagToComplete}`)
      ))
  )
    // Unset flag to complete on ParsingState
    .then(state =>
      set(state, _ => _.flagToComplete)(null)
    )

/**
 * Handle current
 */
const handleFlag = (state: ParsingState) => (flag: string) =>
  when(getCompleteFlagName(flag))
    .is('--only-expand-macros', () =>
      set(state, _ => _.context)(context =>
        setOnlyExpandMacros(context)
      )
    )
    //TODO: Check if current flag is a valid flag name
    .else(flagName =>
      set(state, _ => _.flagToComplete)(flagName as string)
    )

/**
 * Parse given arguments and return a new updated Context
 */
const parseArguments = (context: Context) =>
  (args: string[]): Promise<Context> =>
    Promise.reduce(args, (state: ParsingState, arg: string) =>

      when(true)
        .is(state.flagToComplete !== null, () =>
          completeFlag(state)(arg)
        )
        .is(isFlag(arg), () =>
          handleFlag(state)(arg)
        )
        // Current arg is a file
        .else(() =>
          set(state, _ => _.context)(context =>
            addFile(context)(arg)
          )
        )
      , {
        context: context,
        flagToComplete: null
      })
      // Return context
      .then(state =>
        state.context
      )
      // Catch errors during parsing
      .catch((err: Error) => {
        console.log(`CutRun: Error while parsing arguments: ${err.message}`)
        process.exit(1)
      })

export default parseArguments
