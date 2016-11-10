
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

import { set } from 'monolite'
import { join } from 'path'

const CUT_FOLDER = join(__dirname, '../..')

export type Context = {
  files: string[],
  libraries: string[],
  includePath: string[],
  libraryPath: string[],
  outFolder: string,
  reporter: string,
  onlyExpandMacros: boolean
}

export const addIncludePath = (context: Context) =>
  (path: string) =>
    set(context, _ => _.includePath)(paths =>
      paths.concat(path)
    )

export const addLibraryPath = (context: Context) =>
  (path: string) =>
    set(context, _ => _.libraryPath)(paths =>
      paths.concat(path)
    )

export const addLibrary = (context: Context) =>
  (library: string) =>
    set(context, _ => _.libraries)(libs =>
      libs.concat(library)
    )

export const addFile = (context: Context) =>
  (file: string) =>
    set(context, _ => _.files)(files =>
      files.concat(file)
    )

export const setReporter = (context: Context) =>
  (reporter: string) =>
    set(context, _ => _.reporter)(reporter)

export const setOnlyExpandMacros = (context: Context) =>
  set(context, _ => _.onlyExpandMacros)(true)

export const createContext = () => ({
  files: [],
  libraries: [],
  includePath: [join(CUT_FOLDER, 'include')],
  libraryPath: [],
  outFolder: './.cutrun_objects/',
  reporter: 'default',
  onlyExpandMacros: false
})
