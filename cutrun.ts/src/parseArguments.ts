
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

export type Options = {
  files: string[],
  libraries: string[],
  includePath: string[],
  libraryPath: string[],
  outFolder: string,
  reporter: string,
  onlyExpandMacros: boolean,
  flagToComplete?: string
}

const defaultOptions: Options = {
  files: [],
  libraries: [],
  includePath: [],
  libraryPath: [],
  outFolder: './.cutrun_objects/',
  reporter: 'default',
  onlyExpandMacros: false
}


const isFlag = (arg: string) =>
  arg.startsWith('-')


const getFlagName = (arg: string) =>
  arg.startsWith('--') ? arg : arg.charAt(1)


const getCompleteFlagName = (flag: string) =>
  when(flag)
    .is('-I', '--include-path')
    .is('-L', '--library-path')
    .is('-l', '--library')
    .else(flag)


const parseArguments = (args: string[]): Options =>
  args.reduce((options: Options, arg: string) =>

    // Previous arg was a flag
    when(true)
      .is(options.flagToComplete !== undefined, () =>
        compose(
          opts =>
            set(opts, _ => _.flagToComplete)(undefined),

          (opts: Options) =>
            when(opts.flagToComplete)
              .is('--include-path', () =>
                set(opts, _ => _.includePath)(paths =>
                  paths.concat(arg)
                )
              )
              .is('--library-path', () =>
                set(opts, _ => _.libraryPath)(paths =>
                  paths.concat(arg)
                )
              )
              .is('--library', () =>
                set(opts, _ => _.libraries)(libs =>
                  libs.concat(arg)
                )
              )
              .is('--reporter', () =>
                set(opts, _ => _.reporter)(arg)
              )
              .else(() => {
                throw new Error(
                  `Unknown flag ${options.flagToComplete}`
                )
              })
        )(options)
      )

      // Current arg is a flag
      .is(isFlag(arg), () =>
        when(getCompleteFlagName(arg))
          .is('--only-expand-macros', () =>
            set(options, _ => _.onlyExpandMacros)(true)
          )
          .else(flagName =>
            set(options, _ => _.flagToComplete)(flagName)
          )
      )

      // Current arg is a file
      .else(() =>
        set(options, _ => _.files)(files => files.concat(arg))
      )
    , defaultOptions)

export default parseArguments
